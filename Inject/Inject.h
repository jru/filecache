
#include <map>
#include <utility>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INJECT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INJECT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INJECT_EXPORTS
#define INJECT_API __declspec(dllexport)
#else
#define INJECT_API __declspec(dllimport)
#endif

// The singleton that represents the injection dll
class CInject {
public:
	static INJECT_API CInject& GetInstance();
	~CInject();
private:
	CInject();
	CInject(CInject const&);
	CInject& operator=(CInject const&);

	//Hooks declaration
	static HANDLE WINAPI ImpCreateFile(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
	static BOOL   WINAPI ImpReadFile(HANDLE, LPVOID, DWORD, LPOVERLAPPED, LPOVERLAPPED_COMPLETION_ROUTINE);
	static BOOL   WINAPI ImpWriteFile(HANDLE, LPCVOID, DWORD, LPOVERLAPPED, LPOVERLAPPED_COMPLETION_ROUTINE);


	bool RegisterHooks();
	bool UnRegisterHooks();
	bool AddNewHook(void* original, void* hooked);

	//typedef std::pair<void*, void*> THook;
	typedef void* Address;
	typedef std::map<Address, Address> THooks;
	THooks m_Hooks;
};

extern INJECT_API int nInject;

INJECT_API int fnInject(void);
