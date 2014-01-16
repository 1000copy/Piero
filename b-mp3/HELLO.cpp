#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "u_win.h"
#include "u_robot.h"
#include "u_shortcut.h"
#include "u_dialog.h"
#include "u_hotkey.h"
#include "u_clipboard.h"
#include "u_mutex.h"
// #include "a_dict_edit.h"
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
BOOL dialog_test_call(HWND hwnd){
  dialog *d = new about_dlg();
  d->run(hwnd,L"Dict ",200,32,100,50,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME);
  // ::msgbox("LAIA");
  assert(d);
  delete d ;
  d = NULL;
  assert(!d);
  return TRUE;
}

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
	  dictwin(HINSTANCE h,int show):win(h,show,"bad_robot_main_win" ){
  		b = NULL;  
  	}
    dictwin():win(0,1,"bad_robot_main_win" ){
      b = NULL;  
    }
    dictwin(int x,int y,int w,int h):win(0,1,"bad_robot_main_win" ){   
      set_rect(x,y,w,h);
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

  #define ID_FILE_EXIT  1
	#define ID_FILE_QUERY  2
	#define ID_ABOUT  3	
	LRESULT on_menu(int menu_id,HWND hwnd){  	
		if (menu_id == ID_FILE_EXIT)
			PostQuitMessage(-1);
		if (menu_id == ID_FILE_QUERY)
			query();
    if (menu_id == ID_ABOUT)
      msgbox("ABOUT");
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
      // dialog_test_call(hwnd_handle);
	  	return 0;
	}

  void msgbox(LPCTSTR msg){
    dialog_test_call(hwnd_handle);
  }
	void create_menu(HWND hwnd){
		HMENU hMenu, hSubMenu;       
        hMenu = CreateMenu();
        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_QUERY, "&Query");
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_ABOUT, "&About");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&?");

        SetMenu(hwnd, hMenu);
	}
	
};

void dict_edit::on_enter()
{  
    ((dictwin*)parent_win)->query();
}


int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
  app a ;
  a.save_context(hinst, pinst,cmdline,show);  
  a.cycle(new dictwin(100,100,300,250)); 
}
