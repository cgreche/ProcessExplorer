
//OSDGridView
// File: osdgridview.cpp
// Last edit: 07/01/2018 15:24 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDGridView::OSDGridView(GridView &ref)
		: OSDControl(ref)
	{

	}

	void OSDGridView::create()
	{
		createWindow(*this, WC_LISTVIEW, 0, WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER);

		HWND header = ListView_GetHeader(m_hwnd);
		if (header) {
			/*
			SUMMARY Starting with version 4.70 of ComCtl32.dll,
			the header control of a list view control in report view (LVS_REPORT)
			automatically receives the HDS_FULLDRAG style. When this style is set,
			the parent of a list view control receives HDN_ITEMCHANGING notifications,
			rather than HDN_TRACK notifications, when the column divider of the header
			control is dragged. To receive HDN_TRACK notifications, the header control
			of the list view control must not have the HDS_FULLDRAG style set.
			Note that the HDS_FULLDRAG style is ignored in versions of ComCtl32.dll prior to 4.70.
			*/
			LONG_PTR style = ::GetWindowLongPtr(header, GWL_STYLE);
			::SetWindowLongPtr(header, GWL_STYLE, style | HDS_HOTTRACK);
		}

		ListView_SetExtendedListViewStyle(m_hwnd, LVS_EX_FULLROWSELECT);
	}

	void OSDGridView::update()
	{
		OSDControl::update();
		//	setStyle(ref.m_style);
		setCellCount(ref().m_rowCount, ref().m_colCount);

		int row, col;
		//update header text
		for (col = 0; col < ref().m_colCount; ++col) {
			setHeaderText(col, ref().m_headerText[col]);
		}

		//update items
		for (row = 0; row < ref().m_rowCount; ++row) {
			for (col = 0; col < ref().m_items[row].size(); ++col) {
				setItem(row, col, ref().m_items[row][col]);
			}
		}
		setHeaderVisible(ref().headerVisible());
	}

	void OSDGridView::setHeaderVisible(bool b)
	{
		::SetWindowLong(m_hwnd, GWL_STYLE,
			(::GetWindowLong(m_hwnd, GWL_STYLE) & ~LVS_NOCOLUMNHEADER) |
			(b ? 0 : LVS_NOCOLUMNHEADER));
	}

	void OSDGridView::setHeaderText(int col, const string &text)
	{
		LVCOLUMN column;
		column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
		column.fmt = LVCFMT_LEFT;
		column.pszText = (char*)text.data();
		column.iSubItem = col;
		int res = ListView_SetColumn(m_hwnd, col, &column);
		int a = 1;
	}

	void OSDGridView::setCellCount(int rowCount, int colCount)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = 0;
		lvi.pszText = (char*)"";
		lvi.lParam = (LPARAM)NULL;
		int count = ListView_GetItemCount(m_hwnd);
		if (rowCount < count) {
			for (int i = rowCount; i < count; ++i) {
				//All items are invalid, we need to remove the reference
				LVITEM lvi;
				lvi.mask = LVIF_PARAM;
				lvi.iItem = i;
				lvi.iSubItem = 0;
				lvi.lParam = (LPARAM)NULL;
				BOOL result = ListView_SetItem(m_hwnd, &lvi);
				if (!result)
					int a = 1;
			}

			for (int i = rowCount; i < count; ++i) {
				BOOL result = ListView_DeleteItem(m_hwnd, rowCount);
			}
		}
		else {
			while (count < rowCount) {
				lvi.iItem = count;
				int index = ListView_InsertItem(m_hwnd, &lvi);
				++count;
			}
		}

		LVCOLUMN column;
		column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
		column.fmt = LVCFMT_LEFT;
		column.pszText = (char*)"";
		column.cchTextMax = 0;
		count = Header_GetItemCount(ListView_GetHeader(m_hwnd));
		if (colCount < count) {
			for (int i = colCount; i < count; ++i)
				ListView_DeleteColumn(m_hwnd, i);
		}
		else {
			while (count < colCount) {
				column.iSubItem = count;
				int index = ListView_InsertColumn(m_hwnd, count, &column);
				ListView_SetColumnWidth(m_hwnd, count, LVSCW_AUTOSIZE);
				++count;
			}
		}
	}

	void OSDGridView::insertRow(int at)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = 0;
		lvi.pszText = (char*)"";
		lvi.lParam = (LPARAM)NULL;
		int count = ListView_GetItemCount(m_hwnd);
		while (count < at) {
			lvi.iItem = count;
			int index = ListView_InsertItem(m_hwnd, &lvi);
			++count;
		}
		lvi.iItem = at;
		ListView_InsertItem(m_hwnd, &lvi);
	}

	void OSDGridView::insertColumn(int at)
	{
		LVCOLUMN column;
		column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
		column.fmt = LVCFMT_LEFT;
		column.pszText = (char*)"";
		column.cchTextMax = 0;
		int count = Header_GetItemCount(ListView_GetHeader(m_hwnd));
		while (count < at) {
			column.iSubItem = count;
			int index = ListView_InsertColumn(m_hwnd, count, &column);
			++count;
		}
		column.iSubItem = at;
		int index = ListView_InsertColumn(m_hwnd, at, &column);

		//todo: find a better way to disable resizing
	//	EnableWindow(ListView_GetHeader(m_hwnd),FALSE); //disable resizing
	//	ListView_SetColumnWidth(m_hwnd,0,LVSCW_AUTOSIZE/*LVSCW_AUTOSIZE_USEHEADER*/); //set same size as window
		for (int i = 0; i < ref().m_colCount; ++i) {
			ListView_SetColumnWidth(m_hwnd, i, LVSCW_AUTOSIZE_USEHEADER);
		}
	}

	void OSDGridView::setItem(int row, int column, ListViewItem *item)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = row;
		lvi.iSubItem = column;
		lvi.pszText = item ? (char*)item->text().data() : NULL;
		if (column == 0) {
			lvi.mask |= LVIF_PARAM;
			lvi.lParam = (LPARAM)item;
		}
		int res = ListView_SetItem(m_hwnd, &lvi);
		ListView_SetColumnWidth(m_hwnd, column,/*LVSCW_AUTOSIZE*/LVSCW_AUTOSIZE_USEHEADER);
	}

	void OSDGridView::clear()
	{
		ListView_DeleteAllItems(m_hwnd);
	}

	int OSDGridView::getColumnWidth(int col)
	{
		return ListView_GetColumnWidth(m_hwnd, col);
	}

	void OSDGridView::onPaintEvent(PaintEvent &event)
	{
		//todo: full custom draw to make a godlike looking control
		HDC hdc = ::GetDC(m_hwnd);
		RECT rc;
		::GetUpdateRect(m_hwnd, &rc, FALSE);
		int pos = ::GetScrollPos(m_hwnd, SB_VERT);

		HWND header = ListView_GetHeader(m_hwnd);
		RECT headerRect;
		//	::SetWindowPos(header,0,0,10,0,0,SWP_NOZORDER|SWP_NOSIZE);
		::GetClientRect(header, &headerRect);

		if (rc.top >= headerRect.bottom) {
			int a = 1;
		}
		else {
			int a = 1;
		}

		Color bgColor = ref().transparent() && ref().parent() ? ref().parent()->backgroundColor() : ref().backgroundColor();
		HBRUSH hBrush = ::CreateSolidBrush(RGB(9, 180, 99));//RGB(bgColor.red(),bgColor.green(),bgColor.blue()));
	/*	HBITMAP hBmp = (HBITMAP)::LoadImage(::GetModuleHandle(0),"hello.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

		LVBKIMAGE image;
		image.ulFlags = LVBKIF_STYLE_NORMAL|LVBKIF_SOURCE_HBITMAP;
		image.hbm = hBmp;
		image.cchImageMax = 0;
		image.yOffsetPercent = 0;
		image.xOffsetPercent = 0;
	//	int res = ListView_SetBkImage(m_hwnd,&image);

	//	HBRUSH hBrush = ::CreatePatternBrush(hBmp);
	//	HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	//	::ValidateRect(m_hwnd,&rc);
	//	HDC parentDC = ::GetDC(ref().getTopParent()->osdRef().handle());
	//	BitBlt(hdc,rc.left,rc.top,rc.right,rc.bottom,parentDC,rc.left,rc.top,SRCCOPY);
	*/
	//	::FillRect(hdc,&rc,hBrush);
		::DeleteObject(hBrush);
		//	::DeleteObject(hBmp);
		::ReleaseDC(m_hwnd, hdc);
		//	*/
		::CallWindowProc(m_originalWndProc, m_hwnd, WM_PAINT, 0, 0);
	}

	LRESULT OSDGridView::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
	{
		Color bgColor = ref().transparent() && ref().parent() ? ref().parent()->backgroundColor() : ref().backgroundColor();
		ListView_SetBkColor(m_hwnd, RGB(bgColor.red(), bgColor.green(), bgColor.blue()));
		/*
			HDC hdc = ::GetDC(m_hwnd);
			RECT rc;
		//	::GetClientRect(m_hwnd,&rc);
			::GetUpdateRect(m_hwnd,&rc,TRUE);
			HBRUSH hBrush = ::CreateSolidBrush(RGB(bgColor.red,bgColor.green,bgColor.blue));
			::FillRect(hdc,&rc,hBrush);
			::DeleteObject(hBrush);
			::ReleaseDC(m_hwnd,hdc);
			return TRUE;*/

			//	return FALSE;
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_ERASEBKGND, wParam, lParam);
	}

	LRESULT OSDGridView::OnHScroll(WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = ::CallWindowProc(m_originalWndProc, m_hwnd, WM_HSCROLL, wParam, lParam);
		int x, y;
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		::GetScrollInfo(m_hwnd, SB_HORZ, &si);
		x = si.nPos;
		::GetScrollInfo(m_hwnd, SB_VERT, &si);
		y = si.nPos;
		ScrollEvent *event = new ScrollEvent(Point(x, y));
		ref().onScroll(*event);
		bool accepted = event->accepted();
		return accepted ? result : 0;
	}

	LRESULT OSDGridView::OnVScroll(WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = ::CallWindowProc(m_originalWndProc, m_hwnd, WM_VSCROLL, wParam, lParam);
		int x, y;
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		::GetScrollInfo(m_hwnd, SB_HORZ, &si);
		x = si.nPos;
		::GetScrollInfo(m_hwnd, SB_VERT, &si);
		y = si.nPos;
		ScrollEvent *event = new ScrollEvent(Point(x, y));
		ref().onScroll(*event);
		bool accepted = event->accepted();
		return accepted ? result : 0;
	}

	LRESULT OSDGridView::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg != WM_NOTIFY)
			return 0;

		LPNMLISTVIEW nmlv = (LPNMLISTVIEW)lParam;
		int code = ((LPNMHDR)lParam)->code;

		if (code == NM_CLICK)
		{
			LPNMLISTVIEW nmlv = (LPNMLISTVIEW)lParam;
		}
		else if (code == NM_DBLCLK) {}
		else if (code == NM_RCLICK /*NM_FIRST-5*/) {}
		else if (code == NM_RDBLCLK /*NM_FIRST-6*/) {}
		else if (code == NM_SETFOCUS) {}
		else if (code == NM_KILLFOCUS) {}
		else if (code == NM_RELEASEDCAPTURE) {}

		else if (code == NM_CUSTOMDRAW)
		{

			LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

			switch (lplvcd->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT:
			{
				//Before the paint cycle begins
				//request notifications for individual listview items
				return CDRF_NOTIFYITEMDRAW;
			}
			case CDDS_POSTPAINT:
			{
				return CDRF_DODEFAULT;
			}

			case CDDS_ITEMPREPAINT: //Before an item is drawn
			{
				return CDRF_NOTIFYSUBITEMDRAW;
			}break;

			case CDDS_SUBITEM | CDDS_ITEMPREPAINT: //Before a subitem is drawn
			{
				int row = (int)lplvcd->nmcd.dwItemSpec;
				int col = lplvcd->iSubItem;
				ListViewItem *lvitem = ref().m_items[row][col];
				if (lvitem) {
					//set first item color
					Color color = lvitem->textColor();
					Color backColor = lvitem->backgroundColor();
					bool transparent = lvitem->isTransparent();

					if (transparent) {
						if (ref().transparent() && ref().parent()) {
							backColor = ref().parent()->backgroundColor();
						}
						else {
							backColor = ref().backgroundColor();
						}
					}

					lplvcd->clrText = RGB(color.red(), color.green(), color.blue());
					lplvcd->clrTextBk = RGB(backColor.red(), backColor.green(), backColor.blue());
					::SetBkMode(lplvcd->nmcd.hdc, TRANSPARENT);
					::SelectObject(lplvcd->nmcd.hdc, lvitem->font().osdRef().object());
				}
				else {
					Color backColor;
					if (ref().transparent() && ref().parent()) {
						backColor = ref().parent()->backgroundColor();
					}
					else {
						backColor = ref().backgroundColor();
					}
					lplvcd->clrTextBk = RGB(backColor.red(), backColor.green(), backColor.blue());
				}
				return CDRF_NEWFONT;
			}
			}
			return CDRF_DODEFAULT;
		}
		else if (code == LVN_ITEMCHANGING) {}
		else if (code == LVN_ITEMCHANGED) {

			if (nmlv->uChanged & LVIF_STATE)
			{
				ListViewItem *item = (ListViewItem*)nmlv->lParam;
				if (item) {
					if (!(nmlv->uOldState & LVIS_FOCUSED) && (nmlv->uNewState & LVIS_FOCUSED)) {
						item->m_state |= ListViewItem::Focused;
					}
					else if ((nmlv->uOldState & LVIS_FOCUSED) && !(nmlv->uNewState & LVIS_FOCUSED)) {
						item->m_state &= ~ListViewItem::Focused;
					}

					if (!(nmlv->uOldState & LVIS_SELECTED) && (nmlv->uNewState & LVIS_SELECTED)) {
						item->m_state |= ListViewItem::Selected;

						POINT curPos;
						::GetCursorPos(&curPos);
						::ScreenToClient(m_hwnd, &curPos);
						LVHITTESTINFO info = { 0 };
						info.pt = curPos;
						ListView_SubItemHitTest(m_hwnd, &info);
						ref().m_selectedRow = nmlv->iItem;
						ref().m_selectedCol = nmlv->iSubItem;
						if (ref().m_onItemSelect) {
							ref().m_onItemSelect(ref(), nmlv->iItem, info.iSubItem);
						}
					}
					else if ((nmlv->uOldState & LVIS_SELECTED) && !(nmlv->uNewState & LVIS_SELECTED)) {
						item->m_state &= ~ListViewItem::Selected;
					}
				}
			}

		}
		else if (code == LVN_INSERTITEM /*LVN_FIRST-2*/) {}
		else if (code == LVN_DELETEALLITEMS /*LVN_FIRST-4*/) {}
		else if (code == LVN_BEGINDRAG /*LVN_FIRST-9*/) {
			int a = 1;
		}
		else if (code == NM_HOVER /*LVN_FIRST-13*/) {}
		else if (code == LVN_ITEMACTIVATE) {
			LPNMITEMACTIVATE lpnmia = (LPNMITEMACTIVATE)lParam;
			//		if(ref().m_onActivate) {
						/*
						//lpnmia->lParam doesn't retrieve the correct param, use ListView_GetItem instead
						LVITEM lvi;
						lvi.mask = LVIF_PARAM;
						lvi.iItem = lpnmia->iItem;
						lvi.iSubItem = lpnmia->iSubItem;
						ListView_GetItem(m_hwnd,&lvi);
						*/
						//			ref().m_onActivate(ref(),lpnmia->iItem,lpnmia->iSubItem);
						//		}
		}
		else if (code == LVN_HOTTRACK/*LVN_FIRST-21*/) {}
		else if (code == LVN_KEYDOWN/*LVN_FIRST-55*/) {
			int a = 1;
		}
		else if (code == (LVN_FIRST - 80) /*LVN_BEGINSCROLL*/)
		{
			//	::InvalidateRect(m_hwnd,NULL,TRUE);
		}
		else if (code == (LVN_FIRST - 81) /*code == LVN_ENDSCROLL*/)
		{
			::InvalidateRect(m_hwnd, NULL, TRUE);
		}
		else
		{
			int a = code;
		}

		return 0;
	}

}