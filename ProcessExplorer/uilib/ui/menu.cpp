
//Menu
// File name: menu.cpp
// Last edit: 10/11/2013 02:25 (UTC-3)
// Author: CGR

#include "uibase.h"

namespace uilib {

	//Menu Object
	MenuObject::MenuObject() :UIObj(*new OSDMenuObject(*this))
	{
		osdRef().create();
	}

	MenuObject::MenuObject(OSDMenuObject& osdRef) : UIObj(osdRef)
	{
	}



	//Menu Item

	MenuItem::MenuItem() : MenuObject(*new OSDMenuItem(*this))
	{
		m_onClick = NULL;
		osdRef().create();
	}

	void MenuItem::setText(const string &text)
	{
		m_text = text;
		osdRef().setText(text);
	}

	void MenuItem::setParam(void* param)
	{
		m_param = param;
	}




	//Menu Separator

	MenuSeparator::MenuSeparator() : MenuObject(*new OSDMenuSeparator(*this))
	{
		osdRef().create();
	}



	//Menu

	Menu::Menu() : MenuObject(*new OSDMenu(*this))
	{
		osdRef().create();
	}

	void Menu::setText(const string &text)
	{
		m_text = text;
		osdRef().setText(text);
	}

	void Menu::append(Menu &submenu)
	{
		if (contains(submenu))
			return;

		m_object.push_back(&submenu);
		osdRef().append(submenu);
	}

	void Menu::append(MenuSeparator &separator)
	{
		m_object.push_back(&separator);
		osdRef().append(separator);
	}

	void Menu::append(MenuItem &item)
	{
		if (contains(item))
			return;

		m_object.push_back(&item);
		osdRef().append(item);
	}

	bool Menu::contains(MenuObject &object)
	{
		MenuObjectList::iterator it;
		for (it = m_object.begin(); it != m_object.end(); ++it) {
			if (*it == &object)
				return true;
		}

		return false;
	}





	//
	//
	//// MENU


	MenuBar::MenuBar() :UIObj(*new OSDMenuBar(*this))
	{
		m_parent = NULL;
		m_visible = false;
		osdRef().create();
	}

	void MenuBar::setParent(Control& control)
	{
		//Don't set a MenuBar to a child control
		if (control.parent() != NULL)
			return;

		m_parent = &control;
		osdRef().setParent(control);
	}

	void MenuBar::setVisible(bool visible)
	{
		m_visible = visible;
		osdRef().setVisible(visible);
	}

	void MenuBar::append(Menu &menu)
	{
		if (contains(menu))
			return;

		m_object.push_back(&menu);
		osdRef().append(menu);
	}

	void MenuBar::append(MenuItem &item)
	{
		if (contains(item))
			return;

		m_object.push_back(&item);
		osdRef().append(item);
	}

	bool MenuBar::contains(MenuObject &object)
	{
		MenuObjectList::iterator it;
		for (it = m_object.begin(); it != m_object.end(); ++it) {
			if (*it == &object)
				return true;
		}

		return false;
	}

}