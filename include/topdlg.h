
#ifndef dlg_topdlg__xxxx
#define dlg_topdlg__xxxx
#include "stdafx.h"


class topdlg : public htmlayout::dialog
{
private:
	htmlayout::dom::element  body;
	htmlayout::dom::element  caption;
	htmlayout::dom::element  button_min;
	htmlayout::dom::element  button_max;
	htmlayout::dom::element  button_icon;
	htmlayout::dom::element  button_close;
	htmlayout::dom::element  corner;

public:
	static  topdlg* self(HWND hWnd) { return (topdlg*)::GetWindowLongPtr(hWnd, GWLP_USERDATA); }
	static  void        self(HWND hWnd, topdlg* inst) { ::SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(inst)); }

	topdlg(HWND hwnd, int style = 0) : htmlayout::dialog(hwnd, style)
	{
		alignment = 2;
// 		style = WS_POPUP;
// 		style_ex = 0;
	}
	inline bool is_minimized() const
	{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(hwnd, &wp);
		return wp.showCmd == SW_SHOWMINIMIZED;
	}

	inline bool is_maximized() const
	{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(hwnd, &wp);
		return wp.showCmd == SW_SHOWMAXIMIZED;
	}

	inline int       hit_test(int x, int y)
	{
		//char str[250];
		//sprintf(str, "x=%d,y=%d", x,y);
		//OutputDebugStringA(str);

		POINT pt; pt.x = x; pt.y = y;
		::MapWindowPoints(HWND_DESKTOP, hwnd, &pt, 1);
		
		if (caption.is_valid() && caption.is_inside(pt))
			//OutputDebugStringA(str);
			return HTCAPTION;

		if (button_icon.is_valid() && button_icon.is_inside(pt))
			return HTSYSMENU;

		if (corner.is_valid() && corner.is_inside(pt))
			return HTBOTTOMRIGHT;

		RECT body_rc = body.get_location(ROOT_RELATIVE | CONTENT_BOX);
		
		if (pt.y < body_rc.top + 10)
		{
			if (pt.x < body_rc.left + 10)
				return HTTOPLEFT;
			if (pt.x > body_rc.right - 10)
				return HTTOPRIGHT;
		}
		else if (pt.y > body_rc.bottom - 10)
		{
			if (pt.x < body_rc.left + 10)
				return HTBOTTOMLEFT;
			if (pt.x > body_rc.right - 10)
				return HTBOTTOMRIGHT;
		}

		if (pt.y < 10)
			return HTTOP;
		if (pt.y+10 > body_rc.bottom)
			return HTBOTTOM;
		if (pt.x < 10)
			return HTLEFT;
		if (pt.x+10 > body_rc.right)
			return HTRIGHT;

		if (PtInRect(&body_rc, pt))
			return HTCLIENT;

		return HTCLIENT;

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
		//topdlg * m_dialog = new topdlg(parent);
		

		this->html = html;
		//std::string b((char *)html);
		//MessageBoxA(NULL, b.c_str(), "1", 0);
// 		std::string css = "<div id=\"topbar\">\
// 			<widget id = \"icon\" type = \"button\">< / widget>\
// 			<div id = \"caption\">< / div>\
// 			<widget id = \"minimize\" type = \"button\">0 < / widget >\
// 			<widget id = \"maximize\" type = \"button\">1 < / widget >\
// 			<widget id = \"close\" type = \"button\">r< / widget>\
// 												< / div><style>html{background:#000}#topbar widget[type=\"button\"]{width:1em;height:1em;background-image:none;padding:1px;font-family:marlett;font-size:10pt;color:#FFF;border:1px solid #FFF;text-align:center;vertical-align:center;margin:0;}#topbar widget[type=\"button\"]:hover{//color:orange;//border-color:orange;//background-color:#B0CFD1;//outline:2px glow orange 4px;}#topbar widget[type=\"button\"]:active{background-color:#274749;}#topbar #minimize,#topbar #maximize{margin-right:2px;}#topbar #icon{foreground-image:url(back.png);foreground-repeat:no-repeat;foreground-position:50% 50%;}#topbar{flow:horizontal;padding:4px 10px 5px;}#topbar #caption{//color:#274749;color:#FFF;padding:0 4px;outline:3px glow #274749;}#bottombar{flow:horizontal;}< / style>";
// 		b = b + css;
// 		html = (LPCBYTE)b.c_str();
// 		this->html = html;
// 		this->html_length = b.size();

		this->html_length = html_length;
		HWND hwnd = create_window();
		htmlayout::dom::element root = htmlayout::dom::element::root_element(hwnd);
		if (pvalues)
		{
			htmlayout::set_values(root, *pvalues);
		}
		this->body = root.find_first("body");
		this->caption = root.get_element_by_id("caption");
		this->button_min = root.get_element_by_id("minimize");
		this->button_max = root.get_element_by_id("maximize");
		this->button_icon = root.get_element_by_id("icon");
		this->button_close = root.get_element_by_id("close");
		this->corner = root.get_element_by_id("corner");
		
		self(this->hwnd, this);

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


	virtual inline BOOL handle_event(HELEMENT he, BEHAVIOR_EVENT_PARAMS& params)
	{

		htmlayout::dom::element target = params.heTarget;
			//::MessageBoxW(button.get_element_hwnd(true) ,button.get_attribute("id"), L"Click on the button with id:", MB_OK);
			//::MessageBox(button.get_element_hwnd(true) ,"test", "Click on the button with id:", MB_OK);
		if (params.cmd == BUTTON_CLICK)
		{
			if (target == button_min)
			{
				::ShowWindow(hwnd, SW_MINIMIZE);
				return TRUE;
			}
			if (target == button_max)
			{
				if (is_maximized())
					::ShowWindow(hwnd, SW_RESTORE);
				else
					::ShowWindow(hwnd, SW_MAXIMIZE);

				return TRUE;
			}
			if (target == button_close)
			{
				::PostMessage(hwnd, WM_CLOSE, 0, 0);
				return TRUE;
			}
		}
			return htmlayout::dialog::handle_event(he,params);
		
	}

	inline HWND create_window()
	{
		struct zDLGTEMPLATE : DLGTEMPLATE
		{
			WORD strings[3];
		};

		zDLGTEMPLATE dt;
		memset(&dt, 0, sizeof(dt));

		dt.style = style;
		dt.dwExtendedStyle = style_ex;
		dt.cdit = 0;

		HINSTANCE hinstance =
#if defined(UNDER_CE)
			(HINSTANCE)::GetModuleHandle(NULL);
#else
			(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE);
#endif

		hwnd = CreateDialogIndirectParam(
			hinstance,                        // handle to module 
			&dt,                              // dialog box template
			parent,                           // handle to owner window
			&DialogProc,                      // dialog box procedure
			LPARAM(this)                      // initialization value
			);

		return hwnd;
	}
	static inline INT_PTR CALLBACK DialogProc
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

		topdlg* ctl = self(hwndDlg);
		
		switch (uMsg)
		{
			case WM_NCHITTEST: 
				lr = ctl->hit_test(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				char l[50];
				sprintf(l,"%d",lr);
				OutputDebugStringA(l);
				goto HANDLED;
				//return ctl->hit_test(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			case WM_NCCALCSIZE:	 goto HANDLED;// we have no non-client areas.
			case WM_NCPAINT:  return 0;    // we have no non-client areas.
			case WM_NCACTIVATE:  return (wParam == 0) ? TRUE : FALSE; // we have no non-client areas.
			case WM_INITDIALOG:   lr = on_WM_INITDIALOG(hwndDlg, wParam, lParam); goto HANDLED;
			case WM_CLOSE:        lr = on_WM_CLOSE(hwndDlg, wParam, lParam); goto HANDLED;
			case WM_DESTROY:      lr = on_WM_DESTROY(hwndDlg, wParam, lParam); goto HANDLED;
			//case WM_USER:         lr = on_WM_USER(hwndDlg, wParam, lParam); goto HANDLED;

			case WM_GETMINMAXINFO:
			{
									 LRESULT lr = DefWindowProcW(hwndDlg, uMsg, wParam, lParam);
									 MINMAXINFO* pmmi = (MINMAXINFO*)lParam;
									 pmmi->ptMinTrackSize.x = ::HTMLayoutGetMinWidth(hwndDlg);
									 RECT rc; GetWindowRect(hwndDlg, &rc);
									 pmmi->ptMinTrackSize.y = ::HTMLayoutGetMinHeight(hwndDlg, rc.right - rc.left);
									 return lr;
			}
		}
		return FALSE;
	}

// 	static inline INT_PTR on_WM_INITDIALOG(HWND hwndDlg, WPARAM wParam, LPARAM lParam)
// 	{
// 		topdlg* ctl = static_cast<topdlg *>((void*)lParam);
// #if defined(UNDER_CE)
// 		SetWindowLong(hwndDlg, DWL_USER, LONG(ctl));
// #else
// 		SetWindowLongPtr(hwndDlg, DWLP_USER, LONG_PTR(ctl));
// #endif
// 		ctl->hwnd = hwndDlg;
// 
// 		ctl->setup_callback(hwndDlg);
// 		attach_event_handler(hwndDlg, ctl);
// 
// 		if (!ctl->html)
// 			return FALSE;
// 		if (ctl->html_length == 0) // this is a file name
// 			HTMLayoutLoadFile(hwndDlg, (LPCWSTR)ctl->html);
// 		else
// 			HTMLayoutLoadHtmlEx(hwndDlg, ctl->html, ctl->html_length, ctl->base_url);
// 
// 		htmlayout::dom::element root = htmlayout::dom::element::root_element(hwndDlg);
// 		if (!root.is_valid())
// 			return FALSE;
// 
// 		// set dialog caption
// 		htmlayout::dom::element title = root.find_first("title");
// 		if (title.is_valid())
// 			::SetWindowText(hwndDlg, w2t(title.text().c_str()));
// 
// 		SIZE sz;
// 		htmlayout::dom::element isMaxsz = root.find_first("#maxsz");
// 		if (isMaxsz.is_valid()){
// 			RECT rct;
// 			::GetWindowRect(ctl->parent, &rct);
// 			sz.cx = rct.right - rct.left;
// 			sz.cy = rct.bottom - rct.top;		//È«ÆÁÄ»µ¯´°µÄÐÞ¸Ä
// 		}
// 		else{
// 			sz.cx = HTMLayoutGetMinWidth(hwndDlg);
// 			sz.cy = HTMLayoutGetMinHeight(hwndDlg, sz.cx);
// 		}
// 
// 
// 
// 		RECT rc; rc.left = ctl->position.x;
// 		rc.top = ctl->position.y;
// 		rc.right = rc.left + sz.cx;
// 		rc.bottom = rc.top + sz.cy;
// 
// 		AdjustWindowRectEx(
// 			&rc,
// 			GetWindowLong(hwndDlg, GWL_STYLE),
// 			FALSE,
// 			GetWindowLong(hwndDlg, GWL_EXSTYLE));
// 
// 		sz.cx = rc.right - rc.left;
// 		sz.cy = rc.bottom - rc.top;
// 
// 		if (ctl->alignment == 1)
// 		{
// 			RECT prc;
// 			::GetClientRect(::GetDesktopWindow(), &prc);
// 			rc.left = (prc.right - prc.left - sz.cx) / 2 + prc.left;
// 			rc.top = (prc.bottom - prc.top - sz.cy) / 2 + prc.top;
// 			rc.right = rc.left + sz.cx;
// 			rc.bottom = rc.top + sz.cy;
// 
// 		}
// 		else if (ctl->alignment == 2)
// 		{
// 			RECT prc;
// 			::GetWindowRect(ctl->parent, &prc);
// 			rc.left = (prc.right - prc.left - sz.cx) / 2 + prc.left;
// 			rc.top = (prc.bottom - prc.top - sz.cy) / 2 + prc.top;
// 			rc.right = rc.left + sz.cx;
// 			rc.bottom = rc.top + sz.cy;
// 		}
// 		UINT MOVE_FLAGS = (SWP_NOZORDER);
// 		::SetWindowPos(hwndDlg, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, MOVE_FLAGS);
// 		::PostMessage(hwndDlg, WM_USER, 0, 0);
// 
// 		return 0;
// 	}

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
