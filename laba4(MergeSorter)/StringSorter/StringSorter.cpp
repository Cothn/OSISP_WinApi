#include "stdafx.h"
#include "pch.h"
#include "StringSorter.h"
using namespace std;

#define DELIMITER '\n'

StringSorter::StringSorter(int threadCount)
{
	this->threadCount = threadCount;
	this->threadPool = new ThreadPool(threadCount);
	runtimeThreadCount = 0;
	InitializeCriticalSection(&lock);
}

VOID StringSorter::sort(string inputFile, string outputFile)
{
	vector<string>* stringVector = getStrings(inputFile);
	vector<vector<string>*>* stringVectors = divideVector(stringVector, threadCount);
	delete stringVector;

	TASK *tasks = new TASK[stringVectors->size()];
	PARAM *params = new PARAM[stringVectors->size()];
	for (int i = 0; i < stringVectors->size(); i++)
	{
		params[i].sorter = this;
		params[i].stringVector = stringVectors->at(i);
		tasks[i].func = sortStringVector;
		tasks[i].param = &params[i];
		EnterCriticalSection(&lock);
		runtimeThreadCount++;
		threadPool->enqueueTask(&tasks[i]);
		LeaveCriticalSection(&lock);
	}

	while (true)
	{
		EnterCriticalSection(&lock);
		if (!runtimeThreadCount) { break; }
		LeaveCriticalSection(&lock);
		Sleep(200);
	}
	delete[] params;
	delete[] tasks;
	vector<string> *result = mergeSortedVectors(stringVectors);
	delete stringVectors;
	writeStringsToFile(outputFile, result);
}

StringSorter::~StringSorter()
{
	threadPool->Close();
}

vector<string>* StringSorter::getStrings(string inputFile)
{
	ifstream ifstream(inputFile);
	vector<string>* result = new vector<string>();
	while (!ifstream.eof())
	{
		string str;
		getline(ifstream, str, DELIMITER);
		str += DELIMITER;
		result->push_back(str);
	}
	ifstream.close();
	return result;
}

void StringSorter::writeStringsToFile(string file, vector<string>* strings)
{
	ofstream stream(file);
	for (string str : *strings)
	{
		const char *cstr = str.c_str();
		stream.write(cstr, strlen(cstr));
	}
	stream.close();
	delete strings;
}

vector<vector<string>*>* StringSorter::divideVector(vector<string>* strings, int numOfGroups)
{
	vector<vector<string>*>* result = new vector<vector<string>*>();
	// Strings per group
	size_t count = strings->size() / numOfGroups + 1;
	int index = 0;
	for (int i = 0; i < numOfGroups; i++)
	{
		if (i >= strings->size() % numOfGroups)
		{
			count--;
		}
		vector<string>* stringVector = new vector<string>();
		for (int j = 0; j < count && index < strings->size(); j++)
		{
			stringVector->push_back(strings->at(index));
			index++;
		}
		result->push_back(stringVector);
	}
	return result;
}

DWORD WINAPI StringSorter::sortStringVector(LPVOID param)
{
	PARAM *paramStruct = (PARAM*)param;
	StringSorter *sorter = paramStruct->sorter;
	vector<string> *vector = paramStruct->stringVector;
	std::sort(vector->begin(), vector->end());
	EnterCriticalSection(&(sorter->lock));
	sorter->runtimeThreadCount--;
	LeaveCriticalSection(&(sorter->lock));
	return 0;
}

vector<string>* StringSorter::mergeSortedVectors(vector<vector<string>*>* vectors)
{

	vector<int> indexVector(vectors->size(), 0);
	vector<string>* resVector = new vector<string>();

	size_t stringsCount = 0;
	for (int i = 0; i < vectors->size(); i++)
	{
		stringsCount += vectors->at(i)->size();
	}

	int minStringIndex = 0;
	while (stringsCount)
	{
		string minString = initMinString(vectors, indexVector, &minStringIndex);

		for (int i = 0; i < vectors->size(); i++)
		{
			if (indexVector.at(i) < vectors->at(i)->size() && (vectors->at(i)->at(indexVector.at(i)) < minString))
			{
				minString = vectors->at(i)->at(indexVector.at(i));
				minStringIndex = i;
			}
		}

		resVector->push_back(minString);
		indexVector.at(minStringIndex)++;
		stringsCount--;
	}

	return resVector;
}

string StringSorter::initMinString(vector<vector <string>*>* vectors, vector<int> indexVector, int *minStringIndex)
{
	string minString;
	for (int i = 0; i < vectors->size(); i++)
	{
		if (indexVector.at(i) < vectors->at(i)->size())
		{
			*minStringIndex = i;
			minString = vectors->at(i)->at(indexVector.at(i));
		}
	}
	return minString;
}