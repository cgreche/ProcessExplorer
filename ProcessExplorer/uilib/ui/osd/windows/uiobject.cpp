
#include "..\\..\\uibase.h"

namespace uilib {


	OSDUIObj::OSDUIObj(UIObj &ref) :m_ref(ref)
	{
		UI::UIInit();
		static u32 id = 0;
		m_id = id++;
		OSDBase::objects->push_back(this);
	}

	OSDUIObj::~OSDUIObj()
	{

	}

}