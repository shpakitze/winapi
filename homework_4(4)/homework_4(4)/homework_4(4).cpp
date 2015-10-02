#include <windows.h>
#include "resource.h"
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
struct but{
	UINT pos;
	UINT val;
	HWND des;

};
TCHAR cap[30];
std::vector<but> bbut;
std::vector<UINT> field;
std::vector<HWND> field_b;
std::vector<UINT> field_pos;
UINT zerro;
BOOL win;
std::random_device rd;
std::mt19937 g(rd());
time_t game_time;
tm gtime;
HWND tim;
bool prov(){

	int inv = 0;
	for (int i=0; i<15; ++i)
		for (int j=0; j<i; ++j)
			if (bbut[j].val > bbut[i].val)
				++inv;

	inv+=4;
	return (inv & 1);
}

void start(){

	std::random_device rd1;
	std::mt19937 g1(rd1());
	std::shuffle(field.begin(),field.end(),g1);
	for (int i=0;i<15;++i){
		bbut[i].val=field[i];
		bbut[i].pos=i+1;
		wsprintf(cap,TEXT("%d"),field[i]);
		SetWindowText(bbut[i].des,cap);
		MoveWindow(bbut[i].des,50*((i)%4),50*((i)/4),50,50,1);
	}

	zerro=16;
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
	UINT style;
	style=WS_CHILD|WS_VISIBLE;


	switch(mes)
	{
	case WM_INITDIALOG:
		CreateWindow(TEXT("Button"),TEXT("New game"),style,300,0,100,50,hWnd,HMENU(1000),NULL,NULL);
		CreateWindow(TEXT("Button"),TEXT("prov"),style,300,100,100,50,hWnd,HMENU(1001),NULL,NULL);
		CreateWindow(TEXT("Button"),TEXT("Start"),style,300,50,100,50,hWnd,HMENU(1002),NULL,NULL);
		tim=CreateWindow(TEXT("Static"),TEXT("time"),style,300,200,100,50,hWnd,HMENU(1003),NULL,NULL);
		field.resize(15);
		field_b.resize(15);
		field_pos.resize(15);
		bbut.resize(15);
		for (int i=0;i<15;++i){
			field[i]=i+1;
		}
		std::shuffle(field.begin(),field.end(),g);
		//field.push_back(0);
		for (int i=0;i<15;++i){
			bbut[i].pos=i+1;
			bbut[i].val=field[i];
			wsprintf(cap,TEXT("%d"),field[i]);
			bbut[i].des=CreateWindow(TEXT("Button"),cap,style,50*(i%4),50*(i/4),50,50,hWnd,HMENU(i+1),NULL,NULL);
		}
		zerro=16;
		win=false;
		return true;
	case WM_COMMAND:
		if (LOWORD(wp)==1000) {
			game_time=0;
			start();
			return TRUE;
		}
		if (LOWORD(wp)==1001) {
			if (prov()) MessageBox(0,TEXT("!"),TEXT("bad"),MB_OK);
			return TRUE;
		}
		if (LOWORD(wp)==1002) {
			SetTimer(hWnd,1,1000,NULL);
			game_time=0;
			return TRUE;
		}
		if (bbut[LOWORD(wp)-1].pos==zerro+4) {
			MoveWindow(bbut[LOWORD(wp)-1].des,50*((zerro-1)%4),50*((zerro-1)/4),50,50,1);
			std::swap(bbut[LOWORD(wp)-1].pos,zerro);
		}
		else if (bbut[LOWORD(wp)-1].pos==zerro-4) {
			MoveWindow(bbut[LOWORD(wp)-1].des,50*((zerro-1)%4),50*((zerro-1)/4),50,50,1);
			std::swap(bbut[LOWORD(wp)-1].pos,zerro);

		} else if (bbut[LOWORD(wp)-1].pos==zerro-1) {
			MoveWindow(bbut[LOWORD(wp)-1].des,50*((zerro-1)%4),50*((zerro-1)/4),50,50,1);
			std::swap(bbut[LOWORD(wp)-1].pos,zerro);
		} else if (bbut[LOWORD(wp)-1].pos==zerro+1) {
			MoveWindow(bbut[LOWORD(wp)-1].des,50*((zerro-1)%4),50*((zerro-1)/4),50,50,1);
			std::swap(bbut[LOWORD(wp)-1].pos,zerro);
		}
		win=true;
		for (auto &i:bbut){
			if (i.pos!=i.val) win=false;
		}
		if ((zerro==16)&&(win)){
			KillTimer(hWnd,1);
			MessageBox(0,TEXT("WIN"),TEXT(""),MB_OK);
		}
		return TRUE;
	case WM_CLOSE:
		// закрываем немодальный диалог
		DestroyWindow(hWnd); // разрушаем окно
		PostQuitMessage(0); // останавливаем цикл обработки сообщений
		return TRUE;
	case WM_TIMER:
		++game_time;
		localtime_s(&gtime,&game_time);
		wsprintf(cap,TEXT("M:%d:S:%d"),gtime.tm_min,gtime.tm_sec);
		SetWindowText(tim,cap);
		return TRUE;
	}
	return FALSE;
}