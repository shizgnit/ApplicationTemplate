#include <stdio.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <windows.h>
#include <shellapi.h>

#include <iostream>

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

  //strcpy_s(_notifyIconData.szTip, TEXT("Set icon tooltip text"));

  static PIXELFORMATDESCRIPTOR _PFD = {
    sizeof(PIXELFORMATDESCRIPTOR),                                // nSize;
    1,                                                            // nVersion;
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,   // dwFlags;
    PFD_TYPE_RGBA,                                                // iPixelType;
    24,                                                           // cColorBits;
    0,                                                            // cRedBits;
    0,                                                            // cRedShift;
    0,                                                            // cGreenBits;
    0,                                                            // cGreenShift;
    0,                                                            // cBlueBits;
    0,                                                            // cBlueShift;
    0,                                                            // cAlphaBits;
    0,                                                            // cAlphaShift;
    1,                                                            // cAccumBits;
    16,                                                           // cAccumRedBits;
    16,                                                           // cAccumGreenBits;
    16,                                                           // cAccumBlueBits;
    16,                                                           // cAccumAlphaBits;
    16,                                                           // cDepthBits;
    0,                                                            // cStencilBits;
    0,                                                            // cAuxBuffers;
    PFD_MAIN_PLANE,                                               // iLayerType;
    0,                                                            // bReserved;
    0,                                                            // dwLayerMask;
    0,                                                            // dwVisibleMask;
    0                                                             // dwDamageMask;
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

  int nNoOfDevices = 0;
  POINT p;

  RAWINPUT input;
  UINT szData = sizeof(input), szHeader = sizeof(RAWINPUTHEADER);
  HRAWINPUT handle;

  static bool LBUTTONDOWN = false;
  if (LBUTTONDOWN) {
    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);

    on_touch_drag((float)p.x, (float)p.y);
  }

	switch (uMsg) {
	  case WM_CREATE:

      AllocConsole();

      freopen("CONIN$", "r", stdin);
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);

      UINT nDevices;
      PRAWINPUTDEVICELIST pRawInputDeviceList;
      GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));

      pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST)* nDevices);

      nNoOfDevices = GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));

      RID_DEVICE_INFO rdi;
      rdi.cbSize = sizeof(RID_DEVICE_INFO);

      for (int i = 0; i < nNoOfDevices; i++) {
        TCHAR device[256];
        GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICENAME, device, 0);

        UINT cbSize = rdi.cbSize;
        GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cbSize);

        std::wcout << L"Device Name: " << device << std::endl;
        std::cout << "rdi.dwType ............................ " << rdi.dwType << std::endl;

        if (rdi.dwType == RIM_TYPEMOUSE)
        {
          std::cout << "rdi.mouse.dwId ........................ " << rdi.mouse.dwId << std::endl;
          std::cout << "rdi.mouse.dwNumberOfButtons ........... " << rdi.mouse.dwNumberOfButtons << std::endl;
          std::cout << "rdi.mouse.dwSampleRate ................ " << rdi.mouse.dwSampleRate << std::endl;
        }

        if (rdi.dwType == RIM_TYPEKEYBOARD)
        {
          std::cout << "rdi.keyboard.dwKeyboardMode ........... " << rdi.keyboard.dwKeyboardMode << std::endl;
          std::cout << "rdi.keyboard.dwNumberOfFunctionKeys ... " << rdi.keyboard.dwNumberOfFunctionKeys << std::endl;
          std::cout << "rdi.keyboard.dwNumberOfIndicators ..... " << rdi.keyboard.dwNumberOfIndicators << std::endl;
          std::cout << "rdi.keyboard.dwNumberOfKeysTotal ...... " << rdi.keyboard.dwNumberOfKeysTotal << std::endl;
          std::cout << "rdi.keyboard.dwType ................... " << rdi.keyboard.dwType << std::endl;
          std::cout << "rdi.keyboard.dwSubType ................ " << rdi.keyboard.dwSubType << std::endl;
        }

        if (rdi.dwType == RIM_TYPEHID)
        {
          std::cout << "rdi.hid.dwVendorId .................... " << rdi.hid.dwVendorId << std::endl;
          std::cout << "rdi.hid.dwProductId ................... " << rdi.hid.dwProductId << std::endl;
          std::cout << "rdi.hid.dwVersionNumber ............... " << rdi.hid.dwVersionNumber << std::endl;
          std::cout << "rdi.hid.usUsage ....................... " << rdi.hid.usUsage << std::endl;
          std::cout << "rdi.hid.usUsagePage ................... " << rdi.hid.usUsagePage << std::endl;
        }
      }

      free(pRawInputDeviceList);

      // https://msdn.microsoft.com/en-us/library/windows/desktop/ms645565(v=vs.85).aspx
      RAWINPUTDEVICE rid;
      rid.usUsagePage = 0x01;
      rid.dwFlags = RIDEV_INPUTSINK; //RIDEV_NOLEGACY
      rid.hwndTarget = hWnd;

      rid.usUsage = 0x02; // Mouse
      RegisterRawInputDevices(&rid, 1, sizeof(rid));

      rid.usUsage = 0x06; // Keyboard
      RegisterRawInputDevices(&rid, 1, sizeof(rid));

      rid.usUsage = 0x04; // Joystick
      RegisterRawInputDevices(&rid, 1, sizeof(rid));

      rid.usUsage = 0x05; // Gamepad
      RegisterRawInputDevices(&rid, 1, sizeof(rid));

      // _HMENU = CreatePopupMenu();
      // AppendMenu(_HMENU, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));
      
      break;

	case WM_INPUT:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx

    handle = reinterpret_cast<HRAWINPUT>(lParam);
    //RAWINPUT input;
    //UINT szData = sizeof(input), szHeader = sizeof(RAWINPUTHEADER);
    //HRAWINPUT handle = reinterpret_cast<HRAWINPUT>(lParam);

    GetRawInputData(handle, RID_INPUT, &input, &szData, szHeader);

    if (0 && input.header.dwType == RIM_TYPEKEYBOARD)
    {
      std::cout << "Keyboard - ";
      std::cout << " MakeCode:" << input.data.keyboard.MakeCode;
      std::cout << " Flags:" << input.data.keyboard.Flags;
      std::cout << " Reserved:" << input.data.keyboard.Reserved;
      std::cout << " ExtraInformation:" << input.data.keyboard.ExtraInformation;
      std::cout << " Message:" << input.data.keyboard.Message;
      std::cout << " VKey:" << input.data.keyboard.VKey;
      std::cout << std::endl;
    }

    if (0 && input.header.dwType == RIM_TYPEMOUSE)
    {
      std::cout << "Mouse - ";
      std::cout << " usFlags:" << input.data.mouse.usFlags;
      std::cout << " ulButtons:" << input.data.mouse.ulButtons;
      std::cout << " usButtonFlags:" << input.data.mouse.usButtonFlags;
      std::cout << " usButtonData:" << input.data.mouse.usButtonData;
      std::cout << " ulRawButtons:" << input.data.mouse.ulRawButtons;
      std::cout << " lLastX:" << input.data.mouse.lLastX;
      std::cout << " lLastY:" << input.data.mouse.lLastY;
      std::cout << " ulExtraInformation:" << input.data.mouse.ulExtraInformation;
      std::cout << std::endl;
    }

    if (input.header.dwType == RIM_TYPEMOUSE && input.data.mouse.usButtonFlags & 0x0001)
    {
      GetCursorPos(&p);
      ScreenToClient(hWnd, &p);

      on_touch_press((float)p.x, (float)p.y);
      LBUTTONDOWN = true;
    }
    if (input.header.dwType == RIM_TYPEMOUSE && input.data.mouse.usButtonFlags & 0x0002)
    {
      LBUTTONDOWN = false;
    }
    if (input.header.dwType == RIM_TYPEMOUSE && input.data.mouse.usButtonFlags & 0x0400)
    {
      if (input.data.mouse.usButtonData == 0xFF88) { // 65416
        on_touch_zoom_in();
      }
      if (input.data.mouse.usButtonData == 0x0078) { // 120
        on_touch_zoom_out();
      }
    }

    break;
	  
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

//    case WM_LBUTTONDOWN: {
      // Focus
//      GetCursorPos(&p);
//      ScreenToClient(hWnd, &p);

//      on_touch_press((float)p.x, (float)p.y);
//      LBUTTONDOWN = true;
//    }

    case WM_TRAYICON:	{
      switch (wParam)	{
        case ID_TRAY_APP_ICON:
          break;
      }

      if (lParam == WM_LBUTTONUP) {
        window->Restore();
      }
      else if (lParam == WM_RBUTTONDOWN) {

//        SetForegroundWindow(_HWND);
        /*
        UINT clicked = TrackPopupMenu(
          _HMENU,
          TPM_RETURNCMD | TPM_NONOTIFY,
          p.x,
          p.y,
          0,
          _HWND,
          NULL
          );
          
        printf("returned from call to track\n");

        SendMessage(_HWND, WM_NULL, 0, 0);
        if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM) {
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
