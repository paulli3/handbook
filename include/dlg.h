
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
			alignment = 2;
        }
		
        //void show(int resID);            
        // BOOL handle_key (HELEMENT he, KEY_PARAMS& params ); 
        /* unsigned int show( UINT html_res_id ); */
		virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval)
		{
			if (aux::streq(name, "alert")){
				MessageBox(hwnd, argv[0].to_string().c_str(), TEXT("alert!"), MB_OK);
			}
			else if (aux::streq(name, "resize"))
			{
				//JSON_VALUE id = argv[1];
				//JSON_VALUE title = argv[0];
				//XCALL_ACTION::root_edit(he, hwnd, argv[1], argv[0]);
				//char x[10]; char y[10]; char w[10]; char h[10];
				//sscanf(aux::w2a(argv[0].to_string().c_str()),"%s %s %s %s",x,y,w,h); //用空格，不能用其他符号，不然会贪婪匹配
				//RECT rc;
				//GetWindowRect(this->parent,&rc);
// 				rc.right = atoi (w);
// 				rc.bottom = atoi(h);
				//MoveWindow(hwnd, rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,true);
				//return true;
			}
			return true;
		}


		
}
;

#endif
