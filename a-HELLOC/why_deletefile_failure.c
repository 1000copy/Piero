#define _UNICODE 
#include <stdio.h>
#include <windows.h>
#include "logger.h"
void last_error_message(DWORD error )
{
   LPVOID lpMsgBuf;
   FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM | 
         FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       error,
       // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // English language
       (LPTSTR) &lpMsgBuf,
       0,
       NULL 
   );
   MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );   
   LocalFree( lpMsgBuf );
}

int fileExists(TCHAR * file)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE handle = FindFirstFile(file, &FindFileData) ;
   int found = handle != INVALID_HANDLE_VALUE;
   if(found) 
   {
       //FindClose(&handle); this will crash
       FindClose(handle);
   }
   return found;
}

int fileExists1(TCHAR * tt){
	int r = INVALID_FILE_ATTRIBUTES == GetFileAttributes(tt) && GetLastError()==ERROR_FILE_NOT_FOUND ;
	return !r;
}
void test_file_delete(){
	char *tt = "C:\\Documents and Settings\\Administrator\\「开始」菜单\\程序\\Piero\\Piero.lnk";
	// silently_remove_directory(tt);
	_log("fileexists:%d=%d",fileExists(tt),TRUE);
	DeleteFile(tt);
	last_error_message(GetLastError());
	// GetFileAttributes(tt); // from winbase.h
	if(fileExists1(tt) ==TRUE)
	{
	    MessageBox(NULL,"","found",0);
	}else
	    MessageBox(NULL,"","NOT found",0);
	return ;
}
int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
  test_file_delete();
	return 0;
}