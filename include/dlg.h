
#ifndef dlg___xxxx
#define dlg___xxxx
#include "sql.hpp"
#include "debug.h"
#include "dlg.h"
#include "resource.h"

namespace XCALL_ACTION
{
	void inline root_edit(HELEMENT he, HWND hwnd, json::value id, json::value title)
	{
		std::string sval = std::string(aux::w2a(title.to_string().c_str()));
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		htmlayout::dom::element btn = he;
		
		//std::wstring w_id = id.to_string().c_str();
		//w_id = btn.get_attribute("value");

		std::string sql = "INSERT INTO root values(NULL,'" + sval + "') ";
		PSQL->query(sql);
		::PostMessage(hwnd, WM_CLOSE, 0, 0);
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
	}

	void inline main_edit(HELEMENT he, HWND hwnd, json::value id, json::value title, json::value mean, json::value content )
	{
		std::string m_id = std::string(aux::w2a(id.to_string().c_str()));
		std::string m_title = std::string(aux::w2a(title.to_string().c_str()));
		std::string m_mean = std::string(aux::w2a(mean.to_string().c_str()));
		std::string m_content = std::string(aux::w2a(content.to_string().c_str()));

		htmlayout::dom::element root = $D(he).root_element(hwnd);
		htmlayout::dom::element richtext = root.find_first("richtext");

		//showDebug(m_id.c_str());
		//showDebug(m_content.c_str());
		std::string mcon = "";
		if (!richtext.is_valid()){
			//MessageBoxA(NULL, "1", "1", 0);
			//main_edit($1(input[name=title]):value,$1(input[name=mean]):value,$1(richtext).saveHTML("c:/","emit-all-images"));
			//OutputDebugString((richtext.xcall("saveHTML", "c:/", "emit-all-images")).to_string().c_str());
			MessageBoxA(NULL, "richtext error", "error", 0);
			WStringToString((richtext.xcall("saveHTML", "c:/", "emit-all-images")).to_string().c_str(), mcon);
			return;
		}
		
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		std::string sql = "INSERT INTO main (id,root_id,title,mean,content,create_time) values(NULL,1,'" + m_title + "','" + m_mean + "','" + mcon + "',1) ";
		PSQL->query(sql);
// 		sql * PSQL = &sql::getInstance();
// 		PSQL->connect("db");
// 		std::string sql = "INSERT INTO root values(NULL,'" + sval + "') ";
// 		PSQL->query(sql);
		::PostMessage(hwnd, WM_CLOSE, 0, 0);
	}
	
}
#include "stdafx.h"
class dlg : public htmlayout::dialog
{
    public :
        dlg(HWND hwnd) : htmlayout::dialog(hwnd)
        {
            //MessageBox(hwnd,"1","1",MB_OK);
            //this->hwnd = hwnd;
        }
        //void show(int resID);            
        // BOOL handle_key (HELEMENT he, KEY_PARAMS& params ); 
        /* unsigned int show( UINT html_res_id ); */

		virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval)
		{
			if (aux::streq(name, "alert")){
				MessageBox(hwnd, argv[0].to_string().c_str(), TEXT("alert!"), MB_OK);
			}
			else if (aux::streq(name, "root_edit"))
			{
				//JSON_VALUE id = argv[1];
				//JSON_VALUE title = argv[0];
				XCALL_ACTION::root_edit(he, hwnd, argv[1], argv[0]);
			}
			else if (aux::streq(name, "main_edit"))
			{
				//JSON_VALUE id = argv[0];
				//JSON_VALUE title = argv[1];
				//JSON_VALUE mean = argv[2];
				//JSON_VALUE content = argv[3];
				XCALL_ACTION::main_edit(he, hwnd, argv[0], argv[1], argv[2], argv[3]);
			}
			return true;
		}


		
}
;

#endif
