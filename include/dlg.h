
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
		std::string sql = "INSERT INTO root values(NULL,'" + sval + "') ";
		PSQL->query(sql);
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
				XCALL_ACTION::root_edit(he, hwnd,argv[1] , argv[0]);
			}
			return true;
		}
}
;

#endif
