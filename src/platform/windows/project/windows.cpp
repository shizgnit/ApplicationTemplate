#include <stdio.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <windows.h>
#include <shellapi.h>

#include "ApplicationTemplate.hpp"

#define ID_TRAY_APP_ICON                5000
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define WM_TRAYICON ( WM_USER + 1 )

class Window {
public:
  Window(void) {
    _HDC = NULL;
    _HWND = NULL;
    _HGLRC = NULL;
    width = 640;
    height = 480;
    _bpp = 16;
    _fullscreen = false;
  };

  bool Init();

  bool Create(WNDPROC WndProc);

  bool Show() {
    ShowWindow(_HWND, SW_SHOW);
    SetForegroundWindow(_HWND);
    SetFocus(_HWND);

    return TRUE;
  }

  bool Close() { return true; }
  bool Resize(int w, int h);

  bool Minimize();
  bool Restore();
  bool InitNotifyIconData();

  HWND Handle() { return _HWND; }
  HDC DC() { return _HDC; }

public:
  unsigned int width;
  unsigned int height;

protected:
  HDC _HDC;
  HWND _HWND;

  HGLRC _HGLRC;

  HMENU _HMENU;

  bool _fullscreen;

  unsigned int _bpp;

  NOTIFYICONDATA _notifyIconData;
};

bool Window::Init() {
  return true;
}

bool Window::Create(WNDPROC WndProc) {
  const char *title = "Client";

  WNDCLASS WndClass;

  DWORD dwExStyle;
  DWORD dwStyle;

  RECT WndRect;

  WndRect.left=(long)0;
  WndRect.right=(long)width;
  WndRect.top=(long)0;
  WndRect.bottom=(long)height;

  HINSTANCE hInstance = GetModuleHandle(NULL);

  WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  WndClass.lpfnWndProc = WndProc;
  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hInstance = hInstance;
  WndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  WndClass.hbrBackground = NULL;
  WndClass.lpszMenuName = NULL;
  WndClass.lpszClassName = L"Client";

  if(!RegisterClass(&WndClass)) {
	  return FALSE;
  }

  if(_fullscreen) {
	  DEVMODE dmScreenSettings;
	  memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

	  dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	  dmScreenSettings.dmPelsWidth	= width;
	  dmScreenSettings.dmPelsHeight	= height;
	  dmScreenSettings.dmBitsPerPel	= _bpp;
	  dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	  if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
  	  return FALSE;
	  }

	  dwExStyle = WS_EX_APPWINDOW;
	  dwStyle = WS_POPUP;
	  ShowCursor(FALSE);
  }
  else
  {
	  dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
  }

  AdjustWindowRectEx(&WndRect, dwStyle, FALSE, dwExStyle);

  _HWND = CreateWindowEx(
            dwExStyle,
					  L"Client",
					  L"Client",
					  dwStyle |	WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,
					  0,
					  0,
					  WndRect.right-WndRect.left,
					  WndRect.bottom-WndRect.top,
					  NULL,
					  NULL,
					  hInstance,
					  NULL
          );

  if(!_HWND) {
	  return FALSE;
  }

  _HDC = GetDC(_HWND);

  memset(&_notifyIconData, 0, sizeof(NOTIFYICONDATA));

  _notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

  _notifyIconData.hWnd = _HWND;
  _notifyIconData.uID = ID_TRAY_APP_ICON;
  _notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  _notifyIconData.uCallbackMessage = WM_TRAYICON;
  _notifyIconData.hIcon = (HICON)LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

  //strcpy_s(_notifyIconData.szTip, TEXT("Green man.. here's looking at ya!"));

  //PIXELFORMATDESCRIPTOR to PFD_SWAP_COPY

  static PIXELFORMATDESCRIPTOR _PFD = {
  sizeof(PIXELFORMATDESCRIPTOR),
  1,                     // nVersion should be set to 1
  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // buffer can draw to window // buffer supports OpenGL drawing // buffer is double buffered
  PFD_TYPE_RGBA,         // rgba pixels
  24,                    // 24-bit color depth
  0, 0, 0, 0, 0, 0,      // look up rest at MSDN, color bits ignored
  0,                     // no alpha buffer
  0,                     // shift bit ignored
  1,                     // no accumulation buffer
  16, 16, 16, 16,        // accumulation bits
  16,                    // 16 bit z buffer
  0,                     // no stencil buffer
  0,                     // no auxiliary buffer
  PFD_MAIN_PLANE,        // main drawing layer
  0,                     // reserved
  0, 0, 0                // layer mask ignored
  };

  int i = ChoosePixelFormat(_HDC, &_PFD);

  SetPixelFormat(_HDC, i, &_PFD);

  _HGLRC = wglCreateContext(_HDC);
  wglMakeCurrent(_HDC, _HGLRC);

  Resize(width, height);

  glewInit();

  AddClipboardFormatListener(_HWND);

  _HMENU = CreatePopupMenu();
  AppendMenu(_HMENU, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));

  return true;
}

