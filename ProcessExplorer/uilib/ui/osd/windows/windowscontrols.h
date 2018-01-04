
#ifndef __WINDOWSCONTROLS_H__
#define __WINDOWSCONTROLS_H__

namespace uilib {

#define WIN_MSG(_msg,_class,_method) \
	LRESULT _class::_method(WPARAM wParam, LPARAM lParam)

#define WIN_MSG_DEF(_msg,_class,_method) \
	WIN_MSG(_msg,_class,_method) { return ::CallWindowProc(m_originalWndProc,m_hwnd,_msg,wParam,lParam); }

#define WIN_MSG_VAL(_msg,_class,_method,_val) \
	WIN_MSG(_msg,_class,_method) { return _val; }
#define WIN_MSG_BLOCK(_msg,_class,_method,_val) \
	WIN_MSG_VAL(_msg,_class,_method,0)

#define MSG_HANDLER(_method) \
	virtual LRESULT _method(WPARAM wParam,LPARAM lParam);

	void createWindow(OSDControl &osdControl, const TCHAR *windowClass, DWORD exStyle, DWORD style);

}

#endif