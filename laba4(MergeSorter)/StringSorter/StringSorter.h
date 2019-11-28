#pragma once

#include "Windows.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "ThreadPool.h"

using namespace std;

class StringSorter
{
public:
	StringSorter(int threadCount);
	void sort(string inputFile, string outputFile);
	~StringSorter();
private:
	static DWORD WINAPI sortStringVector(LPVOID param);

	ThreadPool *threadPool;
	int threadCount;
	int runtimeThreadCount;
	CRITICAL_SECTION lock;
	vector<string> *getStrings(string inputFile);
	vector<vector<string>*>* divideVector(vector<string>* strings, int count);
	vector<string>* mergeSortedVectors(vector<vector<string>*>* vectors);
	string initMinString(vector<vector <string>*>* vectors, vector<int> indexVector, int *minStringIndex);
	void writeStringsToFile(string file, vector<string>* strings);

	struct PARAM
	{
		StringSorter *sorter;
		vector<string> *stringVector;
	};
};

