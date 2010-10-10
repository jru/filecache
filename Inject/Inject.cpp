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

// This is the constructor of a class that has been exported.
// see Inject.h for the class definition
CInject::CInject()
{
	DetourTransactionBegin();
	DetourTransactionAbort();
	return;
}
