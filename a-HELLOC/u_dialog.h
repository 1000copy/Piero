#include <windows.h>

class dialog{
public:
  virtual INT_PTR proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam);
  BOOL run(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE);
  HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
    return CreateWindowW(L"button", text, WS_CHILD | WS_VISIBLE | SS_LEFT|WS_TABSTOP ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
  }

  HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
    HWND hwndedit = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
              x,y,w,h,hwnd, (HMENU) id, NULL, NULL);    
    return hwndedit;
  }

};
dialog* d ;
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{ 
  if (wm == WM_INITDIALOG)
    d = (dialog*)lParam; 
  // if ((wm==WM_COMMAND) &&  (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL)) {
  
  if ((wm==WM_COMMAND) ) {
    EndDialog(hwnd, 0);
    delete d ;
    d = NULL;
    return FALSE;
  }else if(d)  {
    return d->proc(hwnd,wm,wParam,lParam);
  }
}



INT_PTR dialog::proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{ 
   switch (wm) {
   case WM_INITDIALOG: {
      // create_label(L"best dicionary",20, 7, 100, 20,hwnd, 0);
      // create_button(L"OK...",20, 7+20, 100, 20,hwnd, 0);  
      return TRUE;
    }   
   }
  return FALSE;
}
 
class about_dlg:public dialog{
public:
  INT_PTR proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
  { 
     switch (wm) {
     case WM_INITDIALOG: {
        create_label(L"best dicionary",20, 7, 100, 20,hwnd, 0);
        create_button(L"OK",20, 7+20, 100, 20,hwnd, 0);  
        return TRUE;
      }   
     }
    return FALSE;
  }
};
BOOL dialog_test_call(HWND hwnd){
  dialog *d = new about_dlg();
  return d->run(hwnd,L"Title ",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME);
}
BOOL dialog_modal(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,DLGPROC proc,LPARAM lp);
BOOL dialog::run(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE)
{
  assert(this);
  return dialog_modal(hwnd,pszTitle,X,Y,W,H,STYLE,DlgProc,(LPARAM)this);
}

