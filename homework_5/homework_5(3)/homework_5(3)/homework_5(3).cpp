#include <windows.h>
#include <ctime>
#include <tchar.h>
#include "resource.h"

HWND hCbox,hInfo;
DWORD disk_list;
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc); 
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR *disk=new TCHAR[3];
	TCHAR m[]={'\\'};
	DWORD type;
	switch(message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		hCbox=CreateWindow(TEXT("COMBOBOX"),TEXT(""),WS_CHILD|WS_VISIBLE|CBS_SORT|CBS_DROPDOWNLIST,10,10,200,80,hWnd,HMENU(100),0,0);
		hInfo=CreateWindow(TEXT("STATIC"),TEXT("File system"),WS_CHILD|WS_VISIBLE,210,10,200,200,hWnd,HMENU(1000),0,0);
		disk_list=GetLogicalDrives();
		for (int i=0;i<26;++i)
			if (disk_list&(1<<i)){
				TCHAR r[30];
				TCHAR a[30];
				_tcscpy(r,TEXT(" :\\ "));
				r[0] = static_cast<wchar_t>(65+i);
				type=GetDriveType(r);
				switch (type)
				{
				case DRIVE_UNKNOWN:
					_tcscpy(a,TEXT("DRIVE_UNKNOWN"));
					break;
				case DRIVE_NO_ROOT_DIR:
					_tcscpy(a,TEXT("DRIVE_NO_ROOT_DIR"));
					break;
				case DRIVE_REMOVABLE:
					_tcscpy(a,TEXT("DRIVE_REMOVABLE"));
					break;
				case DRIVE_FIXED:
					_tcscpy(a,TEXT("DRIVE_FIXED"));
					break;
				case DRIVE_REMOTE:
					_tcscpy(a,TEXT("DRIVE_REMOTE"));
					break;
				case DRIVE_CDROM:
					_tcscpy(a,TEXT("DRIVE_CDROM"));
					break;
				case DRIVE_RAMDISK:
					_tcscpy(a,TEXT("DRIVE_RAMDISK"));
					break;
				}
				_tcscat(r,a);
				SendMessage(hCbox,CB_ADDSTRING,0,LPARAM(r));
			}
			return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam)==100){
			DWORD ch=SendMessage(hCbox,CB_GETCURSEL,0,0);
			if (ch<4) {
				TCHAR buf[40];
				SendMessage(hCbox,CB_GETLBTEXT,ch,LPARAM(buf));
				TCHAR disk[4],disk1[4];
				_tcsnccpy(disk,buf,3);
				TCHAR kbuf[256];
				disk[3]='\0';
				BOOL b=GetVolumeInformation(disk,0,0,0,0,0,kbuf,256);
				
				if (b)
					SetWindowText(hInfo,kbuf);
			}
		}

		return TRUE;
	default:
		return FALSE;
	}
}