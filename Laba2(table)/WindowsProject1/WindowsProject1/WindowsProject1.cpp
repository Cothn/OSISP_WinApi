// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include <windows.h>
//#include <wingdi.h>
#include "Resource.h"


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szString1[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
WCHAR sz3dot[4];            // ...
#define Cwidth 4
#define Cheigth 8

void DrawTable(HDC hdc, int n, int m, RECT field);

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.


    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_STRING105, sz3dot, 4);
	LoadStringW(hInstance, IDS_STRING1, szString1, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // инициализация приложения:
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {

            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	//wcex.style = CS_DBLCLKS;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;

	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBRUSH hBrush; //создаём объект-кисть
	PAINTSTRUCT ps;
	HDC hdc; //создаём контекст устройства
	RECT rect;
	GetClientRect(hWnd, &rect);
	switch (message)
	{

	case WM_SIZE:
	{

		UpdateWindow(hWnd);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// код прорисовки, использующий HDC...
		DrawTable(hdc, Cwidth, Cheigth, rect);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
	void DrawTable(HDC hdc, int n, int m, RECT field)
	{
		LONG width = (field.right - field.left) / m;
		LONG height = (field.bottom - field.top) / n;
		int k, i, l;
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(250,200,100));//создание кисти
		SelectObject(hdc, hBrush);//выбор кисти
		for (k = 1; k < n; k++) {
			MoveToEx(hdc, field.left, k*height, NULL); //сделать текущими координаты 
			LineTo(hdc, field.right, k* height);
		}
		for (k = 1; k < m; k++) {
			MoveToEx(hdc, k * width, field.top, NULL); 
			LineTo(hdc, k * width, field.bottom);
		}
		SIZE Len;
		int strLen = 50;
		GetTextExtentPoint32(hdc, szTitle, strLen, &Len);
		int kolStr = (10+Len.cx) / width +1;
		int lastSym;
		for (k = 0; k < m; k++) {
			for (i = 0; i < n; i++) {
				lastSym = 0;
				l = 0;
				while (lastSym <= strLen ) {
						if ((i * height + (l+1) * Len.cy + 2 + Len.cy) <= height * (i+1)) {
							TextOut(hdc, k * width + 10, i * height + l * Len.cy + 2, &szString1[lastSym], strLen / kolStr);
							lastSym = lastSym + strLen / kolStr;
							l++;
						}
						else
						{
							SIZE len;
							GetTextExtentPoint32(hdc, &szString1[lastSym], strLen / kolStr - 3, &len);
							TextOut(hdc, k * width + 10, i * height + l * Len.cy + 2, &szString1[lastSym], strLen / kolStr - 3);
							TextOut(hdc, k * width + 10 + len.cx , i * height + l * Len.cy + 2, sz3dot, 3);
							break;
						}
					

				}
			}
		}

	}

