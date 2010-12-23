// filecache.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Inject.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CInject::GetInstance();
	CreateFile(L"README", FILE_READ_ACCESS, 0, NULL, OPEN_ALWAYS, 0, NULL);
	CreateFile(L"asdf", FILE_READ_ACCESS, 0, NULL, OPEN_ALWAYS, 0, NULL);
	return 0;
}

