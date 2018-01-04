
//OSDMenu
// File name: osdmenu.cpp
// Last edit: 09/11/2013 01:30 (UTC-3)
// Author: CGR


#include "..\\..\\uibase.h"

namespace uilib {

	//
	// OSDMenuBar
	////

	void OSDMenuBar::create()
	{
		m_hMenu = ::CreateMenu();
	}

	void OSDMenuBar::destroy()
	{
		if (m_hMenu) {
			::DestroyMenu(m_hMenu);
			m_hMenu = 0;
		}
	}

	void OSDMenuBar::setParent(Control& control)
	{
		if (ref.visible())
			::SetMenu(control.osdRef().handle(), m_hMenu);
	}

	void OSDMenuBar::setVisible(bool visible)
	{
		if (ref.parent())
			::SetMenu(ref.parent()->osdRef().handle(), visible ? m_hMenu : NULL);
	}

	void OSDMenuBar::append(Menu& menu)
	{
		u32 itemCount = GetMenuItemCount(m_hMenu);
		::AppendMenu(m_hMenu, MF_POPUP | MF_STRING, (UINT)menu.osdRef().m_hMenu, menu.text().data());
		if (itemCount == 0 && ref.parent())
			::DrawMenuBar(ref.parent()->osdRef().handle());
	}

	void OSDMenuBar::append(MenuItem &item)
	{
		u32 itemCount = GetMenuItemCount(m_hMenu);
		::AppendMenu(m_hMenu, MF_POPUP | MF_STRING, (UINT)item.osdRef().id(), item.text().data());
		if (itemCount == 0 && ref.parent())
			::DrawMenuBar(ref.parent()->osdRef().handle());
	}




	//
	// OSDMenu
	////

	void OSDMenu::create()
	{
		m_hMenu = ::CreatePopupMenu();
	}

	void OSDMenu::destroy()
	{
		if (m_hMenu) {
			::DestroyMenu(m_hMenu);
			m_hMenu = 0;
		}
	}

	void OSDMenu::setText(const string &text)
	{

	}

	void OSDMenu::append(Menu& submenu)
	{
		::AppendMenu(m_hMenu, MF_POPUP | MF_STRING, (UINT)submenu.osdRef().m_hMenu, submenu.text().data());
	}

	void OSDMenu::append(MenuSeparator& separator)
	{
		::AppendMenu(m_hMenu, MF_SEPARATOR, (UINT)separator.osdRef().id(), NULL);
	}

	void OSDMenu::append(MenuItem &item)
	{
		::AppendMenu(m_hMenu, MF_POPUP | MF_STRING, (UINT)item.osdRef().id(), item.text().data());
	}



	//
	// OSDMenuItem
	////

	void OSDMenuItem::setText(const string &text)
	{
	}

	LRESULT OSDMenuItem::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_COMMAND) {
			if (ref.m_onClick)
				ref.m_onClick(ref);
		}
		return 0;
	}

}