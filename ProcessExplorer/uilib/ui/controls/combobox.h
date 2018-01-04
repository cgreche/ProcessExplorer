
//ComboBox
// File: combobox.h
// Last edit: 09/12/2017 19:59 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __COMBOBOX_H__
#define __COMBOBOX_H__

#include <vector>

namespace uilib {

	class ComboBox;
	class ComboBoxItem;

	typedef void(*ComboFunc)(ComboBox &combo, ComboBoxItem *item);


	class ComboBoxItem
	{
		string m_text;
		u32 state; //todo

		Color m_textcolor;
		Font m_font;

		void *m_data;

	public:
		friend class ComboBox;

		ComboBoxItem(const string &text,
			const Color &textcolor,
			const FontDesc &fd,
			void *data = NULL) :
			m_text(text), m_textcolor(textcolor),
			m_data(data)
		{
			m_font.set(fd);
		}

		const string &text() const { return m_text; }
		Color textColor() const { return m_textcolor; }
		void *data() const { return m_data; }

		Font &font() { return m_font; }

	};


	class ComboBox : public Control
	{
		ComboFunc m_onSelect;

		std::vector<ComboBoxItem*> m_items;
		int m_selection;

	public:
		ComboBox();

		void setOnSelect(ComboFunc onSelect) { m_onSelect = onSelect; }

		u32 fastinsertItem(const string &text);
		u32 insertItem(ComboBoxItem &item);
		void removeItem(u32 index);
		void removeItem(ComboBoxItem &item);
		void reset();
		void sort();
		int setSelection(int index);
		int selection() const { return m_selection; }
		ComboBoxItem *selectedItem() const { return item(selection()); }
		ComboBoxItem *item(int row) const;
		int itemCount() const { return m_items.size(); }

		virtual Size getAutoSize();

		friend class OSDComboBox;
		OSDComboBox& osdRef() const { return reinterpret_cast<OSDComboBox&>(Control::osdRef()); }
	};

}

#endif