//GridView 1.0
// File: gridview.h
// Last edit: 09/12/2017 18:21 (UTC-3)
// Author: CGR

#ifndef __UILIB_GRIDVIEW_H__
#define __UILIB_GRIDVIEW_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	class ListView;
	class ListViewItem;
	class GridView;

	typedef void(*GridViewColResizeFunc)(GridView &gridView, int col, int newWidth);
	typedef void(*GridViewActionProc)(GridView &gridView, int row, int col);

	class GridView : public Control
	{
	protected:
		std::vector<string> m_headerText;
		int m_rowCount;
		int m_colCount;
		bool m_headerVisible;
		std::vector<std::vector<ListViewItem *> > m_items;
		int m_selectedRow;
		int m_selectedCol;

		GridViewColResizeFunc m_onColResize;
		GridViewActionProc m_onItemSelect;

		void _setRowCount(int rowCount);
		void _setColumnCount(int rowCount);

		int m_contentScrollX;
		int m_contentScrollY;

	public:
		GridView();
		virtual ~GridView();

		int getColumnWidth(int col);
		void insertRow(int at = -1);
		void insertColumn(int at = -1);

		void setHeaderVisible(bool visible);
		void setHeaderText(int col, const string &text);
		void selCellCount(int rowCount, int colCount);
		void setRowCount(int rowCount);
		void setColumnCount(int colCount);
		void setItem(int row, int col, ListViewItem *item);

		void clear();
		void deleteRows();

		ListViewItem* item(int row, int col) const { return m_items[row][col]; }
		int selectedRow() const { return m_selectedRow; }
		int selectedCol() const { return m_selectedCol; }
		bool headerVisible() const { return m_headerVisible; }
		const string &headerText(int col) const { return m_headerText[col]; }
		int rowCount() const { return m_rowCount; }
		int columnCount() const { return m_colCount; }
		int contentScrollPosX() const { return m_contentScrollX; }
		int contentScrollPosY() const { return m_contentScrollY; }

		void setOnColumnResize(GridViewColResizeFunc onColResize);
		void setOnItemSelect(GridViewActionProc onItemSelect);
		void onColumnResize(int col, int newWidth);

		friend class OSDGridView;
		OSDGridView& osdRef() const { return reinterpret_cast<OSDGridView&>(Control::osdRef()); }
	};

}

#endif