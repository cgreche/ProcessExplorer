
//OSDUITools
// File: osduitools.cpp
// Last edit: 29/11/2014 12:35 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

#include <stdio.h>
#include <tchar.h>

namespace uilib {

	int OSDMessage::winmessage(HWND hwnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
	{
		return ::MessageBox(hwnd, lpText, lpCaption, uType);
	}

	void OSDMessage::msg(Window *parent, const string &text)
	{
		HWND hwndparent = parent ? parent->osdRef().handle() : NULL;
		UINT utype = parent ? 0 : MB_TASKMODAL;
		winmessage(hwndparent, text.data(), TEXT("Message"), utype);
	}

	void OSDMessage::warning(Window *parent, const string &text)
	{
		HWND hwndparent = parent ? parent->osdRef().handle() : NULL;
		UINT utype = parent ? MB_ICONWARNING : (MB_ICONWARNING | MB_TASKMODAL);
		winmessage(hwndparent, text.data(), TEXT("Warning"), utype);
	}

	void OSDMessage::error(Window *parent, const string &text)
	{
		HWND hwndparent = parent ? parent->osdRef().handle() : NULL;
		UINT utype = parent ? MB_ICONERROR : (MB_ICONERROR | MB_TASKMODAL);
		winmessage(hwndparent, text.data(), TEXT("Error"), utype);
	}

	Message::message_result OSDMessage::question(Window *parent, const string &text)
	{
		HWND hwndparent = parent ? parent->osdRef().handle() : NULL;
		UINT utype = parent ? MB_ICONQUESTION | MB_YESNO : (MB_ICONQUESTION | MB_YESNO | MB_TASKMODAL);
		u32 res = OSDMessage::winmessage(hwndparent, text.data(), TEXT("Error"), utype);
		if (res == IDYES)
			return Message::yes;
		return Message::no;
	}




	Size OSDUITools::getTextSize(const string& text, const FontDesc& fontDesc)
	{
		Font font;
		font.set(fontDesc);
		HDC hdc = ::GetDC(NULL);
		HFONT hFontOld = (HFONT)::SelectObject(hdc, font.osdRef().object());
		RECT rect;
		::DrawText(hdc, text.data(), text.length(), &rect, DT_CALCRECT);
		::SelectObject(hdc, hFontOld);
		::ReleaseDC(NULL, hdc);
		return Size(rect.right - rect.left, rect.bottom - rect.top);
	}





	bool OSDUITools::chooseColor(Window *parent, Color &retColor, const Color *defaultColor)
	{
		static COLORREF customColors[16] = { 0 };

		CHOOSECOLOR cc;
		cc.lStructSize = sizeof(CHOOSECOLOR);
		cc.hwndOwner = parent ? parent->osdRef().handle() : NULL;
		cc.hInstance = NULL;
		cc.rgbResult = defaultColor ? RGB(defaultColor->red(), defaultColor->green(), defaultColor->blue()) : 0;
		cc.lpCustColors = customColors;
		cc.Flags = CC_FULLOPEN | (defaultColor ? CC_RGBINIT : 0);
		cc.lCustData = NULL;
		cc.lpfnHook = NULL;
		cc.lpTemplateName = NULL;

		BOOL res = ::ChooseColor(&cc);
		if (res == TRUE) {
			retColor = Color(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
		}

		return res != FALSE;
	}

}