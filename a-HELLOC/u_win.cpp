#include "logger.h"
#include <windows.h>
#include <assert.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
class win{
	WNDPROC winproc;
	HINSTANCE hinst;
	HINSTANCE pinst;
	int show;
	int x,y,w,h;
	
  protected:
  	HWND hwnd_handle;
  	virtual LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)=0;
  	virtual LRESULT on_button(int ctl_id,HWND ctl_hwnd){  		
  		return 0;
  	}
  	virtual LRESULT on_create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
  		// _log("base into ");
  		return 0;
  	}
  	HWND create_ctl(LPCWSTR ctl_type ,LPCWSTR text,int x,int y,int w,int h){
		// DWORD style = WS_CHILD | WS_VISIBLE | SS_LEFT  ;
		// if (strcmp(ctl_type,L"edit" ) == 0)
		// 	 style = style |WS_BORDER|WS_TABSTOP ;
		int id = 0;		
		DWORD style = WS_CHILD | WS_VISIBLE | SS_LEFT  |WS_BORDER|WS_TABSTOP ;
	 	HWND hwndedit = CreateWindowW(ctl_type, text,style ,
	            x,y,w,h,hwnd_handle, (HMENU) id, NULL, NULL);	  
	  return hwndedit;
	}  	
  public:
  	HWND create_label(LPCWSTR text,int x,int y,int w,int h){  		
	 	return  create_ctl(L"STATIC", text,x,y,w,h);	  	
	}
	HWND create_edit(LPCWSTR text,int x,int y,int w,int h){		
	 	return  create_ctl(L"edit", text,x,y,w,h);	  	
	}
	HWND create_button(LPCWSTR text,int x,int y,int w,int h){		
	 	return  create_ctl(L"button", text,x,y,w,h);	  	
	}
	HWND get_rootwindow(){
	  HWND w =GetActiveWindow();
	  HWND last ;
	  while(w !=NULL){
	    last = w;
	    w= GetParent(last);
	  }
	  return last ;
	}
  	win(HINSTANCE hinsta, HINSTANCE pinsta,int showa){
  		winproc = WndProc ;
  		hinst = hinsta ;
  		show = showa;
  		pinst = pinsta;
  	}
  	void set_rect(int x_,int y_,int w_,int h_){
  		x = x_;
  		y = y_;
  		w = w_;
  		h = h_;
  	}  	
  	LRESULT CALLBACK _proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
		switch(msg) {
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
			if (HIWORD(wp) == BN_CLICKED) {
				 on_button(LOWORD(wp),(HWND)lp);  
            }
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);			
			break;
		}
		default :
			return DefWindowProc(hwnd, msg, wp, lp);
		}
		return 0;
  	}
  	void main(){  		
  		if (pinst==NULL) 
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
			wndcls.lpszClassName = "HELLOWIN";

			RegisterClass(&wndcls);
		}
		HWND hwnd = CreateWindow("HELLOWIN",		 /* class name */
			"HELLO--The C version",				 /* title */
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
	int loop(){
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam;
	}
	win * w;
	public :
	app(win *a){
		w = a;
	}
	~app(){
		if (NULL!=w)
			delete w;
	}
	int PASCAL main(HINSTANCE hinst, LPSTR cmdline, int show)
	{		
		assert(w);		
		w->main();
		return loop();
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


