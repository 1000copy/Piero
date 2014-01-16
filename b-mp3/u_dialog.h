#include <windows.h>

class dialog{
public:
  virtual INT_PTR proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)=0;
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

