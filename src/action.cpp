#pragma once
#include "stdafx.h"

#include "sql.hpp"
#include "debug.h"
#include "dlg.h"
#include "resource.h"

namespace XCALL_ACTION
{
	 

	void onDocumentCompele()
	{
		
		
	}

	void inline show_root_list(HELEMENT he,json::value id,json::value db)
	{
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		PSQL->createTable();
		Record * precode;
		PSQL->query("SELECT * FROM root order by id desc");
		
		while ((precode = PSQL->RESCULT()->getone()))
		{
			showDebug(1);
		}
	}

	void inline root_edit(HELEMENT he, HWND hwnd, json::value id, json::value title)
	{
		std::string sval = std::string(aux::w2a(title.to_string().c_str()));
		sql * PSQL = &sql::getInstance();
		PSQL->connect("db");
		std::string sql = "INSERT INTO root values(NULL,'" + sval + "') ";
		
		PSQL->query(sql);

	}

	void inline show_root_edit_box(HELEMENT he, json::value id, HWND hwnd)
	{
		//::htmlayout::window * a = reinterpret_cast<::htmlayout::window *>(lp);
		//htmlayout::dom::element btn = he;

		dlg dlg1(hwnd);

		dlg1.show(IDR_ROOT_EDIT);
	}

}
