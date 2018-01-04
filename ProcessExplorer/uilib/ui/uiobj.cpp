
//UI OBject
// File: uiobj.h
// Last edit: 25/10/2017 06:28 (UTC-3)
// Author: CGR

#include "uibase.h"

namespace uilib {

	UIObj::UIObj() : m_osdRef(*new OSDUIObj(*this))
	{
		UI::UIInit(); //ensure it is init
		osdRef().create();
	}

	UIObj::UIObj(OSDUIObj& osdRef) : m_osdRef(osdRef)
	{
		UI::UIInit(); //ensure it is init
	}

	UIObj::~UIObj()
	{
		osdRef().destroy();
		delete &m_osdRef;
	}

	void UIObj::update()
	{
		osdRef().update();
	}

	OSDUIObj& UIObj::osdRef() const
	{
		return m_osdRef;
	}

}