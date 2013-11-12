// g++ -mwindows mutex.c -o mutex.exe -static
#include <windows.h>

class mutex{
	HANDLE hMutex ;	
public:
    mutex(){

    }
    ~mutex(){
    	CloseHandle(hMutex)	;
    }
	BOOL check(){
		hMutex = CreateMutex(NULL, TRUE, "Global\\DFFDFD-C1733E55-A6FD-47D5-8638-053E938E08B8");
	    if ( hMutex )
	    {
	       if( GetLastError() == ERROR_ALREADY_EXISTS )
	       {
	          CloseHandle( hMutex );
	          return TRUE;
	       }
	    } 	    
	    return FALSE;
	}
};
int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	mutex t;
	if (t.check()){
		MessageBox(NULL,"","exists and exit",0);
		return 0;
	}else
		MessageBox(NULL,"","ready",0);
	// CloseHandle( hMutex );
}
