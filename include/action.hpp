#pragma once
#include "stdafx.h"

#include "sql.hpp"
#include "debug.h"
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

	void inline root_add()
	{

	}

	void inline root_edit_box_show()
	{
		htmlayout::dialog dlg(NULL);
		dlg.show(IDR_ROOT_EDIT);
	}

}