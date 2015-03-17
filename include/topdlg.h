
#ifndef dlg_topdlg__xxxx
#define dlg_topdlg__xxxx
#include "stdafx.h"


class topdlg : public htmlayout::dialog
{
public:
	topdlg(HWND hwnd, int style = 0) : htmlayout::dialog(hwnd, style)
	{
		alignment = 2;
		style = 0;
		style_ex = 0;

	}

	inline unsigned int show(UINT html_res_id)
	{

		PBYTE   html;
		DWORD   html_length;
		if (!htmlayout::load_html_resource(html_res_id, html, html_length))
		{
			assert(false); // resource not found!
			return 0;
		}
		return show(html, html_length);
	}

	// show HTML dialog from html in memory buffer
	inline unsigned int show(LPCBYTE html, UINT html_length)
	{
		this->html = html;
		this->html_length = html_length;
		HWND hwnd = create_window();
		if (pvalues)
		{
			htmlayout::dom::element root = htmlayout::dom::element::root_element(hwnd);
			htmlayout::set_values(root, *pvalues);
		}
		ShowWindow(hwnd, SW_SHOW);
		do_loop(hwnd);
		return return_id;
	}

	inline void do_loop(HWND hwnd)
	{
#if defined(UNDER_CE)
		HWND frm = GetWindow(hwnd, GW_OWNER);
#else
		HWND frm = GetAncestor(hwnd, GA_ROOTOWNER);
#endif
		MSG msg;
		while (::IsWindow(hwnd) && GetMessage(&msg, NULL, 0, 0))
		{
			htmlayout::queue::execute();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}
	inline INT_PTR CALLBACK DialogProc
		(
		HWND hwndDlg,   // handle to dialog box
		UINT uMsg,      // message
		WPARAM wParam,  // first message parameter
		LPARAM lParam) // second message parameter
	{
		BOOL handled = false;
		LRESULT lr = HTMLayoutProcND(hwndDlg, uMsg, wParam, lParam, &handled);
		if (handled)
		{
		HANDLED:
#if defined(_WIN64)
			SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, lr);
#else
			SetWindowLong(hwndDlg, DWL_MSGRESULT, lr);
#endif
			return TRUE;
		}

		switch (uMsg)
		{
			case WM_NCCALCSIZE:	 // we have no non-client areas.
			case WM_NCPAINT:      // we have no non-client areas.
			case WM_NCACTIVATE:  return (wParam == 0) ? TRUE : FALSE; // we have no non-client areas.
		case WM_INITDIALOG:   lr = on_WM_INITDIALOG(hwndDlg, wParam, lParam); goto HANDLED;
		case WM_CLOSE:        lr = on_WM_CLOSE(hwndDlg, wParam, lParam); goto HANDLED;
		case WM_DESTROY:      lr = on_WM_DESTROY(hwndDlg, wParam, lParam); goto HANDLED;
		case WM_USER:         lr = on_WM_USER(hwndDlg, wParam, lParam); goto HANDLED;
		}
		return FALSE;
	}
	//void show(int resID);            
	// BOOL handle_key (HELEMENT he, KEY_PARAMS& params ); 
	/* unsigned int show( UINT html_res_id ); */
	virtual BOOL on_script_call(HELEMENT he, LPCSTR name, UINT argc, json::value* argv, json::value& retval)
	{
		if (aux::streq(name, "alert")){
			MessageBox(hwnd, argv[0].to_string().c_str(), TEXT("alert!"), MB_OK);
		}
		return true;
	}



}
;

#endif
