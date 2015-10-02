#include <windows.h>
#include "resource.h"
#include <cmath>

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
HWND hStatic,hStatic1;
WORD x,y;
LONG ssizex,ssizey,radius;
RECT mainw;
RECT rStatik;
void run(HWND hWnd,LONG x,LONG y,LONG x0,LONG y0){
	LONG x1=rStatik.left;
	LONG y1=rStatik.top;

	if ((x<x0)&&(y<y0)) {
		x1=rStatik.left+3;y1=rStatik.top+3;
	} else 
		if ((x>x0)&&(y<y0)) {x1=rStatik.left-3;y1=rStatik.top+3;}
		else 
			if ((x<x0)&&(y>y0)) {x1=rStatik.left+3;y1=rStatik.top-3;}
			else 
				if ((x>x0)&&(y>y0)) {x1=rStatik.left-3;y1=rStatik.top-3;}

				if (x1<0) x1=0;
				if (x1>mainw.right-ssizex) x1=mainw.right-ssizex;
				if (y1<0) y1=0;
				if (y1>mainw.bottom-ssizey) y1=mainw.bottom-ssizey;
				MoveWindow(hStatic,x1,y1,ssizex,ssizey,1);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	// создаём главное окно приложения на основе немодального диалога
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	// Отображаем окно	
	ShowWindow(hDialog, nCmdShow); 
	//Запускаем цикл обработки сообщений
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{


	switch(mes)
	{
	case WM_INITDIALOG:
		hStatic=CreateWindow(TEXT("Static"),TEXT("!!!! "),WS_VISIBLE | WS_CHILD| WS_BORDER,181,128,80,40,hWnd,(HMENU)10,NULL,NULL);
		GetClientRect(hWnd,&mainw);
		return true;
	case WM_CLOSE:
		// закрываем немодальный диалог
		DestroyWindow(hWnd); // разрушаем окно
		PostQuitMessage(0); // останавливаем цикл обработки сообщений
		return TRUE;
	case WM_MOUSEMOVE:
		LONG x0,y0,d;
		hStatic=GetDlgItem(hWnd,10);
		GetWindowRect(hStatic,&rStatik);
		ssizex=rStatik.right-rStatik.left;
		ssizey=rStatik.bottom-rStatik.top;
		MapWindowPoints(NULL, hWnd, (LPPOINT)&rStatik, 2);
		x0=rStatik.left+ssizex/2;
		y0=rStatik.top+ssizey/2;
		radius=std::sqrt(ssizex*ssizex+ssizey*ssizey)/2;
		TCHAR a[20];
		TCHAR b[20];
		x=LOWORD(lp);
		y=HIWORD(lp);
		wsprintf(a,TEXT("X=%d | Y=%d"),LOWORD(lp),HIWORD(lp));
		SetWindowText(hStatic,a);
		//||(abs(y-rStatik.top+10)<5)
		hStatic1=GetDlgItem(hWnd,IDC_STATIC2);
		d=sqrt(pow(x-x0,2)+pow(y-y0,2));
		wsprintf(b,TEXT("D=%d x=%d y=%d"),d,rStatik.left,y0);
		SetWindowText(hStatic1,b);
		if (d<radius+3)
			run(hStatic,x,y,x0,y0);
		return TRUE;


	}
	return FALSE;
}