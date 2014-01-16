#include <windows.h>
#include <assert.h>
#include "logger.h"

void msgbox(LPCTSTR msg)
{
  MessageBox(NULL, (LPCTSTR)msg,(LPCTSTR)L"tip",MB_OK);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
class win{
	WNDPROC winproc;
	HINSTANCE hinst;
	char *class_name;
	int show;
	int x,y,w,h;
	
  protected:
  	HWND hwnd_handle;
  	// virtual LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)=0;
  	virtual LRESULT on_button(int ctl_id,HWND ctl_hwnd){  		
  		return 0;
  	}
  	virtual LRESULT on_menu(int menu_id,HWND hwnd){  		
  		return 0;
  	}
  	virtual LRESULT on_create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
  		// _log("base into ");
  		return 0;
  	}
  	virtual LRESULT on_command(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){return 0;};  	
  	virtual void on_hotkey(HWND hwnd,int key){
  		return ;
	}
	virtual void on_activate(HWND  hwnd){  
		return ;
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
  public:
  	HWND get_handle(){return hwnd_handle;}
	HWND get_rootwindow(){
	  HWND w =GetActiveWindow();
	  HWND last ;
	  while(w !=NULL){
	    last = w;
	    w= GetParent(last);
	  }
	  return last ;
	}
  	win(HINSTANCE hinsta,int showa,char *class_name_a){
  		winproc = WndProc ;
  		hinst = hinsta ;
  		show = showa;
  		class_name = class_name_a;
  	}
  	win(){  	
  	}
  	win(int x,int y,int w,int h){  	
  		set_rect(x,y,w,h);
  	}
  	void set_rect(int x_,int y_,int w_,int h_){
  		x = x_;
  		y = y_;
  		w = w_;
  		h = h_;
  	}  	
  	LRESULT CALLBACK _proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
		switch(msg) {
			case WM_HOTKEY:
		    {
		      on_hotkey(hwnd,LOWORD(wp));
		      break;
		    }
		    case WM_ACTIVATE:
		    {
		      on_activate(hwnd);      
		      // 没有break ，死相很惨
		      break;
		    }
			case WM_CREATE:
			{
				hwnd_handle = hwnd;
				on_create(hwnd,msg,wp,lp);			
				break;
			}
			case WM_PAINT:
			{
				on_paint(hwnd,msg,wp,lp);
				break;					
			}
			case WM_COMMAND:
			{				
				on_command(hwnd,msg,wp,lp);				
				break;
			}
			case WM_DESTROY:
			{
				// _log("enter keyup");
				PostQuitMessage(0);
				break;
			}
			default :
				return DefWindowProc(hwnd, msg, wp, lp);
		}
		return 0;
  	}
  	void main(){  		
  		// if (pinst==NULL) 
		{
			WNDCLASS wndcls;

			wndcls.style = CS_HREDRAW | CS_VREDRAW;
			wndcls.lpfnWndProc = winproc;
			wndcls.cbClsExtra = 0;
			wndcls.cbWndExtra = 0;
			wndcls.hInstance = hinst;
			wndcls.hIcon = LoadIcon(hinst, "appicon");
			wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wndcls.lpszMenuName = "HELLOMENU";
			wndcls.lpszClassName = class_name ;

			RegisterClass(&wndcls);
		}
		HWND hwnd = CreateWindow(class_name,		 /* class name */
			class_name,				 /* title */
			WS_OVERLAPPEDWINDOW,					 /* window style */
			x,y,w,h,			
			NULL,										 /* parent */
			NULL,										 /* menu */
			hinst,									 /* module instance */
			this);									 /* create param */
		
		ShowWindow(hwnd, show);
		UpdateWindow(hwnd);
  	}
};
class app{
	int loop(HWND hwnd){
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			if(!IsDialogMessage(hwnd,&msg)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return msg.wParam;
	}
	win * w;
	public:
	app(win *a){
		w = a;
	}
	app(){	
	}
	void set_win(win *a){
		w = a;	
	}
	~app(){
		if (NULL!=w)
			delete w;
	}
	void save_context(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show){

	}
	int PASCAL main()
	{		
		
		assert(w);		
		w->main();
		return loop(w->get_handle());
	}
	int cycle(win *w1,int x,int y,int w,int h)
	{
	  // mutex x1 ;
	  // if (x1.check()){
	  //   //restore old windows
	  //   x1.restore_last(window_class_name);
	  //   return 0;
	  // }	  
	  w1->set_rect(x,y,w,h);	  
	  set_win(w1);
	  return main();
	}
	int cycle(win *w1)
	{
	  // w1->set_rect(x,y,w,h);	  
	  set_win(w1);
	  return main();
	}
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{

	if (msg == WM_CREATE){
		// _log("mount this with hwnd:%d",hwnd);
		LONG this_ = (LONG) (((LPCREATESTRUCT) lp)->lpCreateParams);
		SetWindowLong(hwnd,GWL_USERDATA,this_);
	}
	long v = GetWindowLong(hwnd,GWL_USERDATA);
	win *p = (win*)v;
	return p->_proc(hwnd,msg,wp,lp);
}

class ctl{
protected:
	HWND hwnd_handle;
	win* parent_win;
public:
	ctl(win* parent,int id,char *ctl_type,char *text ,int x,int y ,int w,int h)
	{
		assert(parent!=NULL);
		parent_win = parent ;
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
// edit control 

WNDPROC DefEditProc ;
LRESULT CALLBACK MyEditProc(HWND h_edit, UINT message,WPARAM wParam, LPARAM lParam) ;
class edit:public ctl{
public:
	edit(win* parent,int id ,char *text ,int x,int y ,int w,int h):ctl(parent,id,"edit",text,x,y,w,h)
	{
		assert(hwnd_handle);
		// _log("hwnd_handle:%d",hwnd_handle);
		DefEditProc = (WNDPROC)GetWindowLong(hwnd_handle,GWL_WNDPROC);		
		assert(DefEditProc);
		SetWindowLong(hwnd_handle,GWL_WNDPROC,(long)MyEditProc);
		SetWindowLong(hwnd_handle,GWL_USERDATA,(long)this);
		// _log("this:%d",(long)this);
	}
	void select(int from ,int to){
		SendMessage(hwnd_handle, EM_SETSEL, from, to);
	}
	void set_focus(){
		SetFocus(hwnd_handle);
	}			
	virtual void on_enter(){}
};

LRESULT CALLBACK MyEditProc(HWND h_edit, UINT message,WPARAM wParam, LPARAM lParam) 
{  
  if (message == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS;
  switch(message) {
  case WM_CHAR:
  {
  	// _log("enter keyup %d=%d",wParam ,VK_RETURN);
    if( wParam == VK_RETURN ) {        
    	_log("hwnd_handle editproc:%d",h_edit);
    	long v = GetWindowLong(h_edit,GWL_USERDATA);
		// _log("this editproc:%d",(long)v);
		edit *e = (edit*)v;
		e->on_enter();
        // on_enter();
        return(0);
    }    
	}
  default:
    break;
  }
  return( (LRESULT)CallWindowProc((WNDPROC)DefEditProc,h_edit,message,wParam,lParam));
}


// label control 
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