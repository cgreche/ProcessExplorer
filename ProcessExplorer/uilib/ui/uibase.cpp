
//UIBase
//File: uibase.cpp
//Last edit: 25/10/2017 05:32 (UTC-3)
//Author: CGR

#include "uibase.h"

namespace uilib {

	FontDesc g_pUIdefFontDesc;

	bool UI::m_uiinit(false);

	void UI::UIInit() //static
	{
		if (m_uiinit) return;
		OSDBase::UIInit();
		UI::m_uiinit = true;
		g_pUIdefFontDesc = FontDesc("Arial", 10, 0);
	}

	int UI::UIProcess() //static 
	{
		return OSDBase::UIProcess();
	}

	void UI::UIQuit() //static
	{
		OSDBase::UIQuit();
	}

}