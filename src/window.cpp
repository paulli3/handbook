#include "stdafx.h"

namespace htmlayout
{
 #define MAX_LOADSTRING 100
 TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
 TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
  HINSTANCE window::hinstance = 0;

  wchar_t CLASSNAME[MAX_LOADSTRING] = L"htmlayout::window";

  // register window class

  ATOM  window::register_class(HINSTANCE hInstance)
  {
	  LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	  LoadString(hInstance, IDC_W7AERO, CLASSNAME, MAX_LOADSTRING);
    //hinstance = hInstance;
	WNDCLASSEX wcex = { 0 };
	// Initialize global strings
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)win_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_W7AERO));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName = CLASSNAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);

// 
// 
// 	wcex.cbSize = sizeof(WNDCLASSEX);
// 
//     wcex.style          = CS_HREDRAW | CS_VREDRAW;
//     wcex.lpfnWndProc    = (WNDPROC)win_proc;
//     wcex.cbClsExtra     = 0;
//     wcex.cbWndExtra     = 0;
//     wcex.hInstance      = hInstance;
// 	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_W7AERO));
//     wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
//     wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
//     wcex.lpszMenuName   = 0;
//     wcex.lpszClassName  = CLASSNAME;
// 	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
// 
//     return RegisterClassExW(&wcex);
  }

