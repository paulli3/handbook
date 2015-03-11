
#ifndef __window_h__
#define __window_h__

#include "stdafx.h"
#include <windowsx.h>
#include <codecvt>

#pragma once
//#include "stdafx.h"
#include "sql.hpp"
#include "debug.h"
#include "dlg.h"
#include "resource.h"


namespace XCALL_ACTION
{
	void onDocumentCompele()
	{

	}


	inline std::string ToUTF8(const wchar_t* wideStr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(wideStr);
	}
	/*main list 列表*/
	void inline show_main_list(HELEMENT he, HWND hwnd, json::value id)
	{
		
		sql * PSQL = &sql::getInstance();
		PSQL->createTable();
		Record * precode;
		
		PSQL->query("SELECT * FROM main order by id desc");
		std::string html = "";
		while ((precode = PSQL->RESCULT()->getone()))
		{
			html = html + "<tr><td value=\"" + precode->get("id") + "\">" + ToUTF8(aux::a2w(precode->get("title").c_str())) + "</td><td>" + ToUTF8(aux::a2w(precode->get("mean").c_str())) + "</td></tr>";
		}
		//MessageBoxA(NULL, html.c_str(), "1", 0);

		const unsigned  char chtml[1020000] = "";
		strcpy((char*)chtml, html.c_str());
		htmlayout::dom::element root = he;
		root = root.root_element(hwnd);
		htmlayout::dom::element rootList = htmlayout::dom::element(root.find_first(id.to_string().c_str()));//
		if (rootList.is_valid())
		{
			rootList.set_html(chtml, sizeof(chtml));
		}
		else{
			MessageBoxA(NULL, "1", "1", 0);
		}
	}
	/*root list 列表*/
	void inline show_root_list(HELEMENT he,HWND hwnd, json::value id, json::value db)
	{
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		Record * precode;
		PSQL->query("SELECT * FROM root order by id desc");

		std::string html = "";
		while ((precode = PSQL->RESCULT()->getone()))
		{
			html = html + "<div value=\"" + precode->get("id") + "\">" + ToUTF8(aux::a2w(precode->get("title").c_str())) + "</div>";
		}
		
		const unsigned  char chtml[102000] = "";
		strcpy((char*)chtml, html.c_str());
		htmlayout::dom::element root=he;
		root = root.root_element(hwnd);
		htmlayout::dom::element rootList = htmlayout::dom::element(root.find_first(id.to_string().c_str()));
		if (rootList.is_valid())
		{
			rootList.set_html(chtml, sizeof(chtml));
		}
		else{
			MessageBoxA(NULL, "1", "1", 0);
		}
	}


	bool inline show_root_edit_box(HELEMENT he, json::value id, json::value val, HWND hwnd)
	{
		//::htmlayout::window * a = reinterpret_cast<::htmlayout::window *>(lp);
		//htmlayout::dom::element btn = he;
		dlg dlg1(hwnd);
		htmlayout::named_values  p;
		p[TEXT("rootval")] = val.to_string();
		p[TEXT("rootid")] = id.to_string();
		if (dlg1.input(IDR_ROOT_EDIT, p) == IDOK)
		{
			sql * PSQL = &sql::getInstance();
			PSQL->connect("db");
			std::string sval = std::string(aux::w2a(p[L"rootval"].to_string().c_str()));
			std::string sql = "";
			if (p[TEXT("rootid")] != "add"){
				sql = "UPDATE root set title='" + sval + "' where id=" + std::string(aux::w2a(p[L"rootid"].to_string().c_str()));
			}
			else{
				sql = "INSERT INTO root values(NULL,'" + sval + "') ";
			}
			
			PSQL->query(sql);
			//MessageBox(NULL, p[L"rootval"].to_string().c_str(), L"1", 0);
			return true;
		}
		//return false;
		/*std::wstring s;
		for (htmlayout::named_values::const_iterator it = p.begin(); it != p.end(); ++it)
		{
			s += (*it).first;
			s += L"=";
			s += (*it).second.to_string();
			s += L"\n";
		}
		MessageBox(NULL, s.c_str(), L"1", 0);*/
		return false;
	}

	void inline show_main_edit_box(HELEMENT he, json::value id, HWND hwnd)
	{
		//::htmlayout::window * a = reinterpret_cast<::htmlayout::window *>(lp);
		//htmlayout::dom::element btn = he;
		dlg dlg1(hwnd);
		dlg1.show(IDR_MAIN_EDIT);
	}


}


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
  virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) { 
	  showDebug(event_type);
	  if (event_type == MOUSE_DCLICK)
	  {
		  std::wstring a = $D(target).get_value().to_string().c_str();
		  return false;
	  }
	  return FALSE;
  }
  static  void              self(HWND hWnd, window* inst) { ::SetWindowLongPtr(hWnd,GWLP_USERDATA, LONG_PTR(inst)); }
  static  LRESULT CALLBACK  win_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  static  HINSTANCE         hinstance;


  virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval)
  {
// 	  htmlayout::debug_output_console dc;
// 	  dc.printf("call of %s()\n", name);

	  if (aux::streq(name, "show_root_list")){
		  XCALL_ACTION::show_root_list(he,hwnd, argv[0], argv[1]);
	  }
	  else if (aux::streq(name, "show_main_list"))
	  {
		  XCALL_ACTION::show_main_list(he, hwnd, argv[0]);
	  }
	  else if (aux::streq(name, "show_root_edit")){
		  if (XCALL_ACTION::show_root_edit_box(he, argv[0], argv[1], hwnd))//static_cast<void *>(this)
		  {
			  retval = TEXT("1");
		  }
	  }
	  else if (aux::streq(name, "show_main_edit")){
			  XCALL_ACTION::show_main_edit_box(he, argv[0], hwnd);//static_cast<void *>(this)
	  }else if (aux::streq(name, "alert")){
		  MessageBox(hwnd, argv[0].to_string().c_str(), TEXT("alert!"), MB_OK);
	  }
	 
	  return true;
  }
};


}



#endif





