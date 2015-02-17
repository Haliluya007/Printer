#include "StdAfx.h"
#include "ProcessOperator.h"
//#include "DrawLib.h"
using namespace ProcessOperator;

Reader_SysListView32::Reader_SysListView32(void)
{
}

Reader_SysListView32::~Reader_SysListView32(void)
{
}

void Reader_SysListView32::Reset(void)
{
	ClearData();
	ClearHeader();
}

void Reader_SysListView32::ClearData(void)
{
	row.clear();
}

void Reader_SysListView32::ClearHeader(void)
{
	header.clear();
}

BOOL Reader_SysListView32::Read(HWND hwnd)
{
	Reset();

	int cols=GetCols(hwnd);
	if(!cols)return 0;
	int rows=GetRows(hwnd);

	//DWORD pid=GetWindowPid(hwnd);
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if(!pid)return 0;
	HANDLE hp=ProcessOpen(pid);
	if(!hp)return 0;
	DWORD nMemSize=64*1024;
	DWORD rmAddr=ProcessAlloc(hp,nMemSize);
	if(!rmAddr)return 0;
	cols=ReadHeader(hp,hwnd,rmAddr,nMemSize);
	if(rows)
	{
		ReadTable(hp,hwnd,rmAddr,nMemSize,cols);
	}
	ProcessFree(hp,rmAddr,nMemSize);
	ProcessClose(hp);
	return 1;
}

int Reader_SysListView32::ReadHeader(HANDLE hProcess,HWND hwnd, DWORD nRemoteBufferAddress, DWORD nBufferSize)
{

	//CString sClass=GetWindowClass(hwnd);	//有些窗口类不是"SysListView32"也能被读取，如exescope里的表格，可能是它的派生类
	int nCols=GetCols(hwnd);
	if(!nCols)return 0;

	HWND hHeader=GetHeaderWnd(hwnd);

	DWORD nBaseSize=128;	//比sizeof(HDITEM)大一点
	DWORD nTextAddr=nRemoteBufferAddress+nBaseSize;
	DWORD nBuffLen=(nBufferSize-nBaseSize)/sizeof(TCHAR);
	if(nBufferSize<=nBaseSize+2)return 0;

	TCHAR *pText=(TCHAR*)new char[nBufferSize];
	HDITEM hi={0};
	for(int i=0;i<nCols;i++)
	{
		hi.cchTextMax=nBuffLen;
		hi.pszText=(TCHAR*)nTextAddr;
		hi.mask=HDI_TEXT;
		if (!ProcessWrite(hProcess, nRemoteBufferAddress, nBaseSize, &hi))break;
		if(::SendMessage(hHeader,HDM_GETITEM,i,nRemoteBufferAddress))
		{
			ProcessReadText(hProcess, nTextAddr, pText, nBufferSize / sizeof(TCHAR));
			header.push_back(pText);
		}
	}
	delete pText;
	return nCols;
}

int Reader_SysListView32::ReadTable(HANDLE hProcess,HWND hwnd, DWORD nRemoteBufferAddress, DWORD nBufferSize,int nCols)
{
	//LVM_GETCOLUMN也能检测列数量，可以从0列开始一个个测试，但是如果不是报表风格则可能错误。一旦是报表风格则一定会有头控件所以依然使用头控件返回的列数nCols

	int nRows=GetRows(hwnd);
	if(!nRows)return 0;

	DWORD nBaseSize=128;	//比sizeof(LVITEM)大一点
	DWORD nTextAddr=nRemoteBufferAddress+nBaseSize;
	DWORD nBuffLen=(nBufferSize-nBaseSize)/sizeof(TCHAR);
	if(nBufferSize<=nBaseSize+2)return 0;

	TCHAR *pText=(TCHAR*)new char[nBufferSize];
	LVITEM li={0};
	Row BlankRow;
	for(int r=0;r<nRows;r++)
	{
		Reader_SysListView32::row.push_back(BlankRow);
		Row&row=Reader_SysListView32::row[Reader_SysListView32::row.size()-1];
		for(int c=0;c<nCols;c++)
		{
			li.cchTextMax=nBuffLen;
			li.pszText=(LPTSTR)nTextAddr;
			li.mask=LVIF_TEXT;
			li.iItem=r;
			li.iSubItem=c;
			if (!ProcessWrite(hProcess, nRemoteBufferAddress, nBaseSize, &li))break;
			if(::SendMessage(hwnd,LVM_GETITEM,0,nRemoteBufferAddress))
			{
				ProcessReadText(hProcess, nTextAddr, pText, nBufferSize / sizeof(TCHAR));
				row.push_back(pText);
			}
		}
	}
	delete pText;
	return nRows;
}

int Reader_SysListView32::GetCols(HWND hwnd)
{
	HWND hHeader=(HWND)::SendMessage(hwnd,LVM_GETHEADER,0,0);
	if(!::IsWindow(hHeader))return 0;
	int nCount=::SendMessage(hHeader,HDM_GETITEMCOUNT,0,0);
	return nCount;
}

int Reader_SysListView32::GetRows(HWND hwnd)
{
	int nCount=::SendMessage(hwnd,LVM_GETITEMCOUNT,0,0);
	return nCount;
}

HWND Reader_SysListView32::GetHeaderWnd(HWND hwnd)
{
	return (HWND)::SendMessage(hwnd,LVM_GETHEADER,0,0);
}

void Reader_SysListView32::ClearTable(HWND hwnd)
{
	if(!::IsWindow(hwnd))return;
	::SendMessage(hwnd,LVM_DELETEALLITEMS,0,0);
	int nCols=GetCols(hwnd);
	for(int i=nCols-1;i>=0;i--)
	{
		::SendMessage(hwnd,LVM_DELETECOLUMN,i,0);
	}
}

//BOOL Reader_SysListView32::Display(HWND hwnd)
// {
// 	if(!::IsWindow(hwnd))return 0;
// 	ClearTable(hwnd);
// 	HDC hdc=::GetWindowDC(hwnd);
// 
// 	int nCols=Reader_SysListView32::header.size();
// 	LVCOLUMN lc={0};
// 	lc.mask=LVCF_TEXT|LVCF_WIDTH;
// 	LPTSTR sText;
// 	for(int i=0;i<nCols;i++)
// 	{
// 		sText=(LPTSTR)(LPCTSTR)Reader_SysListView32::header[i];
// 		lc.cx=::GetTextWidth(sText,hdc)+10;
// 		if(lc.cx<20)lc.cx=20;
// 		lc.pszText=sText;
// 		::SendMessage(hwnd,LVM_INSERTCOLUMN,i,(LPARAM)&lc);
// 	}
// 	::ReleaseDC(hwnd,hdc);
// 
// 	LV_ITEM li1={0};
// 	LV_ITEM li2={0};
// 	int nItem;
// 	int nRows=Reader_SysListView32::row.size();
// 	for(int r=0;r<nRows;r++)
// 	{
// 		if((nItem=::SendMessage(hwnd,LVM_INSERTITEM,0,(LPARAM)&li1))!=-1)
// 		{
// 			for(int c=0;c<nCols;c++)
// 			{
// 				sText=(LPTSTR)(LPCTSTR)Reader_SysListView32::row[r][c];
// 				li2.pszText=sText;
// 				li2.mask=LVIF_TEXT;
// 				li2.iSubItem=c;
// 				::SendMessage(hwnd,LVM_SETITEMTEXT,nItem,(LPARAM)&li2);
// 			}
// 		}
// 	}
// 	return 1;
// }
