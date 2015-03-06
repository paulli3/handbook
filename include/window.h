#ifndef __window_h__
#define __window_h__

#include "stdafx.h"
#include <windowsx.h>
#include "action.hpp"






namespace htmlayout
{

class window: public event_handler,
              public notification_handler<window>
{
public:  
  HWND          hwnd;

  dom::element  body;
  dom::element  caption;
  dom::element  button_min;
  dom::element  button_max;
  dom::element  button_icon;
  dom::element  button_close;
  dom::element  corner;

  static  window* create( int x, int y, int width, int height, const wchar_t* caption = 0 );
  static  window* self(HWND hWnd) { return (window*)::GetWindowLongPtr(hWnd,GWLP_USERDATA); }

  void            set_caption( const wchar_t* text );

  static  ATOM              register_class(HINSTANCE hInstance);
protected:
	//unsigned(-1)    handler all
	//HANDLE_BEHAVIOR_EVENT  handler behaver
	window() : event_handler(HANDLE_ALL) {}

  int       hit_test( int x, int y );
  HELEMENT  root();
  bool      is_minimized() const;
  bool      is_maximized() const;

  virtual BOOL  on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason );
  virtual LRESULT on_document_complete();
  static  void              self(HWND hWnd, window* inst) { ::SetWindowLongPtr(hWnd,GWLP_USERDATA, LONG_PTR(inst)); }
  static  LRESULT CALLBACK  win_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  static  HINSTANCE         hinstance;


  virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval)
  {
// 	  htmlayout::debug_output_console dc;
// 	  dc.printf("call of %s()\n", name);
	  if (aux::streq(name, "show_root_list"))
	  {
		  XCALL_ACTION::show_root_list(he, argv[0], argv[1]);
	  }




	  return true;
  }


};


}

#endif