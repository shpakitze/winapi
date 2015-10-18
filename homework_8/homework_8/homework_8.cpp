#include <windows.h>
#include <tchar.h>
#include "resource.h"
//#include <commctrl.h>
//#pragma comment(lib,"comctl32")
BOOL CALLBACK MainProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hThisInstance;
HWND hMainWnd,hEdit;
void OpenFile();
void SaveFile();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	hThisInstance=hInstance;
	//	InitCommonControls();

	TCHAR szClassName[] = TEXT("MyClass");
	MSG msg;
	WNDCLASSEX wc;
	// Заполняем структуру класса окна
	wc.cbSize        = sizeof(wc);		
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	// Регистрируем класс окна
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Cannot register class"), TEXT("Error"), MB_OK);
		return 0;
	}
	// Создаем основное окно приложения
	hMainWnd = CreateWindow( 
		szClassName, TEXT("NOTEPAD"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		(HWND)NULL, (HMENU)NULL,
		(HINSTANCE)hInstance, NULL
		);

	if (!hMainWnd) {
		MessageBox(NULL, TEXT("Cannot create main window"), TEXT("Error"), MB_OK);
		return 0;
	}
	HMENU hMenu=LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(hMainWnd,hMenu);

	RECT rc;
	GetClientRect(hMainWnd,&rc);
	//hEdit=CreateWindow(TEXT("Edit"),TEXT(""),0, 0, rc.right,rc.bottom, ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL,);
	hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"),TEXT(""),WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL,0,0,rc.right,rc.bottom,hMainWnd,(HMENU)ID_EDIT,hInstance,0);
	ShowWindow(hMainWnd, nCmdShow); 
	while (GetMessage(&msg,NULL,0,0)) {
		if (!IsDialogMessage(hMainWnd,&msg)) {
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
	return msg.wParam; 
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_OPEN1:
			OpenFile();
			break;
		case ID_FILE_SAVE1:
			SaveFile();
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
void OpenFile(){
	TCHAR filename[MAX_PATH]=TEXT(""); /*буфер имени файла*/ 
	OPENFILENAME ofn;
	TCHAR* buf;
	HANDLE hf;
	DWORD len,len1;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = NULL;
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = TEXT("Please Select a File");
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0;
	ofn.lpfnHook = 0;
	ofn.lpTemplateName = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	GetOpenFileName(&ofn);

	/* открываем файл */
	hf=CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if (hf==INVALID_HANDLE_VALUE) {
		MessageBox(hMainWnd,TEXT("Open failed"),TEXT("Error"),MB_ICONHAND|MB_OK);
	}
	len=GetFileSize(hf,NULL);
	buf=new TCHAR[len+1]; /* доп. байт под символ-терминатор (0) */
	ZeroMemory(buf, sizeof(TCHAR)*len+1);
	if (!buf) {
		MessageBox(hMainWnd,TEXT("Mem alloc failed"),TEXT("Error"),MB_ICONHAND|MB_OK);

	}
	ReadFile(hf,buf,len,&len1,NULL);
	buf[len1]=0;
	CloseHandle(hf);
	SetDlgItemText(hMainWnd,ID_EDIT,buf);
	delete[] buf;
}

void SaveFile(){
DWORD len,len1;
TCHAR* buf;
OPENFILENAME ofn;
TCHAR filename[MAX_PATH] =TEXT("");
HANDLE hf;
ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = NULL;
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = TEXT("Please Select a File");
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0;
	ofn.lpfnHook = 0;
	ofn.lpTemplateName = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	!GetSaveFileName(&ofn);
	hf=CreateFile(filename,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	len=SendDlgItemMessage(hMainWnd,ID_EDIT,WM_GETTEXTLENGTH,0,0);
	buf=new TCHAR[len+1]; /* доп. байт под символ-терминатор (0) */
	GetDlgItemText(hMainWnd,ID_EDIT,buf,len+1);
	if (!buf) {
		MessageBox(hMainWnd,TEXT("Mem alloc failed"),TEXT("Error"),MB_ICONHAND|MB_OK);
	}
	WriteFile(hf,buf,len,&len1,NULL);
	CloseHandle(hf);
	delete[] buf;
}
/* диалоговая процедура */
//BOOL CALLBACK MainProc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
// static DWORD OldIcon=0;    /* id старой иконки диалога */
// static OPENFILENAME of;
// TCHAR* buf;
// HANDLE hf; 
// DWORD len,len1;
// switch (msg) {
//  case WM_INITDIALOG:       /* меняем иконку диалога */
//   OldIcon=SetClassLong(hw,GCL_HICON,(long)LoadIcon(hThisInstance,TEXT("Ex5_Icon")));
//   return TRUE;
//  case WM_COMMAND:
//   switch (LOWORD(wp)) {
//    case IDCANCEL:          /* посылается при закрытии диалога по [Esc]*/
//    case IDC_EXIT:          /* команда меню "Exit" */
//     DestroyWindow(hw);
//     break;
//    case IDC_OPEN:          /* команда меню "Open" */
//     of.lStructSize=OPENFILENAME_SIZE_VERSION_400A;
//     of.hwndOwner=hw;
//     of.lpstrFilter=TEXT("All files (*.*)\0*.*\0");
//     of.lpstrCustomFilter=NULL; of.nMaxCustFilter=0;
//     of.nFilterIndex=1;
//     of.lpstrFile=filename; of.nMaxFile=MAX_PATH;
//     of.lpstrFileTitle=NULL; of.nMaxFileTitle=0;
//     of.lpstrInitialDir=NULL;
//     of.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
//     if (!GetOpenFileName(&of)) break;
//     SetDlgItemText(hw,ID_STATUS,filename);
//     /* открываем файл */
//     hf=CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
//     if (hf==INVALID_HANDLE_VALUE) {
//      MessageBox(hw,TEXT("Open failed"),TEXT("Error"),MB_ICONHAND|MB_OK);
//      break;
//     }
//     len=GetFileSize(hf,NULL);
//     buf=(TCHAR*)malloc(len+1); /* доп. байт под символ-терминатор (0) */
//     if (!buf) {
//      MessageBox(hw,TEXT("Mem alloc failed"),TEXT("Error"),MB_ICONHAND|MB_OK);
//      break;
//     }
//     ReadFile(hf,buf,len,&len1,NULL);
//     buf[len1]=0;
//     CloseHandle(hf);
//     SetDlgItemText(hw,ID_EDIT,buf);
//     free(buf);
//     break;
//    case IDC_SAVEAS:        /* команда меню "Save As" */
//     of.lStructSize=OPENFILENAME_SIZE_VERSION_400A;
//     of.hwndOwner=hw;
//     of.lpstrFilter=TEXT("All files (*.*)\0*.*\0");
//     of.lpstrCustomFilter=NULL; of.nMaxCustFilter=0;
//     of.nFilterIndex=1;
//     of.lpstrFile=filename; of.nMaxFile=MAX_PATH;
//     of.lpstrFileTitle=NULL; of.nMaxFileTitle=0;
//     of.lpstrInitialDir=NULL;
//     of.Flags=OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
//     if (!GetSaveFileName(&of)) break;
//    case IDC_SAVE:          /* команда меню "Save" */
//     if (lstrlen(filename)==0) {
//      /* для нового файла - вызываем диалог "Save As" */
//      PostMessage(hw,WM_COMMAND,IDC_SAVEAS,lp);
//      break;
//     }
//     SetDlgItemText(hw,ID_STATUS,filename);
//     /* сохраняем файл */
//     hf=CreateFile(filename,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
//     if (hf==INVALID_HANDLE_VALUE) {
//      MessageBox(hw,TEXT("Save failed"),TEXT("Error"),MB_ICONHAND|MB_OK);
//      break;
//     }
//     len=SendDlgItemMessage(hw,ID_EDIT,WM_GETTEXTLENGTH,0,0);
//     buf=(TCHAR*)malloc(len+1); /* доп. байт под символ-терминатор (0) */
//     GetDlgItemText(hw,ID_EDIT,buf,len+1);
//     if (!buf) {
//      MessageBox(hw,TEXT("Mem alloc failed"),TEXT("Error"),MB_ICONHAND|MB_OK);
//      break;
//     }
//     WriteFile(hf,buf,len,&len1,NULL);
//     CloseHandle(hf);
//     free(buf);
//     break;
//    case IDC_ABOUT:         /* команда меню "About" */
//     MessageBox(hw,TEXT(" "),TEXT("About"),MB_OK|MB_ICONINFORMATION);
//     break;
//   }
//   return TRUE;
//  case WM_DESTROY:          /* при закрытии окна восстанавливаем старую иконку */
//   SetClassLong(hw,GCL_HICON,(long)OldIcon);
//   PostQuitMessage(0);
//   return TRUE;
// }
// return FALSE;
//}