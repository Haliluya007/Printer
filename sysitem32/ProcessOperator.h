#ifndef __PROCESSOPERSTOR_H__
#define __PROCESSOPERSTOR_H__
#pragma once
#include "Windows.h"
namespace ProcessOperator
{
	DWORD ProcessAlloc(HANDLE hProcess,DWORD size,DWORD nProtection=PAGE_EXECUTE_READWRITE);				//在目标进程里分配大小为size的一段内存
	BOOL ProcessFree(HANDLE hProcess,DWORD addr, DWORD size);
	DWORD ProcessRead(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD nReadLen);
	DWORD ProcessWrite(HANDLE hProcess,DWORD nAddr,DWORD nWriteLen,LPCVOID pBuffer);
	HANDLE ProcessOpen(DWORD pid,DWORD nAccess=PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION);
	BOOL ProcessClose(HANDLE hProcess);
	DWORD ProcessReadTextA(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen);	//ncMaxReadLen是字符数，并且包含了结束符号，所以ncMaxReadLen为字符串实际最大长度+1
	DWORD ProcessReadTextW(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen);
	DWORD ProcessReadText(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen);		//注意ncMaxBufferLen是缓冲区最大容纳的字符数，返回的的读取的字符串长度，无论成功否还是目标没有结束符号函数也会保证pBuffer里是一个NULL结尾的字符串
	CString ProcessReadText(HANDLE hProcess,DWORD nAddr,DWORD ncMaxLen);
}
#endif