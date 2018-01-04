//ListView 1.0
// File: listview.h
// Last edit: 04/01/2018 01:58 (UTC-3)
// Author: CGR

#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

namespace uilib {

	class ListView;
	class ListViewItem;
	class ReportView;

	typedef void(*ListViewActionProc)(ListView &list, int itemIndex, int subitemIndex);

#define LVITEM_MAX_POS 0xFFFFFFFE
#define LVITEM_INVALID_INDEX 0xFFFFFFFF

	class ListViewItem
	{
	public:
		enum ListViewItemState
		{
			Focused = 1,
			Selected = 2
		};

	private:
		string m_text;

		Color m_textColor;
		Color m_backColor;
		bool m_transparent;

		Font m_font;

		u32 m_state;
		void *m_data;

		ListView *m_parentList;
		int m_index;
		int m_colIndex;

	public:

		friend class ListView;
		friend class OSDListView;
		friend class GridView;
		friend class OSDGridView;

		ListViewItem(const string &text,
			const Color &textcolor = Color(0, 0, 0),
			const FontDesc &fd = FontDesc("Arial", 10, 0),
			void *param = NULL, const Color &backcolor = Color(255, 255, 255), bool transparent = true);

		~ListViewItem();

		void setText(const string &text);
		void setTextColor(const Color &color);
		void setTransparent(bool b);

		const string &text() const { return m_text; }
		Color textColor() const { return m_textColor; }
		Color backgroundColor() const { return m_backColor; }
		void *data() const { return m_data; }

		bool isTransparent() { return m_transparent; }
		Font &font() { return m_font; }

		ListView *parent() const { return m_parentList; }

		bool focused() const { return (m_state&Focused) != 0; }
		bool selected() const { return (m_state&Selected) != 0; }

		int index() const { return m_index; }
		int colIndex() const { return m_index; }
	};

	class ListView : public Control
	{
	protected:
		Rect m_workarea;

		std::vector<ListViewItem*> m_items;

		ListViewActionProc m_onSelect;
		ListViewActionProc m_onActivate;

		void updateItem(ListViewItem &item);

	public:
		friend class ListViewItem;
		ListView();
		ListView(OSDListView& ref);
		virtual ~ListView();

		//Attributes setter
		void setWorkArea(Rect &area);

		//Events Setter
		void setOnSelect(ListViewActionProc onSelect) { m_onSelect = onSelect; }
		void setOnActivate(ListViewActionProc onActivate) { m_onActivate = onActivate; }

		//
		int fastinsertItem(const string &text, uint pos = LVITEM_MAX_POS);
		int insertItem(ListViewItem &item, uint pos = LVITEM_MAX_POS);
		void reset();
		void sort();
		bool contains(ListViewItem& item, int *pIndex = NULL);
		ListViewItem* findItem(const string& text, int *pIndex = NULL);
		ListViewItem* itemAt(uint index);
		int selectItem(int index, bool select);
		int firstSelected();
		bool removeItem(uint index);
		bool removeItem(ListViewItem& item);
		int changeItemIndex(uint curIndex, uint newIndex);
		uint itemCount() const;

		//
		friend class OSDListView;
		OSDListView& osdRef() const { return reinterpret_cast<OSDListView&>(Control::osdRef()); }
	};

}

#endif