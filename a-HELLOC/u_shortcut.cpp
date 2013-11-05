
#include <shlobj.h>
#include <objbase.h>
#include <stdio.h.>
#include <Shlwapi.h>
#include "logger.h"
// #pragma comment( lib, "Shlwapi.lib" )


/***********************************************
//pulled from msdn and debugged by Fallenhobit
//debugged as in, their code would not compile
//I got it too after alot of cleanup....and a few
//changes....and now...com the hard way
***********************************************/

//creates a shell link...in this example a Start Group item
HRESULT CreateShellLink(LPCSTR pszShortcutFile, LPSTR pszLink, LPSTR pszDesc)
{
    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
    if (SUCCEEDED(hres))
    {
       IPersistFile* ppf;

       // Query IShellLink for the IPersistFile interface for 
       // saving the shell link in persistent storage.
       hres = psl->QueryInterface(IID_IPersistFile,(void**)&ppf);
       if (SUCCEEDED(hres))
       {   
         WCHAR wsz[MAX_PATH];

         // Set the path to the shell link target.
         hres = psl->SetPath(pszShortcutFile);

         if (!SUCCEEDED(hres))
          MessageBox(NULL,"SetPath failed!","ERROR",MB_OK);

         // Set the description of the shell link.
         hres = psl->SetDescription(pszDesc);

         if (!SUCCEEDED(hres))
            MessageBox(NULL,"Set Description failed!","ERROR",MB_OK);

         // Ensure string is ANSI.
         MultiByteToWideChar(CP_ACP, 0, pszLink, -1, wsz, MAX_PATH);

         // Save the link via the IPersistFile::Save method.
         hres = ppf->Save(wsz, TRUE);
    
         // Release pointer to IPersistFile.
         ppf->Release();
       }
       // Release pointer to IShellLink.
       psl->Release();
    }
    return (hres==S_OK);
}

BOOL FileExists(LPCTSTR szPath)
{
  return PathIsDirectory(szPath);
}

// Fully qualified name of the directory being deleted, without trailing backslash

void silently_remove_directory(LPCTSTR dir) 
{
    SHFILEOPSTRUCT file_op = {
        NULL,
        FO_DELETE,
        dir,
        "",
        FOF_NOCONFIRMATION |
        FOF_NOERRORUI |
        FOF_SILENT,
        false,
        0,
        "" };
    SHFileOperation(&file_op);
}
//adds a program group to te start menu
BOOL AddNewGroup(LPSTR GroupName, char szPath [MAX_PATH],BOOL force) 
{
    LPITEMIDLIST pidlStartMenu;

    // get the pidl for the start menu
    SHGetSpecialFolderLocation( HWND_DESKTOP, CSIDL_PROGRAMS, &pidlStartMenu);

    // transform the PIDL into a path
    SHGetPathFromIDList( pidlStartMenu, szPath);
    strcat (szPath, "\\");
    strcat (szPath, GroupName);
    

     if (FileExists(szPath) && !force)
      return FALSE;
     else{
      // char buffer[1000];
      // MByteToWChar(szPath,buffer,sizeof(buffer));
      // silently_remove_directory(szPath);
      silently_remove_directory(szPath);
     }

    // if (FileExists(szPath) && !force)
    //   return FALSE;
    // else
    // if(-1== remove (szPath)){
    //   _log("delete failure:%s",szPath);
    //   return FALSE;
    // }
    // create the folder
    if(!CreateDirectory(szPath, NULL))
    {
    DWORD   dwLastError = ::GetLastError();
    TCHAR   lpBuffer[256] ;
    if(dwLastError != 0)    // Don't want to see a "operation done successfully" error ;-)
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
               NULL,                                      // No string to be formatted needed
               dwLastError,                               // Hey Windows: Please explain this error!
               MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),  // Do it in the standard language
               lpBuffer,              // Put the message here
               255,                     // Number of bytes to store the message
               NULL);
         MessageBox( HWND_TOP, lpBuffer, NULL, MB_OK);
              return FALSE ;
    }

    // notify the shell that you made a change
    SHChangeNotify( SHCNE_MKDIR, SHCNF_FLUSH | SHCNF_PATH, szPath, 0);
}



/* file */

int file_exists(TCHAR * file)
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

void create_link(BOOL force){
  _log("entry:link");
  char szText [MAX_PATH];
  //call this because we are using COM..initializes com
  CoInitialize(NULL);
  if(!AddNewGroup("Piero", szText,force))//create the demo program group
  {    
    return;
  }
  
  strcat(szText, "\\Piero.lnk");
  if (FileExists(szText))
    return ;
  char filename[255] ;
  GetModuleFileName(0,filename,sizeof(filename));
  if (!CreateShellLink(filename, szText, ""))//create program link
  {
    MessageBox(NULL,"Error Creating Program Link","ERROR",MB_OK);
    return;
  }
  //the companion to CoInitialize()
  CoUninitialize();
}