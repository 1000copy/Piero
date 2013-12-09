#include <windows.h>

class dialog{
public:
  virtual INT_PTR  proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)=0;

};

class some_dialog{
public:
  INT_PTR proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam);

};

HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  return CreateWindowW(L"button", text, WS_CHILD | WS_VISIBLE | SS_LEFT|WS_TABSTOP ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
}

HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  HWND hwndedit = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
  // set_font(hwndedit,8);
  return hwndedit;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{ 
//  switch (wm) {
//  case WM_INITDIALOG: {
//   create_label(L"准备...",20, 7, 350, 20,hwnd, 0);
//   create_button(L"OK...",20, 7+20, 350, 20,hwnd, 0);  
//   return TRUE;
// }
//  case WM_COMMAND:
//   // if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) EndDialog(hwnd, 0);
//   EndDialog(hwnd, 0);
//   break;
//  }
//  return FALSE;
  some_dialog* d = (some_dialog*)lParam;
  // if ((wm==WM_COMMAND) &&  (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL)) {
  if ((wm==WM_COMMAND) ) {
    EndDialog(hwnd, 0);
    delete d ;
    return FALSE;
  }else  
    return  d->proc(hwnd,wm,wParam,lParam);
}

BOOL dialog_modal(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,DLGPROC proc,LPARAM lp);

INT_PTR some_dialog::proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{ 
   switch (wm) {
   case WM_INITDIALOG: {
      create_label(L"准备...",20, 7, 350, 20,hwnd, 0);
      create_button(L"OK...",20, 7+20, 350, 20,hwnd, 0);  
      return TRUE;
    }   
   }
  return FALSE;
}
 
BOOL dialog_test_call(HWND hwnd){
  some_dialog *d = new some_dialog();
  return dialog_modal(hwnd,L"Title ",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME,DlgProc,(LPARAM)d); 
}
