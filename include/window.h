
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
#include "topdlg.h"
#include "resource.h"


namespace XCALL_ACTION
{
	inline char * get_dir()
	{
		static char g_cmd_path[MAX_PATH];
		if (g_cmd_path[0] != '\0') return g_cmd_path;
		memset(g_cmd_path, 0, MAX_PATH*sizeof(char));
		GetCurrentDirectoryA(MAX_PATH, g_cmd_path);
		// showDebug((g_cmd_path));
		return g_cmd_path;
	}


	/*void onDocumentCompele()
	{

	}
	inline BOOL WStringToString(const std::wstring &wstr, std::string &str)
	{
		int nLen = (int)wstr.length();
		str.resize(nLen, ' ');

		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

		if (nResult == 0)
		{
			return FALSE;
		}
		return TRUE;
	}*/

	inline std::string ToUTF8(const wchar_t* wideStr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(wideStr);
	}
	/*main list 列表*/
	void inline show_main_list(HELEMENT he, HWND hwnd, json::value id, json::value rootid)
	{
		sql * PSQL = &sql::getInstance();
		PSQL->createTable();
		Record * precode;
		char * exesql = "SELECT * FROM main where root_id=%s order by id desc";
		const char * srootid = aux::w2a(rootid.to_string().c_str());
		if (*srootid == '\0')return;
		
		PSQL->query(sqlite3_mprintf(exesql, srootid));
		std::string html = "";
		while ((precode = PSQL->RESCULT()->getone()))
		{
			html = html + "<tr value=\"" + precode->get("id") + "\"><td >" + ToUTF8(aux::a2w(precode->get("title").c_str())) + "</td><td>" + ToUTF8(aux::a2w(precode->get("mean").c_str())) + "</td></tr>";
		}
		//MessageBoxA(NULL, html.c_str(), "1", 0);
		//const unsigned  char *  chtml = 0;
		const char * chtml1 = html.c_str();
		//chtml = (unsigned  char *)chtml1;
		//strncpy((char*)chtml, html.c_str(),html.size()+1);
		htmlayout::dom::element root = he;
		root = root.root_element(hwnd);
		htmlayout::dom::element rootList = htmlayout::dom::element(root.find_first(id.to_string().c_str()));//
		if (rootList.is_valid())
		{
			rootList.set_html((unsigned  char *)chtml1, strlen(chtml1));
			//MessageBoxA(NULL, chtml1, "1", 0);
		}
		else{
			MessageBoxA(NULL, "1", "1", 0);
		}
		
	}
	/*main 详细*/
	void inline show_main_view(HELEMENT he, HWND hwnd, json::value id)
	{
		using namespace std;
		sql * PSQL = &sql::getInstance();
		PSQL->createTable();
		Record * precode;

		string s_id = aux::w2a(id.to_string().c_str());
		if (s_id.c_str()[0] == '\0')return;

		PSQL->query("SELECT * FROM main where id=" + s_id + "");
		std::string html = "";
		while ((precode = PSQL->RESCULT()->getone()))
		{
			html = html + precode->get("content");
		}
		topdlg dlg1(hwnd, WS_SIZEBOX);
		//topdlg mtopdlg(hwnd, WS_SIZEBOX);
		htmlayout::named_values  p;
		p[TEXT("val")] = html.c_str();
		dlg1.input(IDR_VIEW,p);
		//MessageBoxA(NULL, html.c_str(), "1", 0);
//		MessageBoxA(NULL, html.c_str(), "1", 0);
// 		const unsigned  char chtml[1020000] = "";
// 		strcpy((char*)chtml, html.c_str());
// 		htmlayout::dom::element root = he;
// 		root = root.root_element(hwnd);
// 		htmlayout::dom::element rootList = htmlayout::dom::element(root.find_first(id.to_string().c_str()));//
// 		if (rootList.is_valid())
// 		{
// 			rootList.set_html(chtml, sizeof(chtml));
// 		}
// 		else{
// 			MessageBoxA(NULL, "1", "1", 0);
// 		}
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
		/*转换方式*/
// 		const unsigned  char chtml[102000] = "";
// 		strcpy((char*)chtml, html.c_str());
		/*================*/
		
		/*================*/
		htmlayout::dom::element root=he;
		root = root.root_element(hwnd);
		htmlayout::dom::element rootList = htmlayout::dom::element(root.find_first(id.to_string().c_str()));
		if (rootList.is_valid())
		{
			//rootList.set_html(chtml, sizeof(chtml));
			rootList.set_html((const unsigned char *)html.c_str(), strlen(html.c_str()));
			
			htmlayout::dom::element first_child = $D(root.find_first(id.to_string().c_str())).child(0);
			HTMLayoutSetAttributeByName(root.find_first("#var"), "rootid", first_child.get_attribute("value"));
			//$D($D(root.find_first(id.to_string().c_str())).child(1)).set_attribute("value",L"1000");
			HTMLayoutSetAttributeByName(first_child, "selected", TEXT("1"));
			//MessageBoxW(NULL, $D($D(root.find_first(id.to_string().c_str())).child(1)).get_attribute("selected"),L"1",0);
			
		}
		else{
			MessageBoxA(NULL, "1", "1", 0);
		}
	}


