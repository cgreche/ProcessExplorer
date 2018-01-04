
//OSDEdit
// File: osdedit.cpp
// Last edit: 09/07/2017 02:27 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDEditBox::OSDEditBox(EditBox &ref) : OSDControl(ref)
	{
		m_placeholderSet = false;
		m_systemCommand = false;
	}

	void OSDEditBox::setStyle(u32 style)
	{
		DWORD windowExStyle = 0;
		if (style & 1)
			windowExStyle |= WS_EX_CLIENTEDGE;
		::SetLastError(ERROR_SUCCESS);
		DWORD last = ::SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, (LONG_PTR)windowExStyle);
		DWORD err = ::GetLastError();
	}

	void OSDEditBox::create()
	{
		//Themes causing some problems, lets set the style on creation
		DWORD windowExStyle = 0;
		if (ref().style() & 1)
			windowExStyle |= WS_EX_CLIENTEDGE;
		createWindow(*this, TEXT("edit"), windowExStyle, WS_TABSTOP | ES_AUTOHSCROLL);

		// todo: For multiline
		//	createWindow(*this,TEXT("edit"),windowExStyle,WS_VSCROLL|ES_AUTOVSCROLL|ES_MULTILINE);
	}

	void OSDEditBox::update()
	{
		OSDControl::update();
		setText(ref().text());
		setMaxChars(ref().maxChars());
		setEditable(ref().editable());
		setPlaceholderText(ref().placeholderText());
		setPlaceholderTextColor(ref().placeholderTextColor());
		setPlaceholderTextBehavior(ref().placeholderTextBehavior());
		setPasswordMode(ref().passwordMode());
	}

	void OSDEditBox::setText(const string &text)
	{
		::SetWindowText(m_hwnd, text.data()); //doesn't block Change notification
	}

	void OSDEditBox::setTextColor(const Color &color)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

	void OSDEditBox::setMaxChars(uint nChars)
	{
		//if wParam == 0, the text limit is set to the default limit, which is usually huge
		::SendMessage(m_hwnd, EM_LIMITTEXT, (WPARAM)nChars, (LPARAM)0);
	}

	void OSDEditBox::setEditable(bool editable)
	{
		//	::SendMessage(m_hwnd,EM_SETREADONLY,(WPARAM)(editable ? FALSE : TRUE),(LPARAM)0);
	}

	void OSDEditBox::setPlaceholderText(const string &text)
	{
		if (ref().text().length() == 0) {
			m_placeholderSet = true;
			//::SetWindowText(m_hwnd, text.data());
		}
	}

	void OSDEditBox::setPlaceholderTextColor(const Color &color)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

	void OSDEditBox::setPlaceholderTextBehavior(PlaceholderTextBehavior behavior)
	{
	}

	void OSDEditBox::setPasswordMode(bool passwordMode)
	{
		::SendMessage(m_hwnd, EM_SETPASSWORDCHAR, passwordMode ? '*' : 0, 0);
	}

	void OSDEditBox::setParent(Control *parent)
	{
		//  It seems that Edit controls only send notifications when
		//it is created with a valid window handle as parent.
		//  As we need the EN_CHANGE notification, we need to recreate the control.
		destroy();
		create();
		update();
	}

	LRESULT OSDEditBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_SETFOCUS, wParam, lParam);
		if (ref().text().length() == 0) {
			//::SendMessage(m_hwnd, EM_SETSEL, 0, 0);
			//::SendMessage(m_hwnd, EM_SCROLLCARET, 0, 0);
			//return 0;
		}
		return 0;
	}

	LRESULT OSDEditBox::OnSetCursor(WPARAM wParam, LPARAM lParam)
	{
		int a = 1;
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_SETCURSOR, wParam, lParam);
	}

	LRESULT OSDEditBox::OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		int vk = wParam;
		//remove placeholder
		/*
		if (m_placeholderSet) {
			if (vk == VK_DELETE || vk == VK_BACK
				|| vk == VK_LEFT || vk == VK_RIGHT || vk == VK_UP || vk == VK_DOWN
				|| vk == VK_PRIOR || vk == VK_NEXT || vk == VK_END || vk == VK_HOME)
				return 0;
		}
		*/

		/*
		char myMask[] = "999...9AS";
		int myMaskLen = ::strlen(myMask);
		int textLength = ref().text().length();
		string text = ref().text();
		int start, end, selCount;
		::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
		selCount = end - start;
		if(vk == VK_DELETE) {
			if(start < textLength)
				if(text.data()[start] == myMask[start])
					return 0;

			//if it is the last char, trim mask
			if(textLength - selCount == start) {
				int i = textLength - 2;
				int charsToRemove = 0;
				while(text.data()[i] == myMask[i] && i >= 0) {
					++charsToRemove;
					--i;
				}
				if(charsToRemove > 0) {
					::SendMessage(m_hwnd, EM_SETSEL, textLength - 1 - charsToRemove, textLength - 1);
					::SendMessage(m_hwnd, EM_REPLACESEL, FALSE, (LPARAM)"");
				}
			}
		}

		*/
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_KEYDOWN, wParam, lParam);
	}

	LRESULT OSDEditBox::OnChar(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == 0x08) { //backspace
			//if (m_placeholderSet)
			//	return 0;
		}
		else {

			/*
			char myMask[] = "999...9AS";
			int myMaskLen = ::strlen(myMask);

			int textLength = ref().text().length();
			int start, end;
			::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
			int selCount = end - start;

			int behavior = 0;
			bool res = behavior == 0 ? false : true;
			bool fixedChar;
			int fixedCharCount = 0;
			char c;
			int i = start;
			do {
				fixedChar = false;
				if(start < myMaskLen) {
					c = myMask[i];
					if(c == '9') res = isdigit(wParam);
					else if(c == 'S') res = isalnum(wParam);
					else if(c == 'A') res = isalpha(wParam);
					else {
						fixedChar = true;
						++fixedCharCount;
						++i;
					}
				}
				else {
					res = false;
				}
			} while(fixedChar);

			if(!res)
				return 0;

			for(int i = 0; i < fixedCharCount; ++i) {
				m_systemCommand = true;
				char _c[2] = { myMask[start+i],'\0' };
				::SendMessage(m_hwnd, EM_REPLACESEL, FALSE, (LPARAM)_c);
				m_systemCommand = false;
			}
			*/
		}

		/*
		//remove placeholder
		if (m_placeholderSet) {
			if (ref().placeholderTextBehavior() == HIDE_ON_FIRST_CHAR) {
				m_systemCommand = true;
				::SetWindowText(m_hwnd, "");
				m_systemCommand = false;
				m_placeholderSet = false;
			}
		}
		*/
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_CHAR, wParam, lParam);
	}

	void OSDEditBox::onMouseMoveEvent(MouseEvent &event)
	{
		/*
		if (event.buttonsState() & 1 && m_placeholderSet)
			event.accepted();
		else*/
			event.ignore();
	}

	void OSDEditBox::onMousePressEvent(MouseEvent &event)
	{
		/*
		if (event.button() == 1) {
			if (m_placeholderSet) {
				::CallWindowProc(m_originalWndProc, m_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
				//::SendMessage(m_hwnd, EM_SETSEL, 0, 0);
				return;
			}
		}
		*/

		event.ignore();
	}

	void OSDEditBox::onMouseDoubleClickEvent(MouseEvent &event)
	{
		/*
		if (event.button() == 1) {
			if (m_placeholderSet) {
				event.accept();
				return;
			}
		}
		*/

		event.ignore();
	}

	LRESULT OSDEditBox::OnPaste(WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_PASTE, wParam, lParam);
	}

	LRESULT OSDEditBox::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_COMMAND) {
			switch (HIWORD(wParam)) {
					
				case EN_SETFOCUS:
				{
					/*
					if (m_placeholderSet) {
						::SendMessage(m_hwnd, EM_SETSEL, 0, 0);
						if (ref().placeholderTextBehavior() == HIDE_ON_FOCUS) {
							::SetWindowText(m_hwnd, "");
							m_placeholderSet = false;
						}
					}
					*/
					break;
				}

				case EN_KILLFOCUS:
				{
					/*
					if (ref().placeholderTextBehavior() == HIDE_ON_FOCUS) {
						unsigned int len = ref().text().length();
						if (len == 0) {
							m_placeholderSet = true;
							::SetWindowText(m_hwnd, ref().placeholderText().data());
						}
					}
					*/
					break;
				}

				case EN_CHANGE:
				{
					if (m_systemCommand)
						return 0;

					int curSel = LOWORD(::SendMessage(m_hwnd, EM_GETSEL, 0, 0));
					string oldText = ref().text();
					int oldLen = ref().text().length();
					string newText;
					int newLen;

					unsigned int controlLen = ::GetWindowTextLength(m_hwnd);
					TCHAR *buf = new TCHAR[controlLen + 1];
					if (buf) {
						::GetWindowText(m_hwnd, buf, controlLen + 1);
					}
					newText = buf;
					newLen = newText.length();
					delete[] buf;

					if (newText == oldText)
						return 0;

					//if (!m_placeholderSet) {
						/*
						if((curSel-1) == 4 && !isdigit(buf[curSel-1])) {
							buf[curSel-1] = '\0';
							::SetWindowText(m_hwnd, buf);
							::SendMessage(m_hwnd, EM_SETSEL, curSel, curSel);
							return 0;
						}
						*/

						*ref().m_receivebuffer = newText;
						if (ref().m_onChange)
							ref().m_onChange(ref());

						::SendMessage(m_hwnd, EM_SETSEL, curSel, curSel);
					//}

					/*
					if (ref().placeholderTextBehavior() == HIDE_ON_FIRST_CHAR) {
						if (newLen == 0 && !m_placeholderSet) {
							m_placeholderSet = true;
							::SetWindowText(m_hwnd, ref().placeholderText().data());
						}
					}
					*/
				}break;

				default:
				{
					int a = 1;
					break;
				}


			}

			//return 0;
		}

		else if (msg == WM_CTLCOLOREDIT)
		{
			Color textColor = ref().textColor();

			if (m_placeholderSet && *ref().placeholderText().data()) {
				textColor = ref().placeholderTextColor();
			}

			::SetTextColor((HDC)wParam, RGB(textColor.red(), textColor.green(), textColor.blue()));
			::SetBkMode((HDC)wParam, TRANSPARENT);

			if (ref().transparent()) {
				if (ref().parent()) {
					return (LRESULT)ref().parent()->osdRef().backgroundBrush();
				}
			}

			HBRUSH backBrush = m_backBrush;
			if (!backBrush)
				backBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			return (LRESULT)backBrush;
		}

		else if (msg == WM_CTLCOLORSTATIC) {
			Color textColor = ref().textColor();
			::SetTextColor((HDC)wParam, RGB(textColor.red(), textColor.green(), textColor.blue()));
			if (ref().parent()) {
				return (LRESULT)ref().parent()->osdRef().backgroundBrush();
			}
			return 0;
		}

		else if (msg == WM_SETCURSOR) {
			int a = 1;
		}
		else {
			int a = 1;
		}

		return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
	}

}