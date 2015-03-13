
#ifndef dlg___xxxx
#define dlg___xxxx
#include "sql.hpp"
#include "debug.h"
#include "dlg.h"
#include "resource.h"

#include "stdafx.h"
class dlg : public htmlayout::dialog
{
    public :
        dlg(HWND hwnd,int style=0) : htmlayout::dialog(hwnd,style)
        {
            //MessageBox(hwnd,"1","1",MB_OK);
            //this->hwnd = hwnd;
        }
		
        //void show(int resID);            
        // BOOL handle_key (HELEMENT he, KEY_PARAMS& params ); 
        /* unsigned int show( UINT html_res_id ); */

		//virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval)
		//{
		//	if (aux::streq(name, "alert")){
		//		MessageBox(hwnd, argv[0].to_string().c_str(), TEXT("alert!"), MB_OK);
		//	}
		//	else if (aux::streq(name, "root_edit"))
		//	{
		//		//JSON_VALUE id = argv[1];
		//		//JSON_VALUE title = argv[0];
		//		XCALL_ACTION::root_edit(he, hwnd, argv[1], argv[0]);
		//	}
		//	else if (aux::streq(name, "main_edit"))
		//	{
		//		//JSON_VALUE id = argv[0];
		//		//JSON_VALUE title = argv[1];
		//		//JSON_VALUE mean = argv[2];
		//		//JSON_VALUE content = argv[3];
		//		XCALL_ACTION::main_edit(he, hwnd, argv[0], argv[1], argv[2], argv[3]);
		//	}
		//	return true;
		//}


		
}
;

#endif
