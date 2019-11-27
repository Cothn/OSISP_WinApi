// RemoteInjection.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <cstdio>

int main()
{ 
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	std::wstring procname(L"ConsoleApplication2.exe");
	char dllpath[] = "D:\\RepositHub\\OSISP_WinApi\\lab3\\ConsoleApplication2\\Dll2.dll";
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	HANDLE hTargetProc = NULL;
	const wchar_t* procName_wchar_t = procname.c_str();
	Process32First(snapshot, &pe32);
	do {
		//std::cout << pe32.szExeFile;
		if (wcscmp(pe32.szExeFile, procName_wchar_t) == 0)
		{
			//std::cout << pe32.szExeFile;
			HANDLE remoteThread = NULL;
			hTargetProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			LPVOID dllPathAddressInRemoteMemory = VirtualAllocEx(hTargetProc, NULL, strlen(dllpath), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			BOOL successfullyWritten = WriteProcessMemory(hTargetProc, dllPathAddressInRemoteMemory, dllpath, strlen(dllpath), NULL);
			LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
			if (successfullyWritten)
				remoteThread = CreateRemoteThread(hTargetProc, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddressInRemoteMemory, NULL, NULL);
			LPVOID lpFunctionAddress = GetProcAddress(GetModuleHandleA(dllpath), "_changestr@4");
			if (!lpFunctionAddress) 
				lpFunctionAddress = GetProcAddress(LoadLibraryA(dllpath), "_changestr@4");
			if (!lpFunctionAddress) std::cout << "Error";
			struct PARAMS
			{
				LPVOID oldstr;
				LPVOID newstr;
				int count_symb_to_replace;
			};
			//char oldstr[] = "Hello, World!";
			//char newstr[] = "World, Hello!";
			std::cout << "Enter string you want to change\n";
			char oldstr[512];
			std::cin.getline(oldstr, 512, '\n');
			std::cout << "Enter new string\n";
			char newstr[512];
			std::cin.getline(newstr, 512, '\n');
			//int count_symb_to_repl = 14;
			LPVOID oldStrAddrInMemory = VirtualAllocEx(hTargetProc, NULL, strlen(oldstr), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			successfullyWritten = WriteProcessMemory(hTargetProc, oldStrAddrInMemory, oldstr, strlen(oldstr), NULL);
			LPVOID newStrAddrInMemory = VirtualAllocEx(hTargetProc, NULL, strlen(newstr), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			successfullyWritten = WriteProcessMemory(hTargetProc, newStrAddrInMemory, newstr, strlen(newstr), NULL);
			PARAMS params;
			params.oldstr = oldStrAddrInMemory;
			params.newstr = newStrAddrInMemory;
			if (remoteThread != NULL)
				CloseHandle(remoteThread);
			params.count_symb_to_replace = 14;

			LPVOID ParamsAddressInRemoteMemory = VirtualAllocEx(hTargetProc, NULL, sizeof(params), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			//SIZE_T byteswritten;
			successfullyWritten = WriteProcessMemory(hTargetProc, ParamsAddressInRemoteMemory, &params, sizeof(params), NULL);
			if (successfullyWritten)
				remoteThread = CreateRemoteThread(hTargetProc, NULL, NULL, (LPTHREAD_START_ROUTINE)lpFunctionAddress, ParamsAddressInRemoteMemory, NULL, NULL);
			if (remoteThread != NULL)
				CloseHandle(remoteThread);
		}

	} while (Process32Next(snapshot, &pe32));

	if (hTargetProc != NULL)
	{
		CloseHandle(hTargetProc);
	}
	CloseHandle(snapshot);
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
