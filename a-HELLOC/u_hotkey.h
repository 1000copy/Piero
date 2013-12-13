#define HOTKEY_ID  100
class hotkey{
private:
   BOOL is_fail ;
   HWND hwnd;
public:
  hotkey(HWND hwnd1){
    hwnd = hwnd1;
    is_fail = FALSE;
    if (0==RegisterHotKey(hwnd, HOTKEY_ID,MOD_CONTROL, 0X4D)){
        UnregisterHotKey(hwnd,HOTKEY_ID);
        _log("RegisterHotKey failure!");
        is_fail = TRUE;
        return;
    }
  }
  ~hotkey(){   
   if(!is_fail)
      UnregisterHotKey(hwnd,HOTKEY_ID);    
  }
};