	bool inline show_root_edit_box(HELEMENT he, json::value id, json::value val, HWND hwnd)
	{
		//::htmlayout::window * a = reinterpret_cast<::htmlayout::window *>(lp);
		//htmlayout::dom::element btn = he;
		topdlg dlg1(hwnd);
		htmlayout::named_values  p;
		p[TEXT("rootval")] = val.to_string();
		p[TEXT("rootid")] = id.to_string();
		
		if (dlg1.input(IDR_ROOT_EDIT, p) == IDOK)
		{
			sql * PSQL = &sql::getInstance();
			PSQL->connect("db");
			std::string sval = std::string(aux::w2a(p[L"rootval"].to_string().c_str()));
			std::string sql = "";
			std::wstring a = p[TEXT("rootid")].to_string().c_str();
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

	bool inline delete_main(HELEMENT he, HWND hwnd, json::value id)
	{
		using namespace std;
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		string sql;
		sql = "delete FROM main where id=" + std::string(aux::w2a(id.to_string().c_str()));
		return PSQL->query(sql);
	}
	bool inline delete_root(HELEMENT he, HWND hwnd, json::value id)
	{
		using namespace std;
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		string sql;
		const char * rootid = aux::w2a(id.to_string().c_str());
		sql = "delete FROM main where root_id=" + std::string(rootid);
		if (PSQL->query(sql))
		{
			sql = "delete FROM root where id=" + std::string(rootid);
			return PSQL->query(sql);
		}
	}
	bool inline show_main_edit_box(HELEMENT he, json::value id, json::value rootid, HWND hwnd)
	{
		using namespace std;
		//::htmlayout::window * a = reinterpret_cast<::htmlayout::window *>(lp);
		//htmlayout::dom::element btn = he;
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		topdlg dlg1(hwnd, WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX );
		string sql;
		const char * _id = aux::w2a(id.to_string().c_str());
		const char * _rootid = aux::w2a(rootid.to_string().c_str());
		sql = "SELECT * FROM main where id=" + std::string(_id);
		Record * data=0;
		if (*_id != '\0' && id != "add"){
			PSQL->query(sql);
			data = PSQL->RESCULT()->getone();
		}

		htmlayout::named_values  p;
		if (data){
			p[TEXT("mainid")] = id;
			p[TEXT("mean")] = data->get("mean");
			p[TEXT("title")] = data->get("title");
			p[TEXT("content")] = data->get("content");
			p[TEXT("content1")] = data->get("content");
		}
		else{
			p[TEXT("mainid")] = id;
			p[TEXT("mean")] = "";
			p[TEXT("title")] = TEXT("");
			p[TEXT("content")] = TEXT("");
		}
		//htmlayout::dom
		if (dlg1.input(IDR_MAIN_EDIT, p) == IDOK)
		{
			string s_mainid = std::string(aux::w2a(p[TEXT("mainid")].to_string().c_str()));
			string s_mean = std::string(aux::w2a(p[TEXT("mean")].to_string().c_str()));
			string s_title = std::string(aux::w2a(p[TEXT("title")].to_string().c_str()));
			string s_content = std::string(aux::w2a(p[TEXT("content")].to_string().c_str()));
			std::wstring s = p[TEXT("mainid")].to_string().c_str();			
			if (id != "add"){
				sql = "UPDATE main set title='%q', mean='%q',content='%q' where id=" + std::string(aux::w2a(id.to_string().c_str()));
				sql = sqlite3_mprintf(sql.c_str(), s_title.c_str(),s_mean.c_str(),s_content.c_str());
			}
			else{
				sql = "INSERT INTO main (id,root_id,title,mean,content,create_time) values(NULL,%q,'" + s_title + "','" + s_mean + "','" + "%q" + "',1) ";
				//	MessageBoxA(hwnd, s_content.c_str(), "1", 0);
				sql = sqlite3_mprintf(sql.c_str(),_rootid, s_content.c_str());
			}
			//htmlayout::dom::element root = $D(he).root_element(hwnd);
			//htmlayout::dom::element  root = htmlayout::dom::element::root_element(dlg1.hwnd);
			//htmlayout::dom::element richtext = root.find_first("#test");
			//if (!richtext.is_valid()){
			//	MessageBoxA(NULL, "richtext error", "error", 0);
				//WStringToString((richtext.xcall("saveHTML", "c:/", "emit-all-images")).to_string().c_str(), mcon);
				//return;
			//}
			return PSQL->query(sql);
			//MessageBoxA(hwnd, s_content.c_str(), "1", 0);
		}
		return FALSE;
	}


}
#define __DIR__ XCALL_ACTION::get_dir()


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
  static  void        self(HWND hWnd, window* inst) { ::SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(inst)); }
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
//   virtual BOOL on_mouse(HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates) { 
// 	  if (event_type == MOUSE_DCLICK)
// 	  {
// 		  htmlayout::dom::element src = target;
// 
// 		  if (aux::streq(src.get_element_type(), "text"))
// 		  {
// 			  src = src.parent();
// 		  }
// 		  if (aux::streq(src.get_element_type(), "td"))
// 		  {
// 			  src = src.parent();
// 		  }
// 		  if (aux::streq(src.get_element_type(), "tr") )
// 		  {
// 			  if (!aux::wcseq(src.get_attribute("value") , L""))
// 			  {
// 				  XCALL_ACTION::show_main_view(he, hwnd, src.get_attribute("value"));
// 			  }
// 		  }
// 		  
// 		  //
// 		  return false;
// 	  }
// 	  return FALSE;
//   }
  
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
		  XCALL_ACTION::show_main_list(he, hwnd, argv[0], argv[1]);
	  }
	  else if (aux::streq(name, "show_root_edit")){
		  if (XCALL_ACTION::show_root_edit_box(he, argv[0], argv[1], hwnd))//static_cast<void *>(this)
		  {
			  retval = TEXT("1");
		  }
	  }
	  else if (aux::streq(name, "show_main_edit")){
		  if (XCALL_ACTION::show_main_edit_box(he, argv[0], argv[1], hwnd))
		  {
			  retval = TEXT("1");
		  }
			 //static_cast<void *>(this)
	  }else if (aux::streq(name, "alert")){
		  MessageBox(hwnd, argv[0].to_string().c_str(), TEXT("alert!"), MB_OK);
	  }
	  else if (aux::streq(name, "show_main_view")){
		  XCALL_ACTION::show_main_view(he, hwnd, argv[0]);
	  }
	  else if (aux::streq(name, "delete_main")){
		  if (XCALL_ACTION::delete_main(he, hwnd, argv[0]))
		  {
			  retval = TEXT("1");
		  }
	  }
	  else if (aux::streq(name, "delete_root")){
		  if (XCALL_ACTION::delete_root(he, hwnd, argv[0]))
		  {
			  retval = TEXT("1");
		  }
	  } else if (aux::streq(name, "appwidth")){
		  RECT rect;
		  GetWindowRect(hwnd,&rect);
		  char w[50];
		  sprintf(w,"%d|%d",rect.right - rect.left,rect.bottom - rect.top);
		  retval = w;
	  }
	  else if (aux::streq(name, "showmin")){
		  /*PBYTE pb; DWORD cb;
		  if (load_resource_data(L"MINDEFAULT", pb, cb))
		  {
			  HTMLayoutLoadHtml(hwnd, pb, cb);
		  }*/
		  
		  topdlg mtopdlg(hwnd, WS_SIZEBOX);
		  mtopdlg.show(IDR_ROOT_TEST);

// 		  BOOL done = TRUE;
// 		  WIN32_FIND_DATAA fd;
// 		  char dirpath[255];
// 		  sprintf(dirpath, "%s/*.db",__DIR__);
// 		  HANDLE hFind = FindFirstFileA(dirpath, &fd);//第一个参数是路径名，可以使用通配符，懂DOS的人应该知道吧！fd存储有文件的信息
// 		  std::string ret = "";
// 		  while (done)
// 		  {
// 			  OutputDebugStringA(fd.cFileName);
// 			  ret = ret + fd.cFileName;
// 			  done = FindNextFileA(hFind, &fd);//返回的值如果为0则没有文件要寻了
// 		  }
// 		  retval = ret.c_str();

		  //MessageBoxA(NULL, ret.c_str(), "1", 0);
	  }
	  
	  return true;
  }
};


}



#endif





