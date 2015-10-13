#include <windows.h>
#include "resource.h"
#include <cmath>
#include <tchar.h>
BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
HWND hRadioSumm,hRadioMulti,hRadioAver,hList,hBut,hSt;
DOUBLE sum,multi,aver;
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow){
	MSG msg;
	// создаём главное окно приложения на основе немодального диалога
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	// Отображаем окно	
	ShowWindow(hDialog, nCmdShow); 
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	HWND h;
	switch(mes)
	{
	case WM_INITDIALOG:
		hRadioSumm=GetDlgItem(hWnd,IDC_RADIO2);
		hRadioMulti=GetDlgItem(hWnd,IDC_RADIO3);
		hRadioAver=GetDlgItem(hWnd,IDC_RADIO4);
		hList=GetDlgItem(hWnd,IDC_LIST1);
		hBut=GetDlgItem(hWnd,IDC_BUTTON1);
		hSt=GetDlgItem(hWnd,IDC_STATIC1);
		return TRUE;
	case WM_COMMAND:
		h=GetFocus();

		if (h==hBut) {
			SendMessage(hList,LB_RESETCONTENT,0,0);
			sum=0;
			multi=1;
			int i=10+rand()%10;
			for (int j=0;j<i;++j){
				TCHAR buf[4];
				int num=std::pow(-1,rand()%2)*((rand()%10)+1);
				sum+=num;
				multi*=num;
				wsprintf(buf,TEXT("%d"),num);
				SendMessage(hList,LB_ADDSTRING,0,LPARAM(buf));
			}
			aver=sum/i;
			SendNotifyMessage(hRadioSumm,BM_SETCHECK,WPARAM(BST_UNCHECKED),0);
			SendNotifyMessage(hRadioMulti,BM_SETCHECK,WPARAM(BST_UNCHECKED),0);
			SendNotifyMessage(hRadioAver,BM_SETCHECK,WPARAM(BST_UNCHECKED),0);
			SetWindowText(hSt,TEXT(""));
		} else 
			if (h==hRadioAver)
			{
				TCHAR buf[40];
				_stprintf(buf,TEXT("%f"),aver);
				SetWindowText(hSt,buf);
			} else
				if (h==hRadioSumm)
				{
					TCHAR buf[40];
					_stprintf(buf,TEXT("%f"),sum);
					SetWindowText(hSt,buf);
				}else
					if (h==hRadioMulti)
					{
						TCHAR buf[40];
						_stprintf(buf,TEXT("%f"),multi);
						SetWindowText(hSt,buf);
					}

					return TRUE;
	case WM_CLOSE:
		// закрываем немодальный диалог
		DestroyWindow(hWnd); // разрушаем окно
		PostQuitMessage(0); // останавливаем цикл обработки сообщений
		return TRUE;
	}
	return FALSE;
}