#ifndef __PROCESSOPERSTOR_H__
#define __PROCESSOPERSTOR_H__
#pragma once
#include "Windows.h"
namespace ProcessOperator
{
	DWORD ProcessAlloc(HANDLE hProcess,DWORD size,DWORD nProtection=PAGE_EXECUTE_READWRITE);				//��Ŀ�����������СΪsize��һ���ڴ�
	BOOL ProcessFree(HANDLE hProcess,DWORD addr, DWORD size);
	DWORD ProcessRead(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD nReadLen);
	DWORD ProcessWrite(HANDLE hProcess,DWORD nAddr,DWORD nWriteLen,LPCVOID pBuffer);
	HANDLE ProcessOpen(DWORD pid,DWORD nAccess=PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION);
	BOOL ProcessClose(HANDLE hProcess);
	DWORD ProcessReadTextA(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen);	//ncMaxReadLen���ַ��������Ұ����˽������ţ�����ncMaxReadLenΪ�ַ���ʵ����󳤶�+1
	DWORD ProcessReadTextW(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen);
	DWORD ProcessReadText(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen);		//ע��ncMaxBufferLen�ǻ�����������ɵ��ַ��������صĵĶ�ȡ���ַ������ȣ����۳ɹ�����Ŀ��û�н������ź���Ҳ�ᱣ֤pBuffer����һ��NULL��β���ַ���
	CString ProcessReadText(HANDLE hProcess,DWORD nAddr,DWORD ncMaxLen);
}
#endif