
#include "..\\uibase.h"

//ListView 1.0
// File: listview.cpp
// Last edit: 16/06/2017 00:57 (UTC-3)
// Author: CGR

namespace uilib {

	ListViewItem::ListViewItem(const string &text, const Color &textcolor, const FontDesc &fd, void *data, const Color &backcolor, bool transparent)
		:m_text(text),
		m_textColor(textcolor),
		m_state((ListViewItemState)0),
		m_data(data),
		m_backColor(backcolor),
		m_transparent(transparent),
		m_parentList(NULL),
		m_index(-1),
		m_colIndex(-1)
	{
		m_font.set(fd);
	}

	ListViewItem::~ListViewItem()
	{
	}

	void ListViewItem::setText(const string& text)
	{
		m_text = text;
		if (m_parentList)
			m_parentList->updateItem(*this);
	}

	void ListViewItem::setTextColor(const Color &color)
	{
		m_textColor = color;
		if (m_parentList)
			m_parentList->updateItem(*this);
	}

	void ListViewItem::setTransparent(bool transparent)
	{
		m_transparent = transparent;
		if (m_parentList)
			m_parentList->updateItem(*this);
	}





	//
	//
	////
	void ListView::updateItem(ListViewItem &item)
	{
		osdRef().updateItem(item);
	}

	ListView::ListView() :Control(*new OSDListView(*this))
	{
		m_transparent = false;
		m_style = 0;
		m_onSelect = NULL;
		m_onActivate = NULL;

		//
		osdRef().create();
		setDefaultFont();
	}

	ListView::ListView(OSDListView &osdRef) :Control(osdRef)
	{
		m_transparent = false;
		m_style = 0;
		m_onSelect = NULL;
		m_onActivate = NULL;
	}

	ListView::~ListView()
	{
	}

	int ListView::fastinsertItem(const string &text, uint indexPos)
	{
		return insertItem(*new ListViewItem(text, Color(0, 0, 0), m_font.desc()), indexPos);
	}

	int ListView::insertItem(ListViewItem &item, uint indexPos)
	{
		if (item.m_parentList) //item i sin a list already
			return -1;

		uint itemCount = m_items.size();

		if (indexPos > itemCount)
			indexPos = itemCount;

		m_items.insert(m_items.begin() + indexPos, &item);
		osdRef().insertItem(item, indexPos);

		if (indexPos < itemCount - 1) {
			uint i;
			//update other items index
			for (i = indexPos; i < itemCount; ++i) {
				ListViewItem *itemAtIndex = m_items[i];
				itemAtIndex->m_index = i + 1;
			}
		}

		//set item and its subItems internal info about us
		item.m_parentList = this;
		item.m_index = indexPos;
		return indexPos;
	}

	void ListView::reset()
	{
		if (m_items.size() > 0) {
			std::vector<ListViewItem*>::iterator it;
			it = m_items.begin();
			while (it != m_items.end()) {
				delete *it;
				++it;
			}
			m_items.clear();
		}
		osdRef().reset();
	}

	void ListView::sort()
	{
		osdRef().sort();
	}

	bool ListView::contains(ListViewItem& item, int *pIndex)
	{
		u32 i = 0;
		for (i = 0; i < m_items.size(); ++i) {
			ListViewItem* _item = m_items[i];
			if (&item == _item) {
				if (pIndex)
					*pIndex = i;
				return true;
			}
		}

		if (pIndex)
			*pIndex = -1;
		return false;
	}

	ListViewItem* ListView::findItem(const string& text, int *pIndex)
	{
		ListViewItem *retItem = NULL;
		int index = -1;

		u32 i = 0;
		for (i = 0; i < m_items.size(); ++i) {
			ListViewItem* item = m_items[i];
			if (text == item->text()) {
				index = i;
				retItem = item;
				break;
			}
		}

		if (pIndex)
			*pIndex = index;
		return retItem;
	}

	ListViewItem* ListView::itemAt(uint index)
	{
		if (index >= m_items.size())
			return NULL;

		return m_items[index];
	}

	int ListView::selectItem(int index, bool select)
	{
		if (index < 0) {
			//todo: deselect all
			return 1;
		}

		ListViewItem* item = itemAt(index);
		if (!item)
			return 0;

		item->m_state |= ListViewItem::Selected;
		osdRef().selectItem(index, select);
		return 1;
	}

	int ListView::firstSelected()
	{
		uint i = 0;
		for (i = 0; i < m_items.size(); ++i) {
			ListViewItem* item = m_items[i];
			if (item->selected())
				return i;
		}
		return -1;
	}

	bool ListView::removeItem(uint index)
	{
		if (index >= m_items.size())
			return false;

		bool res = osdRef().removeItem(index);
		if (res) {
			delete m_items[index];
			m_items.erase(m_items.begin() + index);
		}
		return res;
	}

	bool ListView::removeItem(ListViewItem& item)
	{
		int index;
		if (!contains(item, &index))
			return false;

		return removeItem(index);
	}

	int ListView::changeItemIndex(uint curIndex, uint newIndex)
	{
		ListViewItem *item = itemAt(curIndex);
		if (!item) {
			return -1;
		}

		ListViewItem *itemToSwap = itemAt(newIndex);
		if (!itemToSwap)
			return 1;

		//important: we must change in the OS firstly, because OS implementation may request our infos.
		osdRef().changeItemIndex(curIndex, newIndex);

		m_items[curIndex] = itemToSwap;
		m_items[newIndex] = item;
		return 0;
	}


	uint ListView::itemCount() const
	{
		return m_items.size();
	}


	void ListView::setWorkArea(Rect &area)
	{
		m_workarea = area;
		osdRef().setWorkArea(area);
	}

}