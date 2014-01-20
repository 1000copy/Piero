
#include <windows.h>
#include "dialog_common.h"


#include <windows.h> 

#include <shlobj.h>
#include <objbase.h>
#include <Shlwapi.h>
#include <objidl.h>
//
//

HWND get_rootwindow(){
  HWND w =GetActiveWindow();
  HWND last ;
  while(w !=NULL){
    last = w;
    w= GetParent(last);
  }
  return last ;
}

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc){
  WNDCLASS wc; 
  HWND hwnd; 
  MSG msg; 

  wc.style=CS_HREDRAW | CS_VREDRAW|WS_EX_TOOLWINDOW; 
  wc.lpfnWndProc=WindowProc; 
  wc.cbClsExtra=0; 
  wc.cbWndExtra=0; 
  wc.hInstance=hInst; 
  wc.hIcon=LoadIcon(NULL,IDI_WINLOGO); 
  wc.hCursor=LoadCursor(NULL,IDC_ARROW); 
  // wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME; 
  wc.hbrBackground=(HBRUSH)GetSysColorBrush(COLOR_3DFACE);
  wc.lpszMenuName=NULL; 
  wc.lpszClassName=AppTitle; 
  if (!RegisterClass(&wc)) 
    return 0 ;else return 1;
}
HWND  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow,int x,int y,int w,int h){
  // DWORD style = WS_OVERLAPPEDWINDOW ;
  DWORD style = WS_OVERLAPPED  | WS_SYSMENU | WS_THICKFRAME | WS_EX_TOOLWINDOW ;
  // HWND hwnd = CreateWindow(AppTitle,AppTitle, style, x,y,w,h,NULL,NULL,hInst,NULL); 
  HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW,AppTitle,AppTitle, style, x,y,w,h,NULL,NULL,hInst,NULL); 
  // ::SendMessage(hwnd, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

  ShowWindow(hwnd,nCmdShow); 
  UpdateWindow(hwnd); 
  return hwnd;
}
void loop(HWND hwnd){    
    MSG msg; 
    while (GetMessage(&msg,NULL,0,0) > 0) 
    { 
      if(!IsDialogMessage(hwnd,&msg)){
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
      }
    } 
}
void set_font(HWND hwnd,int height){
    HFONT hf;
    HDC hdc;
    long lfHeight;
    
    hdc = GetDC(NULL);
    lfHeight = -MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), 40);
    ReleaseDC(NULL, hdc);    
    hf = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
    SendMessage (hwnd, WM_SETFONT, WPARAM (hf), TRUE);
}
HWND create_edit(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
    HWND hwndedit = CreateWindowW(L"EDIT", text, WS_CHILD | WS_VISIBLE | SS_LEFT |WS_BORDER|WS_TABSTOP  ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
    set_font(hwndedit,12);
    return hwndedit;
}

HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  return CreateWindowW(L"button", text, WS_CHILD | WS_VISIBLE | SS_LEFT|WS_TABSTOP ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
}

HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  HWND hwndedit = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
  set_font(hwndedit,8);
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


const int id_about = 1 ;
void on_create(HWND  hwnd){    
    create_button(L"?",0, 0, 50, 25,hwnd, id_about);    
}
void on_about(HWND hwnd){
  // hwnd 不传入的话，是不会模态的！
   dialog_modal(hwnd,L"Title ",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME,DlgProc); 
}
void on_click(int id,HWND hwnd){      
    if(id==id_about){
       on_about(hwnd);
    }else{
       PostQuitMessage(0);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
  // _log("msg:%d", msg);
  switch (msg) 
  { 
    case WM_CREATE:
    {      
        on_create(hwnd);        
        break;
    }
    case WM_DESTROY: 
    {
      PostQuitMessage(0);
      break; 
    }
    case WM_COMMAND:
          if (HIWORD(wparam) == BN_CLICKED) {
             on_click(LOWORD(wparam),hwnd);  
          }
          break;
    default: 
      return DefWindowProc(hwnd, msg, wparam, lparam); 
  } 
  return 0; 
} 

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
  char *title="Dictionary"; 
  if (0==create_class(hInst,title,WindowProc))
    return 0;  
  HWND hwnd = create_win(NULL,title,nCmdShow,CW_USEDEFAULT,CW_USEDEFAULT,400,180);  
  loop(hwnd); 
} 
