
//OSDListView
// File: osdlistview.cpp
// Last edit: 26/06/2017 22:04 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDListView::OSDListView(ListView &ref) : OSDControl(ref)
	{

	}

	void OSDListView::create()
	{
		createWindow(*this, WC_LISTVIEW, 0, WS_TABSTOP | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
		ListView_SetToolTips(m_hwnd, NULL); //it starts enabled, and can cause ugly appearance at first
	}

	void OSDListView::update()
	{
		OSDControl::update();
		for (uint i = 0; i < ref().m_items.size(); ++i)
			insertItem(*ref().m_items[i], i);
	}


	void OSDListView::setStyle(u32 style)
	{
		if (style&CS_Border) {
			::SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, (LONG_PTR)WS_EX_CLIENTEDGE);
		}
		else {
			::SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, (LONG_PTR)0);
		}
		::SetWindowPos(m_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	}

	void OSDListView::setBackgroundColor(const Color& color)
	{
		//	ListView_SetBkColor(m_hwnd,RGB(color.red,color.green,color.blue));
		this->redraw();
	}

	void OSDListView::setTransparent(bool transparent)
	{
		//	setBackgroundColor(transparent && ref().parent() ? ref().parent()->backgroundColor() : ref().backgroundColor());
		this->redraw();
	}

	int OSDListView::insertItem(ListViewItem &item, uint indexPos)
	{
		uint itemCount = ListView_GetItemCount(m_hwnd);

		if (indexPos > itemCount)
			indexPos = itemCount;

		LVITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.pszText = (char*)item.text().data();
		lvi.iItem = indexPos;//ListView_GetItemCount(m_hwnd);
		lvi.iSubItem = 0;
		lvi.lParam = (LPARAM)&item;
		int index = ListView_InsertItem(m_hwnd, &lvi);

		if (index >= 0) {
			ListView_SetColumnWidth(m_hwnd, 0,/*LVSCW_AUTOSIZE*/LVSCW_AUTOSIZE_USEHEADER);

			/*
			for(uint i = 0; i < item.subItemCount(); ++i) {
				lvi.mask = LVIF_TEXT; //Subitems don't have the State/Param attribute (from msdn), they MUST NOT be set.
				lvi.pszText = (char*)item.subItem(i)->text().data();
				lvi.iItem = index;
				lvi.iSubItem = i+1;
				BOOL res = ListView_SetItem(m_hwnd,&lvi);

	//			ListView_SetColumnWidth(m_hwnd,i+1,/*LVSCW_AUTOSIZE*///LVSCW_AUTOSIZE_USEHEADER);
	//		}
		}

		return index;
	}


	void OSDListView::reset()
	{
		ListView_DeleteAllItems(m_hwnd);
	}

	void OSDListView::sort()
	{
	}

	bool OSDListView::removeItem(uint index)
	{
		return ListView_DeleteItem(m_hwnd, index) != FALSE;
	}

	int OSDListView::selectItem(int index, bool select)
	{
		if (index < 0) {
			return 1;
		}

		ListView_SetItemState(m_hwnd, index, LVIS_SELECTED, LVIS_SELECTED);
		return 1;
	}

	int OSDListView::changeItemIndex(uint curIndex, uint newIndex)
	{
		ListViewItem *item = ref().itemAt(curIndex);
		ListViewItem *itemToSwap = ref().itemAt(newIndex);

		if (!item || !itemToSwap)
			return -1;

		removeItem(curIndex);
		insertItem(*itemToSwap, curIndex);
		removeItem(newIndex);
		insertItem(*item, newIndex);
		return 0;
	}


	void OSDListView::setWorkArea(Rect &area)
	{
		RECT rc = {
			area.x1(),
			area.y1(),
			area.x2(),
			area.y2() };
		//	::SendMessage(m_hwnd,LVM_SETWORKAREAS,(WPARAM)1,(LPARAM)&rc);

		ListView_SetWorkAreas(m_hwnd, 1, &rc);
	}


	void OSDListView::updateItem(ListViewItem &item)
	{
		if (item.index() < 0)
			return;

		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = (char*)item.text().data();
		lvi.iItem = item.index();
		lvi.iSubItem = item.colIndex();
		ListView_SetItem(m_hwnd, &lvi);
	}

	void OSDListView::onPaintEvent(PaintEvent &event)
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

	LRESULT OSDListView::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
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

	LRESULT OSDListView::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
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
				ListViewItem *lvitem = (ListViewItem*)lplvcd->nmcd.lItemlParam;
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

				if (!(nmlv->uOldState & LVIS_FOCUSED) && (nmlv->uNewState & LVIS_FOCUSED)) {
					item->m_state |= ListViewItem::Focused;
				}
				else if ((nmlv->uOldState & LVIS_FOCUSED) && !(nmlv->uNewState & LVIS_FOCUSED)) {
					item->m_state &= ~ListViewItem::Focused;
				}

				if (!(nmlv->uOldState & LVIS_SELECTED) && (nmlv->uNewState & LVIS_SELECTED)) {
					item->m_state |= ListViewItem::Selected;
					if (ref().m_onSelect) {
						ref().m_onSelect(ref(), nmlv->iItem, nmlv->iSubItem);
					}
				}
				else if ((nmlv->uOldState & LVIS_SELECTED) && !(nmlv->uNewState & LVIS_SELECTED)) {
					item->m_state &= ~ListViewItem::Selected;
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
			if (ref().m_onActivate) {
				/*
				//lpnmia->lParam doesn't retrieve the correct param, use ListView_GetItem instead
				LVITEM lvi;
				lvi.mask = LVIF_PARAM;
				lvi.iItem = lpnmia->iItem;
				lvi.iSubItem = lpnmia->iSubItem;
				ListView_GetItem(m_hwnd,&lvi);
				*/
				ref().m_onActivate(ref(), lpnmia->iItem, lpnmia->iSubItem);
			}
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