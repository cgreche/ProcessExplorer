
//OSDComboBox
// File: osdcombobox.cpp
// Last edit: 06/10/2017 04:39 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDComboBox::OSDComboBox(ComboBox &ref) : OSDControl(ref)
	{

	}

	void OSDComboBox::create()
	{
		createWindow(*this, TEXT("combobox"), 0, WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST);
	}

	void OSDComboBox::update()
	{
		OSDControl::update();
		for (std::vector<ComboBoxItem*>::iterator it = ref().m_items.begin(); it != ref().m_items.end(); ++it)
			insertItem(**it);
		setSelection(ref().selection());
	}



	u32 OSDComboBox::insertItem(ComboBoxItem &item)
	{
		u32 index = ::SendMessage(m_hwnd, CB_ADDSTRING, (WPARAM)0, (LPARAM)item.text().data());
		if (index == CB_ERR || index == CB_ERRSPACE)
			return -1;

		::SendMessage(m_hwnd, CB_SETITEMDATA, (WPARAM)index, (LPARAM)&item);
		return index;
	}

	void OSDComboBox::removeItem(u32 index)
	{
		::SendMessage(m_hwnd, CB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}

	void OSDComboBox::reset()
	{
		::SendMessage(m_hwnd, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	}

	void OSDComboBox::sort()
	{
	}

	int OSDComboBox::setSelection(int row)
	{
		u32 lResult = ::SendMessage(m_hwnd, CB_SETCURSEL, (WPARAM)row, (LPARAM)0);
		if (lResult == CB_ERR)
			return 0xffffffff;
		if (ref().m_onSelect)
			ref().m_onSelect(ref(), ref().item(lResult));
		return lResult; //returns the selected item
	}

	Size OSDComboBox::getAutoSize()
	{
		Size textSize;
		RECT rc = { 0,0,0,0 };
		HDC hdc = ::GetDC(m_hwnd);
		::SelectObject(hdc, ref().font().osdRef().object());
		::DrawText(hdc, " ", -1, &rc, DT_CALCRECT);
		//	GetTextExtentPoint32(hdc,ref.m_text(),ref.m_text.len(),&size);
		::ReleaseDC(m_hwnd, hdc);
		textSize = Size(rc.right, rc.bottom);

		RECT rect = { 0,0,0,0 };
		GetClientRect(m_hwnd, &rect);
		RECT windowRect;
		::GetWindowRect(m_hwnd, &windowRect);

		//method 1:
	//	u32 itemHeight = ::SendMessage(m_hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0);
	//	return Size(0xffffffff,itemHeight+6);
		//Where did I get this constant 6?
		//Well, if I resize the control with any width and a height of 1,
		//then get its size through GetWindowRect and the item height through CB_GETITEMHEIGHT,
		//the control size will have a difference of 6 to the item height
		//PS: control must have a parent Window;


		//method 2:
		//clientrect - textsize = difference of 8
		return Size(100, textSize.height() + 8);
	}

	void OSDComboBox::setSize(const Size &size)
	{
		::SetWindowPos(m_hwnd, NULL, 0, 0, size.width(), 1, SWP_NOMOVE | SWP_NOZORDER);
		RECT rc;
		::GetWindowRect(m_hwnd, &rc);
		u32 windowHeight = (rc.bottom - rc.top);
		u32 itemHeight = ::SendMessage(m_hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0);
		u32 borderHeight = windowHeight - itemHeight;
		u32 adjustedHeight = size.height() - ((rc.bottom - rc.top) - ::SendMessage(m_hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
		::SendMessage(m_hwnd, CB_SETITEMHEIGHT, (WPARAM)-1, adjustedHeight);
	}



	LRESULT OSDComboBox::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_COMMAND) {
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				u32 cursel = ::SendMessage((HWND)lParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (cursel != CB_ERR) {
					ComboBoxItem *item = (ComboBoxItem*)::SendMessage((HWND)lParam, CB_GETITEMDATA, (WPARAM)cursel, (LPARAM)0);
					ref().m_selection = cursel;
					if (ref().m_onSelect)
						ref().m_onSelect(ref(), item);
				}
			}
		}

		/* From MSDN:
		Return value
			The return value is ignored except for notification messages that specify otherwise.
		*/
		return 0;
	}

}