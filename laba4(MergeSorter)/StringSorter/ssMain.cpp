// StringSorter.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include "stdafx.h"
#include "StringSorter.h"


int main()
{
	int threadCount = 0;
	string inputFile, outputFile;
	while (threadCount <= 0)
	{
		cout << "Number of threads: ";
		cin >> threadCount;
	}
	cout << "Input file: ";
	cin >> inputFile;
	cout << "Output file: ";
	cin >> outputFile;
	StringSorter sorter(threadCount);
	sorter.sort(inputFile, outputFile);
	printf("File was sorted\n");
	getchar();
	getchar();
	return 0;
}


