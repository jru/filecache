#pragma once

#include <string>
#include <list>
#include <set>
#include <map>

//!Cached data and meta information about a file
class FileNode
{
	HANDLE m_handle;
	DWORD  m_position;
	//TODO: Add sections of data and counters.
	//notification info, sections read, data blob, acceses, bytes read)
};

//!Invariant reference to a FileNode by a handle
struct NodeRef
{
	const HANDLE m_handle;
	FileNode&  m_fileNode;
};

//!File identification number
typedef DWORD FileID;

class CFileCache
{
public:
	CFileCache();
	~CFileCache();

	//!Whether a file was successfully added to the cache
	bool AddFile(LPCTSTR sAbsolutePath, HANDLE hFile);
	//!Whether the request was successfully processed or aborted
	bool GetFileData(HANDLE handle, LPVOID buffer, DWORD bytesRequested, DWORD& bytesRead);

private:	
	typedef std::basic_string<TCHAR> tstring;
	typedef std::set<NodeRef> ClientNodes;
	typedef std::map<DWORD, ClientNodes> ProcessMap;
	typedef std::map<FileID, FileNode> FileMap;

	static FileID MakeFileID(HANDLE hFile);
	bool OpenAndInsert(LPCTSTR sPath, HANDLE hClientFile);

	//!Map to locate client handles and associated FileNode
	ProcessMap m_processMap;
	//!File data cached
	FileMap m_fileMap;
	//!Least recently used list with links to FileNode
	std::list<NodeRef> m_lru;

	CRITICAL_SECTION m_cacheLock;
};

