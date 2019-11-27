// ConsoleApplication2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <string.h>
#include <string>
#include <windows.h>

typedef int __stdcall Tdynchangestr (char*, char*, int);

extern "C" __declspec(dllimport) int _stdcall changestr(char* searchstr, char* newstr, int count_symb_to_replace);

int main()
{
	std::cout << "Choose: \n 1. Use static import \n 2. Use dynamic import \n 3. Use remote thread\n";
	int choice;
	std::cin >> choice;
	std::cout << "Enter string you want to change\n";
	char oldstrarr[512];
	std::cin.ignore();
	std::cin.getline(oldstrarr, 512, '\n');
	std::cout << "Enter new string\n";
	char newstrarr[512];
	std::cin.getline(newstrarr, 512, '\n');
	switch (choice)
	{
	case 1:
	{
		changestr(oldstrarr, newstrarr, strlen(oldstrarr));
		std::cout << oldstrarr;
		break;
	}
	case 2:
	{
		HINSTANCE getDLL = LoadLibrary(L"D:\\RepositHub\\OSISP_WinApi\\lab3\\ConsoleApplication2\\Dll1.dll");
		if (!getDLL)
		{
			std::cout << "Could not load dll.";
			return -1;
		}
		Tdynchangestr* dynfunc = (Tdynchangestr*)GetProcAddress(getDLL, "_changestr@12");
		if (dynfunc == NULL)
		{
			std::cout << "Could not find the function";
			return -1;
		}
		dynfunc(oldstrarr, newstrarr, strlen(oldstrarr));
		std::cout << oldstrarr;
		FreeLibrary(getDLL);
		break;
	}
	case 3:
	{
		while (true)
		{
			std::cout << oldstrarr;
			std::cout << "\n";
			Sleep(500);
		}
	}
	break;
	default:
	{
		std::cout << "Incorrect option chosen";
	}
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
