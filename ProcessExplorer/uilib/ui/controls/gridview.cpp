
//GridView 1.0
// File: gridview.h
// Last edit: 09/12/2017 18:21 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	GridView::GridView()
		: Control(*new OSDGridView(*this))
	{
		m_rowCount = 0;
		m_colCount = 0;
		m_headerVisible = false;
		m_selectedRow = -1;
		m_selectedCol = -1;
		m_onColResize = NULL;
		m_onItemSelect = NULL;

		m_contentScrollX = 0;
		m_contentScrollY = 0;

		//
		osdRef().create();
		setDefaultFont();
	}

	GridView::~GridView()
	{
	}

	void GridView::setHeaderVisible(bool visible)
	{
		m_headerVisible = visible;
		osdRef().setHeaderVisible(visible);
	}

	void GridView::setHeaderText(int col, const string &text)
	{
		if (col >= m_colCount)
			return;
		m_headerText[col] = text;
		osdRef().setHeaderText(col, text);
	}

	void GridView::_setRowCount(int rowCount)
	{
		int i, j;
		if (rowCount < m_rowCount) {
			int diff = m_rowCount - rowCount;
			for (i = rowCount; i < m_rowCount; ++i) {
				for (j = 0; j < m_colCount; ++j) {
					if (m_items[i][j])
						delete m_items[i][j];
				}
			}
		}

		m_items.resize(rowCount);
		for (i = m_rowCount; i < rowCount; ++i)
			m_items[i].resize(m_colCount);
		m_rowCount = rowCount;
	}

	void GridView::_setColumnCount(int colCount)
	{
		int i, j;
		if (colCount < m_colCount) {
			uint diff = m_colCount - colCount;
			for (i = 0; i < m_rowCount; ++i) {
				for (j = colCount; j < m_colCount; ++j) {
					if (m_items[i][j])
						delete m_items[i][j];
				}
			}
		}

		for (i = 0; i < m_rowCount; ++i) {
			m_items[i].resize(colCount);
		}
		m_headerText.resize(colCount);
		m_colCount = colCount;
	}

	void GridView::setRowCount(int rowCount)
	{
		if (rowCount != m_rowCount) {
			_setRowCount(rowCount);
			osdRef().setCellCount(m_rowCount, m_colCount);
		}
	}

	void GridView::setColumnCount(int colCount)
	{
		if (colCount != m_colCount) {
			_setColumnCount(colCount);
			osdRef().setCellCount(m_rowCount, m_colCount);
		}
	}

	void GridView::selCellCount(int rowCount, int colCount)
	{
		if (rowCount < 0 || colCount < 0)
			return;
		if (rowCount != m_rowCount || colCount != m_colCount) {
			if (colCount != m_colCount)
				_setColumnCount(colCount);
			if (rowCount != m_rowCount)
				_setRowCount(rowCount);
			osdRef().setCellCount(m_rowCount, m_colCount);
		}
	}


	void GridView::insertRow(int at)
	{
		if (at < 0)
			at = m_rowCount;
		if (at > m_rowCount) {
			uint i;
			m_items.resize(at + 1);
			for (i = m_rowCount; i < m_items.size(); ++i)
				m_items[i].resize(m_colCount);
			m_rowCount = at + 1;
		}
		else {
			std::vector<ListViewItem*> test;
			m_items.insert(m_items.begin() + at, test);
			m_items[at].resize(m_colCount);
			++m_rowCount;
		}
		osdRef().insertRow(at);
	}

	void GridView::insertColumn(int at)
	{
		if (at < 0)
			at = m_colCount;
		if (at > m_colCount) {
			uint i;
			for (i = 0; i < m_items.size(); ++i)
				m_items[i].resize(at + 1);
			m_colCount = at + 1;
		}
		else {
			uint i;
			for (i = 0; i < m_items.size(); ++i) {
				m_items[i].insert(m_items[i].begin() + at, NULL);
			}
			++m_colCount;
		}
		osdRef().insertColumn(at);
	}

	void GridView::setItem(int row, int col, ListViewItem *item)
	{
		if (row < 0 || col < 0)
			return;
		if (row >= m_rowCount || col >= m_colCount)
			return;
		m_items[row][col] = item;
		osdRef().setItem(row, col, item);
	}

	void GridView::clear()
	{
		if (m_rowCount > 0) {
			for (int i = 0; i < m_rowCount; ++i) {
				for (int j = 0; j < m_colCount; ++j) {
					if (m_items[i][j])
						delete m_items[i][j];
				}
			}

			m_rowCount = 0;
			m_colCount = 0;
			m_items.clear();
		}
		osdRef().clear();
	}

	void GridView::deleteRows()
	{
		setRowCount(0);
	}




	int GridView::getColumnWidth(int col) {
		if (col < 0 || col >= m_colCount)
			return -1;
		return osdRef().getColumnWidth(col);
	}

	void GridView::setOnColumnResize(GridViewColResizeFunc onColResize)
	{
		m_onColResize = onColResize;
	}

	void GridView::setOnItemSelect(GridViewActionProc onItemSelect)
	{
		m_onItemSelect = onItemSelect;
	}

	//todo: change maybe
	//is this an event? make it ona proper Event structure
	//if not, work on a better integration between osdgridview and osd header
	void GridView::onColumnResize(int col, int newWidth)
	{
		if (m_onColResize)
			m_onColResize(*this, col, newWidth);
	}

}