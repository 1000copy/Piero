#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "u_win.cpp"
#include "u_robot.h"
#include "u_shortcut.h"

class ctl{
protected:
	HWND hwnd_handle;
public:
	ctl(win* parent,int id,char *ctl_type,char *text ,int x,int y ,int w,int h)
	{
		assert(parent!=NULL);
		HWND handle = parent->get_handle();
		assert(handle !=0);	 	
		DWORD style = WS_CHILD | WS_VISIBLE | SS_LEFT  |WS_BORDER|WS_TABSTOP ;
		hwnd_handle = CreateWindow(ctl_type, text,style ,
	            x,y,w,h,handle, (HMENU)id, NULL, NULL);	  	
	}
	HWND get_handle(){return hwnd_handle;}
	void get_text(char *buf,int size){		
		int len = GetWindowTextLength(hwnd_handle) + 1;         
		assert(size>=len);
		// char text[len];
		GetWindowText(hwnd_handle, buf, size);
	}
	void set_text(char *buf){
		SetWindowText(hwnd_handle, buf);
	}
	int get_len(){		
		return GetWindowTextLength(hwnd_handle) ;		
	}
	
};
class edit:public ctl{
private:	
public:
	edit(win* parent,int id ,char *text ,int x,int y ,int w,int h):ctl(parent,id,"edit",text,x,y,w,h){}
	void select(int from ,int to){
		SendMessage(hwnd_handle, EM_SETSEL, from, to);
	}
	void set_focus(){
		SetFocus(hwnd_handle);
	}		
};

class label:public ctl{
private:
	HWND hwnd_handle;
public:
	label(win* parent,int id,char *text ,int x,int y ,int w,int h):ctl(parent,id,"static",text,x,y,w,h){}	
};

class btn:public ctl{
private:
	HWND hwnd_handle;
public:
	btn(win* parent,int id,char *text ,int x,int y ,int w,int h):ctl(parent,id,"button",text,x,y,w,h){}	
};
class dictwin:public win{
private:
	edit * e;
	btn *b;
	label*l;	
public:
	dictwin(HINSTANCE hinsta, HINSTANCE pinsta,int showa):win(hinsta,pinsta,showa){
  		b = NULL;
  	}
  	~dictwin(){
  		delete e;
  	}
   LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{
  		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, "dddHello, world.", -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
  		//   if (b!=NULL)
  		//   	_log("b->get_handle():%d,ctl_hwnd:%d",b->get_handle(),ctl_hwnd);
  		//   if ((b!=NULL) && (ctl_hwnd == b->get_handle())) {
  		// 	query();	
		  // }  		
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
  		l = new label(this,0,"type word...",20, x, w, 40);
  		e = new edit(this,0,"some",20, x+40, w, 40);
  		b = new btn(this,ID_FILE_QUERY,"query",20, x+80, w, 40);
  		l = new label(this,0,"ready",20, x+120, w, 40);
  		e->set_focus();
  		//create_link(TRUE);
  		create_menu(hwnd);
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


int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	dictwin *w = new dictwin(hinst, pinst,show);
	w->set_rect(100,100,300,250);
	app a (w);
	return a.main(hinst,cmdline,show);
}
