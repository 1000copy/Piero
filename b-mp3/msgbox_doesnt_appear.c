/*
question :

I'm stuck with a strange problem. I'm making a Win32 application in VC++ 2008, making 
a class to encapsulate most of the work for easy repetition when calling a MessageBox. 
The message box` is created (I think) but doesn't show up unless I press the Alt key!

src code :
http://stackoverflow.com/questions/11308530/show-a-messagebox-when-clicking-menu-doesnt-appear-on-screen
solution:
    http://stackoverflow.com/questions/4299915/win32-messagebox-doesnt-appear

    Ohhhhhhh finally i found the solution of this problem ... and for everyone to benefit the problem was in 
    WndProc(.......) at the WM_PAINT Message i wrote some code in it and removed all the code along with 
    BeginPaint and EndPaint functions so the program enter a freeze period once anything is being painted 
    over it including that MessageBox but it only show when i press Alt i think coz the control is transfered 
    to the system in that step to show the system Menu (i think)

    the solution either remove the WM_PAINT message handler or add the normal BeginPaint and EndPaint functions

    作为遇到同一个诡异问题的人，我非常理解他的 Ohhhhhhh,就像是长长的吁了一口气。


*/
#include <windows.h>
#define _UNICODE

LPCTSTR App_Name = TEXT("TestApp");
LPCTSTR App_Title = TEXT("TestTitle");

const int windowWidth = 480;
const int windowHeight = 480;
#define MNU_HELP_ABOUT 1

void create_menu(HWND hwnd){
    HMENU hMenu, hSubMenu;       
    hMenu = CreateMenu();
    hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, MNU_HELP_ABOUT, "&About");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&?");
    SetMenu(hwnd, hMenu);
}
    
LRESULT CALLBACK WndProc(HWND hwnd,UINT message, WPARAM wparam, LPARAM lparam)
{   
        switch( message )
        {
            case  WM_CREATE:
            {
                create_menu(hwnd);
                break;
            }
            case WM_PAINT:
            {
                UpdateWindow(hwnd);
                break;
            }
            case WM_COMMAND:
            {
                int id = LOWORD(wparam);
                int event = LOWORD(wparam);
                if(id == MNU_HELP_ABOUT)
                {
                    MessageBox(NULL, TEXT("TEXT"), TEXT("TITLE"), MB_OK | MB_ICONINFORMATION);
                    break;
                }
                break;
            }
            default:
                return DefWindowProc(hwnd, message, wparam, lparam);
        }
        return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = App_Name;
    // wc.lpszMenuName = MAKEINTRESOURCE(MNU_MAINMENU);
    wc.lpszMenuName = "";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc); 

    RECT rc;
    GetWindowRect(GetDesktopWindow(), &rc);

    long screenWidth = rc.right;
    long screenHeight = rc.bottom;

    HWND hwnd = CreateWindow
    (
        App_Name,
        App_Title,
        WS_MINIMIZEBOX | WS_SYSMENU,
        (screenWidth / 2) - (windowWidth / 2), (screenHeight / 2)-(windowHeight/2),
        windowWidth, windowHeight,
        NULL, NULL,
        hInstance, NULL
    );


    ShowWindow(hwnd, iCmdShow );
    UpdateWindow(hwnd);

    MSG msg;

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return msg.wParam;
}