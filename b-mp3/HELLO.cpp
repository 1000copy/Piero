#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "u_win.h"
#include "u_robot.h"
#include "u_shortcut.h"
#include "u_dialog.h"
#include "u_clipboard.h"
#include "u_mutex.h"
#include "a_about_dialog.h"



class dict_edit:public edit{
protected:
  void on_enter();
public:
  dict_edit(win* parent,int id ,char *text ,int x,int y ,int w,int h):edit(parent,id,text,x,y,w,h){}; 
};

class dictwin:public win{
private:
	// dict_edit * e;
	btn *b1;
  btn *b2;
	label*l;	  
public:  
    void on_activate(HWND  hwnd){      
    }
	  dictwin(HINSTANCE h,int show):win(h,show,"bad_robot_main_win" ){
  		// b = NULL;  
  	}
    dictwin():win(0,1,"bad_robot_main_win" ){
      // b = NULL;  
    }
    dictwin(int x,int y,int w,int h):win(0,1,"bad_robot_main_win" ){   
      set_rect(x,y,w,h);
   }
  	~dictwin(){
        	
  	}
  	virtual LRESULT on_command(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{  		
		int ctl_id = LOWORD(wp) ;
		int ctl_type = HIWORD(wp) ;
  		HWND ctl_hwnd = (HWND)lp ;
  		if (ctl_type == BN_CLICKED) // Control 
  		{
  			if (ctl_id == 1)
  				query();	
		  }
  		return 0;
  	}
  	void query(){
  	} 
  	LRESULT on_create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{		  		
  		int x = 5;
  		int w = 200 ;
      new label(this,0,"channel",20, x, w, 40);
      x += 40;
  		new label(this,0,"music",20, x, w, 40);
      x += 40;
  		b1 = new btn(this,1,"next",20, x, w/2, 40);      
      b2 = new btn(this,2,"fav",20+w/2,x , w/2, 40);      
      return 0;
	}

  void msgbox(LPCTSTR msg){
        dialog *d = new about_dlg();
        d->run(hwnd_handle,L"Dict ",200,32,100,50,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME);
        delete d ;
        d = NULL;   
  }
	void create_menu(HWND hwnd){}	
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
