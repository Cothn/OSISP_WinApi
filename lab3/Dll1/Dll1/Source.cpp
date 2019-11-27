#include <windows.h>
#include <stdio.h>
#include "stdafx.h"
#include <iostream>
#include <string>
#include <winnt.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <string>
#include <vector>

SYSTEM_INFO sysinfo;
void find_in_next_pages(int j, LPBYTE startadr, LPBYTE adr, char* searchstr, char* newstr, int count_symb_to_replace);

extern "C" int __declspec(dllexport) _stdcall changestr(char* searchstr, char* newstr, int count_symb_to_replace)
{
	int searchstrlen = strlen(searchstr);
	MEMORY_BASIC_INFORMATION info;
	info.State = MEM_FREE;
	std::vector<char> buffer;
	std::vector<char> oldbuffer;
	GetSystemInfo(&sysinfo);
	LPBYTE adr = (LPBYTE)sysinfo.lpMinimumApplicationAddress;
	LPBYTE startadr = adr;
	int position = 0;
	int j = 0;
	int i = 0;
	int pageindex = 0;

	while ((adr < (LPBYTE)sysinfo.lpMaximumApplicationAddress) && (info.State != MEM_COMMIT))
	{
		adr += sysinfo.dwPageSize;
		VirtualQueryEx(GetCurrentProcess(), adr, &info, sizeof(info));
	}

	while (adr < (LPBYTE)sysinfo.lpMaximumApplicationAddress)
	{
		VirtualQueryEx(GetCurrentProcess(), adr, &info, sizeof(info));
		if (info.State == MEM_COMMIT)
		{
			DWORD bytes_read;
			position = 0;
			buffer.resize(sysinfo.dwPageSize);
			ReadProcessMemory(GetCurrentProcess(), adr, &buffer[0], sysinfo.dwPageSize, &bytes_read);
			buffer.resize(bytes_read);
			if (bytes_read > 0)
			{
				for (i = 0; i < buffer.size(); i++)
				{
					pageindex = 0;
					position++;
					for (j; j < searchstrlen; j++)
					{
						if ((i + pageindex) >= buffer.size())
						{
							find_in_next_pages(j, startadr, adr + sysinfo.dwPageSize, searchstr, newstr, count_symb_to_replace);
							break;
						}
						if (buffer[i + pageindex] != searchstr[j])
						{
							j = 0;
							break;
						}
						if (j == 0) startadr = adr + position;
						pageindex++;
						if ((searchstrlen - 1) == j)
						{
							j = 0;
							WriteProcessMemory(GetCurrentProcess(), startadr - 1, newstr, count_symb_to_replace, 0);
						}
					}
				}
			}
		}
		adr += sysinfo.dwPageSize;
	}
	return 0;
}

void find_in_next_pages(int j, LPBYTE startadr, LPBYTE adr, char* searchstr, char* newstr, int count_symb_to_replace)
{
	int position;
	std::vector<char> buffer;
	MEMORY_BASIC_INFORMATION info;
	int pageindex = 0;
	int i = 0;
	int searchstrlen = strlen(searchstr);
	if (adr < (LPBYTE)sysinfo.lpMaximumApplicationAddress)
	{
		VirtualQueryEx(GetCurrentProcess(), adr, &info, sizeof(info));
		if (info.State == MEM_COMMIT)
		{
			DWORD bytes_read;
			position = 0;
			buffer.resize(sysinfo.dwPageSize);
			ReadProcessMemory(GetCurrentProcess(), adr, &buffer[0], sysinfo.dwPageSize, &bytes_read);
			buffer.resize(bytes_read);
			if (bytes_read > 0)
			{
				for (i = 0; i < buffer.size(); i++)
				{
					pageindex = 0;
					position++;
					for (j; j < searchstrlen; j++)
					{
						if ((i + pageindex) >= buffer.size())
						{
							find_in_next_pages(j, startadr, adr + sysinfo.dwPageSize, searchstr, newstr, count_symb_to_replace);
							return;
							//break;
						}
						if (buffer[i + pageindex] != searchstr[j])
						{
							j = 0;
							break;
							return;
						}
						if (j == 0) startadr = adr + position;
						pageindex++;
						if ((searchstrlen - 1) == j)
						{
							j = 0;
							WriteProcessMemory(GetCurrentProcess(), startadr - 1, newstr, count_symb_to_replace, 0);
						}
					}
				}
			}
		}
		adr += sysinfo.dwPageSize;
	}
}