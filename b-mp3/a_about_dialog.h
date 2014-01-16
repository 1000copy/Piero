class about_dlg:public dialog{
public:
  INT_PTR proc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
  { 
     switch (wm) {
     case WM_INITDIALOG: {
        create_label(L"best dicionary0)",20, 7, 150, 20,hwnd, 0);
        create_button(L"OK",20, 7+30, 100, 20,hwnd, 0);  
        return TRUE;
      }   
     }
    return FALSE;
  }
};