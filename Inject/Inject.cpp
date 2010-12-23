// Inject.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Inject.h"


// This is an example of an exported variable
INJECT_API int nInject=0;

// This is an example of an exported function.
INJECT_API int fnInject(void)
{
	return 42;
}

CInject&
CInject::GetInstance()
{
	static CInject instance;
	return instance;
}

// This is the constructor of a class that has been exported.
// see Inject.h for the class definition
CInject::CInject()
{
	printf("Registering detours...");
	fflush(stdout);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	RegisterHooks();

	LONG error = DetourTransactionCommit();
	if (error == NO_ERROR)
	{
		printf("OK!\n");
	}
	else {
		printf("Failed with code: %d\n Forcing exit in 5 seconds", error);
		Sleep(5000);
		//TODO: Throw an exception
		//throw std::run
	}
}

CInject::~CInject()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	UnRegisterHooks();

	DetourTransactionCommit();
}


template <class T,class U>
T typeof(T t, U t2)
{
	return static_cast<T>(t2);
}

//
//Hooks implementation
//
HANDLE
WINAPI
CInject::ImpCreateFile(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    )
{
	//TODO: 
	// - Pass current path, filename, flags and handle to CFileCache 
	// - If it is good to be cached it will be added to the tree of opened files (use GetFileType)
	// - Need to hook the file pointer API's to keep 

	//Get full path GetFullPathName
	TCHAR FullName[1024];
    GetFullPathName(lpFileName,sizeof(FullName)/sizeof(TCHAR), FullName, NULL);
	//Getting the Universal name of the resource, if physical drive returns ERROR_NOT_CONNECTED
	TCHAR Buffer[1024];
	DWORD dwBufferLength = sizeof(Buffer);
    WNetGetUniversalName(FullName, UNIVERSAL_NAME_INFO_LEVEL, (LPVOID) &Buffer, &dwBufferLength );

	Address pReal = GetInstance().m_Hooks.find(ImpCreateFile)->second;
	wprintf(L"Opening: %s...", lpFileName);
	HANDLE ret = typeof(ImpCreateFile, pReal)(lpFileName, dwDesiredAccess, dwShareMode, 
						lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	wprintf(L"%d\n", ret);
	return ret;
}
BOOL
WINAPI
CInject::ImpReadFile(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPOVERLAPPED lpOverlapped,
    LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    )
{
	return FALSE;
}
BOOL
WINAPI
CInject::ImpWriteFile(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPOVERLAPPED lpOverlapped,
    LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    )
{
	return FALSE;
}

bool
CInject::AddNewHook(void* original, void* hooked)
{
	std::pair<THooks::iterator, bool> res = m_Hooks.insert(std::make_pair(hooked, original));
	bool ret = DetourAttach(&res.first->second, hooked) == NO_ERROR;
	return ret;
}

bool
CInject::RegisterHooks()
{
	return 
	AddNewHook(::CreateFileW, CInject::ImpCreateFile) &&
	AddNewHook(::ReadFile, CInject::ImpReadFile) &&
	AddNewHook(::WriteFile, CInject::ImpWriteFile);
}

bool
CInject::UnRegisterHooks()
{
	bool ret = true;
	for (THooks::iterator it = m_Hooks.begin();
		it != m_Hooks.end();
		++it)
	{
		ret &= (DetourDetach(&it->second, it->first) == NO_ERROR); 
	}
	return ret;
}