bool Window::Resize(int w, int h) {
  width = w;
  height = h;

  glViewport(0, 0, width, height);

  on_resize(width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 1000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  return true;
}

bool Window::Minimize() {
  Shell_NotifyIcon(NIM_ADD, &_notifyIconData);
  ShowWindow(_HWND, SW_HIDE);
  return true;
}

bool Window::Restore() {
  Shell_NotifyIcon(NIM_DELETE, &_notifyIconData);
  ShowWindow(_HWND, SW_SHOW);
  return true;
}


Window *window;
bool running;
bool setupGraphics(int w, int h);
void renderFrame();

void OnInit() {
  on_startup("C:\\Users\\codeneko\\Dropbox\\Development\\projects\\ApplicationTemplate\\assets\\");
}

void OnTimer() {
  on_draw();
  SwapBuffers(window->DC());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
//    case WM_CREATE:
//      _HMENU = CreatePopupMenu();
//      AppendMenu(_HMENU, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));
//      break;

    case WM_ACTIVATE: {
      if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam))) {
        SetTimer(hWnd, 1, 10, NULL);
      }
      return 0;
    }

    case WM_SYSCOMMAND:	{
      switch (wParam) {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
          return 0;
      }
      break;
    }

    case WM_TIMER: {
      OnTimer();
      return 0;
    }

    case WM_CLOSE: {
      //KillTimer(hWnd, 1);
      //running = false;
      //return 0;
      window->Minimize();
      return 0;
    }

    case WM_SIZE: {
      window->Resize(LOWORD(lParam),HIWORD(lParam));
      return 0;
    }

    case WM_LBUTTONUP: {
      // Focus
    }

    case WM_TRAYICON:	{
      printf("Tray icon notification, from %d\n", wParam);
      switch (wParam)	{
      case ID_TRAY_APP_ICON:
        printf("Its the ID_TRAY_APP_ICON.. one app can have several tray icons, ya know..\n");
        break;
      }

      if (lParam == WM_LBUTTONUP) {
        printf("You have restored me!\n");
        window->Restore();
      }
      else if (lParam == WM_RBUTTONDOWN) {
        printf("Mmm.  Let's get contextual.  I'm showing you my context menu.\n");
        POINT curPoint;
        GetCursorPos(&curPoint);
//        SetForegroundWindow(_HWND);
        printf("calling track\n");
        /*
        UINT clicked = TrackPopupMenu(
          _HMENU,
          TPM_RETURNCMD | TPM_NONOTIFY,
          curPoint.x,
          curPoint.y,
          0,
          _HWND,
          NULL
          );
          
        printf("returned from call to track\n");

        SendMessage(_HWND, WM_NULL, 0, 0);
        if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM) {
          printf("I have posted the quit message, biatch\n");
          PostQuitMessage(0);
        }
        */
      }
    }

  }

  return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  MSG msg;

  window = new Window();

  if(window->Create(WndProc)) {
    window->Show();

    OnInit();

    running = true;
    while(running) {
      if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
        if(msg.message==WM_QUIT || msg.message==WM_CLOSE) {
          break;
        }
        else {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
    }

    window->Close();
  }
  else {
    MessageBox(NULL,L"Window Creation Error.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
  }

  return (0);
}
