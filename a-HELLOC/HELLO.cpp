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
	ctl(win* parent,char *ctl_type,char *text ,int x,int y ,int w,int h)
	{
		assert(parent!=NULL);
		int id = 0;		
		DWORD style = WS_CHILD | WS_VISIBLE | SS_LEFT  |WS_BORDER|WS_TABSTOP ;
	 	hwnd_handle = CreateWindow(ctl_type, text,style ,
	            x,y,w,h,parent->get_handle(), (HMENU)id, NULL, NULL);	  	
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
	edit(win* parent,char *text ,int x,int y ,int w,int h):ctl(parent,"edit",text,x,y,w,h){}
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
	label(win* parent,char *text ,int x,int y ,int w,int h):ctl(parent,"static",text,x,y,w,h){}	
};

class btn:public ctl{
private:
	HWND hwnd_handle;
public:
	btn(win* parent,char *text ,int x,int y ,int w,int h):ctl(parent,"button",text,x,y,w,h){}	
};
class dictwin:public win{
private:
	edit * e;
	btn *b;
	label*l;	
public:
	dictwin(HINSTANCE hinsta, HINSTANCE pinsta,int showa):win(hinsta,pinsta,showa){
  		
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
		_log("menu_id:%d",menu_id)	;
		if (menu_id == ID_FILE_EXIT)
			PostQuitMessage(-1);
		if (menu_id == ID_FILE_QUERY)
			query();
  		return 0;
  	}
  	
  	virtual LRESULT on_command(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{  		
		int ctl_id = LOWORD(wp) ;
  		HWND ctl_hwnd = (HWND)lp ;
  		if (b && ctl_hwnd == b->get_handle()){
  			query();	
		}else{
			on_menu(LOWORD(wp),hwnd);
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
  		l = new label(this,"type word...",20, x, w, 40);
  		e = new edit(this,"some",20, x+40, w, 40);
  		b = new btn(this,"query",20, x+80, w, 40);
  		l = new label(this,"ready",20, x+120, w, 40);
  		e->set_focus();
  		// create_link(TRUE);
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

#define _UNICODE 
#include "stdio.h"

void last_error_message(DWORD error )
{
   LPVOID lpMsgBuf;
   FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM | 
         FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       error,
       // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // English language
       (LPTSTR) &lpMsgBuf,
       0,
       NULL 
   );
   MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );   
   LocalFree( lpMsgBuf );
}

int fileExists(TCHAR * file)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE handle = FindFirstFile(file, &FindFileData) ;
   int found = handle != INVALID_HANDLE_VALUE;
   if(found) 
   {
       //FindClose(&handle); this will crash
       FindClose(handle);
   }
   return found;
}

int fileExists1(TCHAR * tt){
	int r = INVALID_FILE_ATTRIBUTES == GetFileAttributes(tt) && GetLastError()==ERROR_FILE_NOT_FOUND ;
	return !r;
}
void test_file_delete(){
	char *tt = "C:\\Documents and Settings\\Administrator\\「开始」菜单\\程序\\Piero\\Piero.lnk";
	// silently_remove_directory(tt);
	_log("fileexists:%d=%d",fileExists(tt),TRUE);
	DeleteFile(tt);
	last_error_message(GetLastError());
	// GetFileAttributes(tt); // from winbase.h
	if(fileExists1(tt) ==TRUE)
	{
	    MessageBox(NULL,"","found",0);
	}else
	    MessageBox(NULL,"","NOT found",0);
	return ;
}
int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	dictwin *w = new dictwin(hinst, pinst,show);
	w->set_rect(100,100,300,250);
	app a (w);
	return a.main(hinst,cmdline,show);
}
