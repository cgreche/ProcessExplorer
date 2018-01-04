
//Menu
// File name: menu.h
// Last edit: 01/08/2014 01:51 (UTC-3)
// Author: CGR

#ifndef __MENU_H__
#define __MENU_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#include <list>

namespace uilib {

	class MenuBar;
	class MenuObject;
	class Menu;
	class MenuItem;
	class MenuSeparator;

	typedef void(*MenuItemFunc)(MenuItem &item);
	typedef std::list<MenuObject*> MenuObjectList;


	class MenuObject : protected UIObj
	{

	public:
		MenuObject();
		MenuObject(OSDMenuObject& osdRef);

		friend class OSDMenuObject;
		OSDMenuObject& osdRef() const { return reinterpret_cast<OSDMenuObject&>(UIObj::osdRef()); }
	};




	class MenuItem : public MenuObject
	{

	protected:
		string m_text;
		MenuItemFunc m_onClick;
		void *m_param;
		Menu* m_parent;

	public:
		friend class Menu;
		MenuItem();

		void setText(const string &text);
		void setOnClick(MenuItemFunc onClick) { m_onClick = onClick; }
		void setParam(void *param);

		const string &text() const { return m_text; }
		void *param() const { return m_param; }
		Menu *parent() const { return m_parent; }
		//	MenuBar* parentMenuBar() const;
		//	Control* parentControl() const;

		friend class OSDMenuItem;
		OSDMenuItem& osdRef() const { return reinterpret_cast<OSDMenuItem&>(MenuObject::osdRef()); }
	};


	class MenuSeparator : public MenuObject
	{

	public:

		MenuSeparator();

		friend class OSDMenuSeparator;
		OSDMenuSeparator& osdRef() const { return reinterpret_cast<OSDMenuSeparator&>(MenuObject::osdRef()); }
	};



	class Menu : public MenuObject
	{

	protected:
		string m_text;
		MenuObjectList m_object;

	public:
		Menu();

		void setText(const string &text);
		void append(Menu &submenu);
		void append(MenuSeparator &separator);
		void append(MenuItem &item);

		const string &text() const { return m_text; }

		bool contains(MenuObject &object);

		friend class OSDMenu;
		OSDMenu& osdRef() const { return reinterpret_cast<OSDMenu&>(MenuObject::osdRef()); }
	};


	class MenuBar : public UIObj
	{

	protected:
		MenuObjectList m_object;
		bool m_visible;
		Control *m_parent;

	public:
		MenuBar();

		void setParent(Control& control);
		void setVisible(bool visible);
		bool visible() const { return m_object.size() > 0 && m_visible; }

		void append(Menu &menu);
		void append(MenuItem &item);

		bool contains(MenuObject &object);

		Control* parent() const { return m_parent; }

		OSDMenuBar& osdRef() const { return reinterpret_cast<OSDMenuBar&>(UIObj::osdRef()); }
	};

}

#endif