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
 switch (wm) {
 case WM_INITDIALOG: {
  create_label(L"准备...",20, 7, 350, 20,hwnd, 0);
  create_button(L"OK...",20, 7+20, 350, 20,hwnd, 0);  
  return TRUE;
}
 case WM_COMMAND:
  // if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) EndDialog(hwnd, 0);
  EndDialog(hwnd, 0);
  break;
 }
 return FALSE;
}

BOOL dialog_modal(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,DLGPROC proc);
BOOL dialog_test_call(HWND hwnd){
  return dialog_modal(hwnd,L"Title ",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME,DlgProc); 
}