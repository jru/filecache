#include "StdAfx.h"
#include "FileCache.h"

using namespace std;

//!CFileCache implementation

CFileCache::CFileCache()
{
	InitializeCriticalSection(&m_cacheLock);
}
CFileCache::~CFileCache()
{
	DeleteCriticalSection(&m_cacheLock);
}

FileID
CFileCache::MakeFileID(HANDLE hFile)
{
	BY_HANDLE_FILE_INFORMATION fileInfo;
	if (GetFileInformationByHandle(hFile, &fileInfo) == TRUE &&
		fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY != 0)
	{
		//Generate a "unique" id for the file 
		//TODO: Consider using a 64-bit id
		return fileInfo.dwVolumeSerialNumber ^
			   fileInfo.nFileIndexHigh ^
			   fileInfo.nFileIndexLow;
	}
	else
		return 0;
}

bool
CFileCache::OpenAndInsert(LPCTSTR sPath, HANDLE hClientFile)
{
	//TODO: 
	// -Lock cache
	// -Check not used files every n seconds to see if they were deleted
	// -Special care must be taken if file is to be used asynchronously
	HANDLE hCacheFile = CreateFile(sPath, FILE_GENERIC_READ, 
								   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
								   NULL,
								   OPEN_EXISTING,
								   FILE_ATTRIBUTE_NORMAL,
								   NULL);
}

bool
CFileCache::AddFile(LPCTSTR sAbsolutePath, HANDLE hFile)
{
	FileID fID = MakeFileID(hFile);

	//Check that we got a valid file
	if (fID == 0)
	{
		return false;
	}

	//Look for the file in the cache
	FileMap::const_iterator it = m_fileMap.find(fID);
	if (it == m_fileMap.end())
	{
		//It is new, insert it
		OpenAndInsert(sAbsolutePath, hFile);
	}
}
bool
CFileCache::GetFileData(HANDLE handle, LPVOID buffer, DWORD bytesRequested, DWORD& bytesRead)
{
}
