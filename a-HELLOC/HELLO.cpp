#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "u_win.cpp"
#include "u_robot.cpp"
#include "u_shortcut.h"

class dictwin:public win{
private:
	HWND hwnd_edit;
	HWND hwnd_button;
	HWND hstatic;
public:
	dictwin(HINSTANCE hinsta, HINSTANCE pinsta,int showa):win(hinsta,pinsta,showa){
  		
  	}

   LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{
  		// PAINTSTRUCT ps;
		// HDC hdc;
		// RECT rect;
		// hdc = BeginPaint(hwnd, &ps);
		// GetClientRect(hwnd, &rect);
		// DrawText(hdc, "dddHello, world.", -1, &rect,
		// 	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		// EndPaint(hwnd, &ps);
		UpdateWindow(hwnd);
		return 0;
  	}
  	LRESULT on_button(int ctl_id,HWND ctl_hwnd){  		
  		if (ctl_hwnd == hwnd_button){
  			// get edit text
			int len = GetWindowTextLengthW(hwnd_edit) + 1;         
			char text[len];
			GetWindowText(hwnd_edit, text, len);
			// set window text 
			// SetWindowText(hwnd,text);	
			// get robot
			robot r ;
			char buffer1[1000];
			int rlen = r.get_badrobot(text,buffer1,sizeof(buffer1));
			if (rlen >0) {
			  SetWindowText(hstatic, buffer1);
			}else{                    
			  wchar_t buffer[1000];
			  if(rlen ==-1)
			    wcscpy(buffer,L"要联网哦");// raw string is utf-8 ,because the source code file is utf-8 encoded file 
			  else
			    wcscpy(buffer,L"好刁钻的单词");
			  SetWindowTextW(hstatic,buffer);
			}
			SendMessage(hwnd_edit, EM_SETSEL, 0, -1);
		}
  		return 0;
  	}
  	LRESULT on_create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{		  		
  		int x = 5;
  		int w = 200;
  		create_label(L"type word...",20, x, w, 40);
  		hwnd_edit = create_edit(L"SOME",20, x+40, w, 40);
  		hwnd_button = create_button(L"query",20,x+80, w, 40);
  		hstatic = create_label(L"ready",20, x+120, w, 40);
  		SetFocus(hwnd_edit);
  		create_link(TRUE);
	  	return 0;
	}
	
};


int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	dictwin *w = new dictwin(hinst, pinst,show);
	w->set_rect(100,100,300,250);
	app a (w);
	return a.main(hinst,cmdline,show);
}
