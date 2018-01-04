
//EditBox
// File: editbox.h
// Last edit: 02/07/2017 02:25 (UTC-3)
// Author: CGR

#ifndef __UILIB_EDITBOX_H__
#define __UILIB_EDITBOX_H__


#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif


#define UNLIMITEDCHARS 0

class EditBox;

typedef void (*EditBoxFunc)(EditBox& editBox);

/*
class Observable
{

public:
	void addObserver();
};
*/

class EditBox : public Control
{

protected:
	string m_text; //default buffer
	string *m_receivebuffer;
	Color m_textColor;
	uint m_maxChars;
	bool m_editable;

	string m_placeholder;
	Color m_placeholderColor;

	string mask;

	EditBoxFunc m_onChange;

public:
	EditBox();
	EditBox(OSDEditBox& ref);
	
	//Control
	virtual Size getAutoSize();

	//
	void update();

	//Events setter
	void setOnChange(EditBoxFunc onChange);

	//Attributes setter
	void setText(const string &text);
	void setReceiveBuffer(string *pBuffer);
	void setTextColor(const Color &color);
	void setMaxChars(uint nChars);
	void setEditable(bool editable);

	//Getters
	string& text() { return *m_receivebuffer; }
	Color textColor() const { return m_textColor; }
	u32 maxChars() const { return m_maxChars; }
	bool editable() const { return m_editable; }

	friend class OSDEditBox;
	OSDEditBox& osdRef() const { return reinterpret_cast<OSDEditBox&>(Control::osdRef()); }
};

#endif