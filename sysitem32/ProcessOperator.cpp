#include "StdAfx.h"

namespace ProcessOperator
{
	DWORD ProcessAlloc(HANDLE hProcess,DWORD size,DWORD nProtection)				//��Ŀ�����������СΪsize��һ���ڴ�
	{
		return (DWORD)::VirtualAllocEx(hProcess,0,size,MEM_RESERVE | MEM_COMMIT,nProtection);
	}

	BOOL ProcessFree(HANDLE hProcess,DWORD addr, DWORD size)
	{
		::VirtualFreeEx(hProcess,(void*)addr,size,MEM_DECOMMIT);			//����ύ
		return ::VirtualFreeEx(hProcess,(void*)addr,0,MEM_RELEASE);			//�ͷű�����ַ
	}

	DWORD ProcessRead(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD nReadLen)
	{
		if(!pBuffer)return 0;
		DWORD nRead;
		if(::ReadProcessMemory(hProcess,(LPCVOID)nAddr,pBuffer,nReadLen,&nRead))
			return nRead;
		return 0;
	}

	DWORD ProcessReadTextA(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen)	//ncMaxBufferLen���ַ��������Ұ����˽������ţ�����ncMaxBufferLenΪ�ַ���ʵ����󳤶�+1�����ص��Ƕ�ȡ���ַ�������(��������0)
	{
		if(!pBuffer)return 0;
		DWORD nSecLen=512;			//Ĭ�ϵ���Ҫ���ȡ�ĳ���
		DWORD nReadCount=0;			//�Ѿ���ȡ���ܳ���
		DWORD nReadLen=nSecLen;		//����Ҫ���ȡ�ĳ���
		DWORD nRead=0;				//����ʵ�ʶ�ȡ����
		if(nReadLen>ncMaxBufferLen)nReadLen=ncMaxBufferLen;
		char* ps=(char*)pBuffer;
		char* ps1;
		while(nReadLen && ::ReadProcessMemory(hProcess,(LPCVOID)(nAddr+nReadCount),ps+nReadCount,nReadLen,&nRead))
		{
			ps1=ps+nReadCount;
			for(DWORD i=0;i<nRead;i++)	//�����Զ�ȡ���������Ƿ��Ѿ����ַ�����β��
			{
				if(ps1[i]==0)
				{
					nRead=i;
					break;
				}
			}
			nReadCount+=nRead;
			if(nRead!=nReadLen)break;
			nReadLen=ncMaxBufferLen-nReadCount;
			if(nReadLen>nSecLen)nReadLen=nSecLen;
		}
		if(nReadCount>=ncMaxBufferLen)nReadCount=ncMaxBufferLen-1;
		((char*)pBuffer)[nReadCount]=0;
		return nReadCount;
	}

	DWORD ProcessReadTextW(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen)	//ע��ncMaxBufferLen�ǻ�����������ɵ��ַ��������صĵĶ�ȡ���ַ�������
	{
		if(!pBuffer)return 0;
		DWORD nSecLen=512;			//Ĭ�ϵ���Ҫ���ȡ�ĳ���
		DWORD nReadCount=0;			//�Ѿ���ȡ���ܳ���
		DWORD nReadLen=nSecLen;		//����Ҫ���ȡ�ĳ���
		DWORD nRead=0;				//����ʵ�ʶ�ȡ����
		if(nReadLen>ncMaxBufferLen)nReadLen=ncMaxBufferLen;
		wchar_t* ps=(wchar_t*)pBuffer;
		wchar_t* ps1;
		wchar_t* pmAddr=(wchar_t*)nAddr;
		while(nReadLen && ::ReadProcessMemory(hProcess,pmAddr+nReadCount,ps+nReadCount,nReadLen*2,&nRead))
		{
			ps1=ps+nReadCount;
			for(DWORD i=0;i<nRead/2;i++)	//�����Զ�ȡ���������Ƿ��Ѿ����ַ�����β��
			{
				if(ps1[i]==0)
				{
					nRead=i*2;
					break;
				}
			}
			nReadCount+=nRead/2;
			if(nRead!=nReadLen*2)break;
			nReadLen=ncMaxBufferLen-nReadCount;
			if(nReadLen>nSecLen)nReadLen=nSecLen;
		}
		if(nReadCount>=ncMaxBufferLen)nReadCount=ncMaxBufferLen-1;
		((wchar_t*)pBuffer)[nReadCount]=0;
		return nReadCount;
	}

	DWORD ProcessReadText(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen)	//ע��ncMaxBufferLen�ǻ�����������ɵ��ַ��������صĵĶ�ȡ���ַ������ȣ����۳ɹ�����Ŀ��û�н������ź���Ҳ�ᱣ֤pBuffer����һ��NULL��β���ַ���
	{
		if(sizeof(TCHAR)==2)
			return ProcessReadTextW(hProcess,nAddr,pBuffer,ncMaxBufferLen);
		else
			return ProcessReadTextA(hProcess,nAddr,pBuffer,ncMaxBufferLen);
	}

	CString ProcessReadText(HANDLE hProcess,DWORD nAddr,DWORD ncMaxLen)
	{
		if(!nAddr || !ncMaxLen)return _T("");
		TCHAR* pBuffer=new TCHAR[ncMaxLen+1];
		if(!pBuffer)return _T("");
		ProcessReadText(hProcess,nAddr,pBuffer,ncMaxLen+1);
		CString sText=pBuffer;
		delete pBuffer;
		return sText;
	}

	DWORD ProcessWrite(HANDLE hProcess,DWORD nAddr,DWORD nWriteLen,LPCVOID pBuffer)
	{
		if(!pBuffer)return 0;
		DWORD nWritten;
		if(::WriteProcessMemory(hProcess,(LPVOID)nAddr,pBuffer,nWriteLen,&nWritten))
			return nWritten;
		return 0;
	}

	HANDLE ProcessOpen(DWORD pid,DWORD nAccess)
	{
		return ::OpenProcess(nAccess,0,pid);
	}

	BOOL ProcessClose(HANDLE hProcess)
	{
		return ::CloseHandle(hProcess);
	}
}
