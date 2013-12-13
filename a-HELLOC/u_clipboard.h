class clipboard{
public:
  static char* get() {
      OpenClipboard(NULL);
      HANDLE pText = GetClipboardData(CF_TEXT);
      CloseClipboard();
      LPVOID text = GlobalLock(pText);
      return (char*)text;
  }  
  static void set(const char* output)
  {    
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
  }
};