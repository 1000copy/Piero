class mutex{
  HANDLE hMutex ; 
public:
    mutex(){

    }
    ~mutex(){
      CloseHandle(hMutex) ;
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
  void restore_last(char* windowName)
  {
    HWND firstInstance = FindWindow(windowName,NULL);
    ShowWindow(firstInstance, 1);
    SetForegroundWindow(firstInstance);
  }
};