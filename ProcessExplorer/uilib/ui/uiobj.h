
//UI Object
//File: uiobj.h
//Last edit: 30/09/2013 11:44 (UTC-3)
//Author: CGR

#ifndef __UIOBJ_H__
#define __UIOBJ_H__

namespace uilib {

	class UIObj
	{
		OSDUIObj &m_osdRef;

		virtual void update();

	public:

		UIObj();
		UIObj(OSDUIObj& ref);
		virtual ~UIObj();

		OSDUIObj& osdRef() const;
	};

}

#endif

