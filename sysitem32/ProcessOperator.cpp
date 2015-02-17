#include "StdAfx.h"

namespace ProcessOperator
{
	DWORD ProcessAlloc(HANDLE hProcess,DWORD size,DWORD nProtection)				//在目标进程里分配大小为size的一段内存
	{
		return (DWORD)::VirtualAllocEx(hProcess,0,size,MEM_RESERVE | MEM_COMMIT,nProtection);
	}

	BOOL ProcessFree(HANDLE hProcess,DWORD addr, DWORD size)
	{
		::VirtualFreeEx(hProcess,(void*)addr,size,MEM_DECOMMIT);			//解除提交
		return ::VirtualFreeEx(hProcess,(void*)addr,0,MEM_RELEASE);			//释放保留地址
	}

	DWORD ProcessRead(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD nReadLen)
	{
		if(!pBuffer)return 0;
		DWORD nRead;
		if(::ReadProcessMemory(hProcess,(LPCVOID)nAddr,pBuffer,nReadLen,&nRead))
			return nRead;
		return 0;
	}

	DWORD ProcessReadTextA(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen)	//ncMaxBufferLen是字符数，并且包含了结束符号，所以ncMaxBufferLen为字符串实际最大长度+1，返回的是读取的字符串长度(不含结束0)
	{
		if(!pBuffer)return 0;
		DWORD nSecLen=512;			//默认单次要求读取的长度
		DWORD nReadCount=0;			//已经读取的总长度
		DWORD nReadLen=nSecLen;		//单次要求读取的长度
		DWORD nRead=0;				//单次实际读取长度
		if(nReadLen>ncMaxBufferLen)nReadLen=ncMaxBufferLen;
		char* ps=(char*)pBuffer;
		char* ps1;
		while(nReadLen && ::ReadProcessMemory(hProcess,(LPCVOID)(nAddr+nReadCount),ps+nReadCount,nReadLen,&nRead))
		{
			ps1=ps+nReadCount;
			for(DWORD i=0;i<nRead;i++)	//查找以读取的数据中是否已经有字符串结尾了
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

	DWORD ProcessReadTextW(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen)	//注意ncMaxBufferLen是缓冲区最大容纳的字符数，返回的的读取的字符串长度
	{
		if(!pBuffer)return 0;
		DWORD nSecLen=512;			//默认单次要求读取的长度
		DWORD nReadCount=0;			//已经读取的总长度
		DWORD nReadLen=nSecLen;		//单次要求读取的长度
		DWORD nRead=0;				//单次实际读取长度
		if(nReadLen>ncMaxBufferLen)nReadLen=ncMaxBufferLen;
		wchar_t* ps=(wchar_t*)pBuffer;
		wchar_t* ps1;
		wchar_t* pmAddr=(wchar_t*)nAddr;
		while(nReadLen && ::ReadProcessMemory(hProcess,pmAddr+nReadCount,ps+nReadCount,nReadLen*2,&nRead))
		{
			ps1=ps+nReadCount;
			for(DWORD i=0;i<nRead/2;i++)	//查找以读取的数据中是否已经有字符串结尾了
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

	DWORD ProcessReadText(HANDLE hProcess,DWORD nAddr,LPVOID pBuffer,DWORD ncMaxBufferLen)	//注意ncMaxBufferLen是缓冲区最大容纳的字符数，返回的的读取的字符串长度，无论成功否还是目标没有结束符号函数也会保证pBuffer里是一个NULL结尾的字符串
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