// #ifndef WS_EX_LAYERED
// #define WS_EX_LAYERED           0x00080000
// #endif

  // create window instance

  window* window::create( int x, int y, int width, int height, const wchar_t* caption ) 
  {
    window* pw = new window();
	//WS_EX_TOPMOST  置顶选项
    // UINT style = WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
	// pw->hwnd = CreateWindowExW(WS_EX_LAYERED, CLASSNAME, NULL, style,  x, y, width, height, NULL, NULL, hinstance, NULL);
	
	pw->hwnd = CreateWindow(CLASSNAME, caption,   WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hinstance, NULL);
	
	::SetWindowPos(pw->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//::SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE)
//    pw->hwnd = CreateWindowExW( 0, CLASSNAME, NULL, style ,
//                                x, y, width, height, NULL, NULL, hinstance, NULL);
//	SetDoubleClickTime(2000);
	self(pw->hwnd,pw);
  
	//暂时隐藏
	HTMLayoutSetCallback(pw->hwnd,&callback,pw);
    PBYTE pb; DWORD cb;
#ifndef VER2
    if(load_resource_data(L"DEFAULT",pb,cb))
#else
	if (load_resource_data(L"MINDEFAULT", pb, cb))
#endif
    {
      HTMLayoutLoadHtml(pw->hwnd,pb,cb);

      dom::element r = pw->root();

      pw->body            = r.find_first("body");
      pw->caption         = r.get_element_by_id("caption");
      pw->button_min      = r.get_element_by_id("minimize");
      pw->button_max      = r.get_element_by_id("maximize");
      pw->button_icon     = r.get_element_by_id("icon");
      pw->button_close    = r.get_element_by_id("close");
      pw->corner          = r.get_element_by_id("corner");

      attach_event_handler(pw->hwnd, pw);

      pw->set_caption(caption);

    }
    return pw;
  }
  /*加载完成后执行*/
  LRESULT window::on_document_complete() { 
	  //MessageBoxA(NULL, "1", "333", 0); 
	  
		  HELEMENT he=0;
		  XCALL_ACTION::show_root_list(he, hwnd, "#second", "db");
		  dom::element root = dom::root_element(hwnd);
		  //MessageBoxW(NULL, $D(root.find_first("#value")).get_value().to_string().c_str(),L"1",0);
		  XCALL_ACTION::show_main_list(he, hwnd, "#mainbody", $D(root.find_first("#var")).get_attribute("rootid"));
		  return 0;
  }
  
  void window::set_caption( const wchar_t* text )
  {
    if(text)
    {
      ::SetWindowTextW(hwnd,text);
      if( caption.is_valid() )
      {
        caption.set_text(text);
        //caption.update(true);
      }
    }
  }


  HELEMENT  window::root()
  {
    return dom::element::root_element(hwnd);
  }

  int       window::hit_test( int x, int y )
  {
    
    POINT pt; pt.x = x; pt.y = y;
    ::MapWindowPoints(HWND_DESKTOP,hwnd,&pt,1);

    if( caption.is_valid() && caption.is_inside(pt) )
      return HTCAPTION;

    if( button_icon.is_valid() && button_icon.is_inside(pt) )
      return HTSYSMENU;

    if( corner.is_valid() && corner.is_inside(pt) )
      return HTBOTTOMRIGHT;

    RECT body_rc = body.get_location(ROOT_RELATIVE | CONTENT_BOX);

    if( PtInRect(&body_rc, pt) )
      return HTCLIENT;

    if( pt.y < body_rc.top + 10 ) 
    {
      if( pt.x < body_rc.left + 10 ) 
        return HTTOPLEFT;
      if( pt.x > body_rc.right - 10 ) 
        return HTTOPRIGHT;
    }
    else if( pt.y > body_rc.bottom - 10 ) 
    {
      if( pt.x < body_rc.left + 10 ) 
        return HTBOTTOMLEFT;
      if( pt.x > body_rc.right - 10 ) 
      {
//         htmlayout::debug_output_console dc;
//         dc.printf("HTBOTTOMRIGHT\n");
        return HTBOTTOMRIGHT;
      }
    }

    if( pt.y < body_rc.top ) 
      return HTTOP;
    if( pt.y > body_rc.bottom ) 
      return HTBOTTOM;
    if( pt.x < body_rc.left ) 
      return HTLEFT;
    if( pt.x > body_rc.right ) 
      return HTRIGHT;


    return HTCLIENT;
   
  }

  BOOL window::on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason )
  {
	
    if( type != BUTTON_CLICK)
      return FALSE; // handling only button clicks here. 

    if( target == button_min)
    {
      ::ShowWindow(hwnd,SW_MINIMIZE); 
      return TRUE;
    }
    if( target == button_max)
    {
      if( is_maximized())
        ::ShowWindow(hwnd,SW_RESTORE); 
      else
        ::ShowWindow(hwnd,SW_MAXIMIZE); 
      
      return TRUE;
    }
    if( target == button_close)
    {
      ::PostMessage(hwnd, WM_CLOSE, 0,0); 
      return TRUE;
    }

    // click on some other button
    dom::element button = target;
    //::MessageBoxW(button.get_element_hwnd(true) ,button.get_attribute("id"), L"Click on the button with id:", MB_OK);
    //::MessageBox(button.get_element_hwnd(true) ,"test", "Click on the button with id:", MB_OK);

    return TRUE;
  }

  bool window::is_minimized() const
  {
      WINDOWPLACEMENT wp;
      GetWindowPlacement(hwnd,&wp);
      return wp.showCmd == SW_SHOWMINIMIZED;
  }

  bool window::is_maximized() const
  {
      WINDOWPLACEMENT wp;
      GetWindowPlacement(hwnd,&wp);
      return wp.showCmd == SW_SHOWMAXIMIZED;
  }

  LRESULT CALLBACK window::win_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    LRESULT lResult;
    BOOL    bHandled;

  // HTMLayout +
    // HTMLayout could be created as separate window 
    // using CreateWindow API.
    // But in this case we are attaching HTMLayout functionality
    // to the existing window delegating windows message handling to 
    // HTMLayoutProcND function.
    lResult = HTMLayoutProcND(hwnd,message,wParam,lParam, &bHandled);
    if(bHandled)
      return lResult;
  // HTMLayout -

    window* me = self(hwnd);

    switch (message) 
    {
// 	case WM_LBUTTONDBLCLK:
// 		MessageBoxA(NULL,"1","1",0);
//		break;
 		case WM_ERASEBKGND:
 			return TRUE; // as HTMLayout will draw client area in full

      case WM_NCHITTEST:
        if(me)
          return me->hit_test( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
        break;
// 
      case WM_NCCALCSIZE:  return 0; // we have no non-client areas.
      case WM_NCPAINT:     return 0; // we have no non-client areas.
      case WM_NCACTIVATE:  return (wParam == 0)? TRUE : FALSE; // we have no non-client areas.
// 
      case WM_GETMINMAXINFO:
		  {
							   LRESULT lr = DefWindowProcW(hwnd, message, wParam, lParam);
							   MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
							   pmmi->ptMinTrackSize.x = ::HTMLayoutGetMinWidth(hwnd);
							   RECT rc; GetWindowRect(hwnd, &rc);
							   pmmi->ptMinTrackSize.y = ::HTMLayoutGetMinHeight(hwnd, rc.right - rc.left);
							   return lr;
		  }

      case WM_CLOSE:
        ::DestroyWindow(hwnd);
        return 0;

      case WM_DESTROY:
        delete me; // delete window instance!
        self(hwnd,0);
        PostQuitMessage(0);
        return 0;

     }
     return DefWindowProcW(hwnd, message, wParam, lParam);
  }

















}
