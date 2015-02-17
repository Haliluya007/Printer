#pragma once
#include <vector>
class Reader_SysListView32
{
public:
	typedef std::vector<CString>Row;
	Row header;
	std::vector<Row>row;
public:
	Reader_SysListView32(void);
	~Reader_SysListView32(void);
	BOOL Read(HWND hwnd);
	void Reset(void);
	void ClearData(void);
	void ClearHeader(void);
	BOOL Display(HWND hwnd);
	int ReadHeader(HANDLE hProcess,HWND hwnd, DWORD nRemoteBufferAddress, DWORD nBufferSize);
	int ReadTable(HANDLE hProcess,HWND hwnd, DWORD nRemoteBufferAddress, DWORD nBufferSize,int nCols);
	int GetCols(HWND hwnd);
	int GetRows(HWND hwnd);
	HWND GetHeaderWnd(HWND hwnd);
	void ClearTable(HWND hwnd);
};
