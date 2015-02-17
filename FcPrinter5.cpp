// FcPrinter5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FcPrinter5.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

 set<vector<CString>> setNeedPrint;
 set<vector<CString>> setBackUp;
 set<vector<CString>> setDif;
 

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam)
{

	wchar_t MyClassName[260] = { 0 };
	GetClassName(hwnd, MyClassName, 255);
	wchar_t MyWindowsText[260] = { 0 };
	GetWindowText(hwnd, MyWindowsText, 255);

	do
	{
		if (!StrCmpI(MyClassName, L"PrintUI_PrinterQueue"))
		{
			if (StrStrI(MyWindowsText, L"漫游打印"))
			{
				HWND hr = FindWindowEx(hwnd, NULL, L"SysListView32", L"");
				Reader_SysListView32 readList;
				BOOL bRet = readList.Read(hr);
				if (bRet)
				{
					//打印ListView 标签头
// 					//printf("OK!\n");
// 					vector<CString>::iterator iter = readList.header.begin();
// 					for (iter; iter != readList.header.end(); iter++)
// 					{
// 						//CString a = *iter;
// 						wstring xxx((*iter).GetBuffer());
// 						wprintf(L"%-100ws\t", xxx.c_str());
// 
// 					}
// 					printf("\n");

					//打印 ListView 各项
					vector<vector<CString>>::iterator it = readList.row.begin();
					for (it; it != readList.row.end(); it++)
					{
						setNeedPrint.insert(*it);
					}
					CloseHandle(hr);
				}
			}
		}
	} while (0);
	CloseHandle(hwnd);
	return TRUE;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	setlocale(LC_ALL, "");
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			fprintf(stdout, "Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			int count = 0;
			while (true)
			{
				::EnumWindows(::EnumWindowsProc, NULL);
				if (setBackUp.size() == 0 )
				{
					setBackUp = setNeedPrint;
				}
				if (setBackUp.size() != setNeedPrint.size())
				{
					int a = 0;
				}

				setDif.clear();
				set_difference(setNeedPrint.begin(), setNeedPrint.end(), setBackUp.begin(), setBackUp.end(), inserter(setDif, setDif.begin()));
			//	set_difference(setBackUp.begin(), setBackUp.end(), setNeedPrint.begin(), setNeedPrint.end(), inserter(setDif, setDif.end()));
// 				if (setNeedPrint.size() > setBackUp.size())
// 				{
// 					setBackUp = setNeedPrint;
// 				}
				if (setDif.size())
				{
					set<vector<CString>>::iterator iter = setDif.begin();
					for (iter; iter != setDif.end(); iter++)
					{
						vector<CString> vv = (*iter);
						vector<CString>::iterator it = vv.begin();
						//printf("count = %d\n", vv.size());
						for (it; it != vv.end(); it++)
						{
							wchar_t * lpcwBuf = (*it).GetBuffer();
							if (!lpcwBuf)
							{
								continue;
							}
							wstring aaa(lpcwBuf);
							//wprintf(L"%ws ",aaa.c_str());
							fprintf(stdout, "%ws ", aaa.c_str());
						}
						fprintf(stdout, "\n");
					}
					
					setBackUp = setNeedPrint;
				}
				Sleep(500);
				fflush(stdout);
			}
			

		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
