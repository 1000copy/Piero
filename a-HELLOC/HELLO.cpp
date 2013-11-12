#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "u_win.h"
#include "u_robot.h"
#include "u_shortcut.h"
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
class dict_edit:public edit{
protected:
  void on_enter();
public:
  dict_edit(win* parent,int id ,char *text ,int x,int y ,int w,int h):edit(parent,id,text,x,y,w,h){}; 
};

class dictwin:public win{
private:
	dict_edit * e;
	btn *b;
	label*l;	
  hotkey *key;
public:
  BOOL is_change(char* new_,HWND hwndedit){
    int len = GetWindowTextLengthW(hwndedit) + 1;         
    char text[len];
    memset(text,0,len);
    GetWindowText(hwndedit, text, len);
    // _log("is_change:1");
    BOOL r = 0!=strcmp(new_,text); 
    // _log("is_change:2");
    return r;
  }
 
  void on_activate(HWND  hwnd){  
    e->set_focus();
    char* content = clipboard::get();
    HWND hwnd1 = get_rootwindow();
    HWND hwndedit = e->get_handle();        
    if(content && is_change(content,hwndedit)){            
      SetWindowText(hwndedit,content);
      query();
    }
  }
	dictwin(HINSTANCE h,int show,char *class_name):win(h,show,class_name){
  		b = NULL;  
  	}
  	~dictwin(){
      delete key;
  		delete e;
  	}
    void on_hotkey(HWND hwnd,int key)
    {
      if (key != HOTKEY_ID)return;      
      if (hwnd != GetActiveWindow()){
        ShowWindow(hwnd,SW_SHOW);
        SetForegroundWindow(hwnd);
      }
      else{
        char buf[1024];
        l->get_text(buf,sizeof(buf));
        clipboard::set(buf);
        ShowWindow(hwnd,SW_HIDE);   
      }            
   }
  LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  {
      PAINTSTRUCT ps;
      HDC hdc;
      RECT rect;
      hdc = BeginPaint(hwnd, &ps);
      // GetClientRect(hwnd, &rect);
      // DrawText(hdc, "dddHello, world.", -1, &rect,
      // 	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
      UpdateWindow(hwnd);
      EndPaint(hwnd, &ps);		
      return 0;
  }
  #define ID_FILE_EXIT  1
	#define ID_FILE_QUERY  2
	#define ID_STUFF_GO  3	
	LRESULT on_menu(int menu_id,HWND hwnd){  	
		if (menu_id == ID_FILE_EXIT)
			PostQuitMessage(-1);
		if (menu_id == ID_FILE_QUERY)
			query();
  		return 0;
  	}
  	
  	virtual LRESULT on_command(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{  		
		int ctl_id = LOWORD(wp) ;
		int ctl_type = HIWORD(wp) ;
  		HWND ctl_hwnd = (HWND)lp ;
  		if (ctl_type ==  0) //menu 
  			on_menu(LOWORD(wp),hwnd);
  		else if (ctl_type ==  1) //Accelerator
  			;
  		else if (ctl_type == BN_CLICKED) // Control 
  		{
  			if (ctl_id == ID_FILE_QUERY)
  				query();	
		}
  		return 0;
  	}
  	void query(){
  		char text[e->get_len()+1];
  		assert(e);
  		e->get_text(text,sizeof(text));			
  		// _log("text:%s",text);
  		
  		char buf[1000];
  		get_robot(text,buf,sizeof(buf));
  		l->set_text(buf);
  		// SetWindowText(hstatic,buf);
  		e->select(0,-1);
  		e->set_focus();		
  	} 
  	LRESULT on_create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{		  		
  		int x = 5;
  		int w = 200;
  		new label(this,0,"type word...",20, x, w, 40);
  		e = new dict_edit(this,0,"some",20, x+40, w, 40);
  		b = new btn(this,ID_FILE_QUERY,"query",20, x+80, w, 40);
  		l = new label(this,0,"ready",20, x+120, w, 40);
      assert(e);
      assert(b);
      assert(l);
  		e->set_focus();
  		//create_link(TRUE);
  		create_menu(hwnd);
      key = new hotkey(hwnd);
	  	return 0;
	}
	void msgbox(LPCTSTR msg){
  		_log("hwnd_handle:%d",hwnd_handle)	;
  		MessageBox(
	        NULL,
	        (LPCTSTR)msg,
	        (LPCTSTR)L"tip",
	        MB_OK
	    );
  	}
	void create_menu(HWND hwnd){
		HMENU hMenu, hSubMenu;       
        hMenu = CreateMenu();
        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_QUERY, "&Query");
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&About");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&?");

        SetMenu(hwnd, hMenu);
	}
	
};

void dict_edit::on_enter()
{  
    ((dictwin*)parent_win)->query();
}


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
};

void ShowToFront(char* windowName)
{
  HWND firstInstance = FindWindow(windowName,NULL);
  ShowWindow(firstInstance, 1);
  SetForegroundWindow(firstInstance);
}

int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
  char * window_class_name = "HELLOWIN" ;
  mutex x ;
  if (x.check()){
    //restore old windows
    ShowToFront(window_class_name);
    return 0;
  }
	dictwin *w = new dictwin(hinst, show,window_class_name);
	w->set_rect(100,100,300,250);
	app a (w);
	return a.main(hinst,cmdline,show);
}
