
//OSDBase
// File: osdbase.h
// Last edit: 04/01/2018 01:56 (UTC-3)
// Author: CGR

#ifndef __OSDBASE_H__
#define __OSDBASE_H__

#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE

	//#define UNICODE	
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
//#include <io.h>
#include <shellapi.h>
#include <shlobj.h>

#pragma comment(lib,"comctl32.lib")

#include <list>
#include "windowscontrols.h"

namespace uilib {

	class OSDBase
	{
	public:
		static std::list<OSDUIObj*> *objects;
		static u32 nextId;
		static OSDUIObj *find(u32 id);

		static void UIInit();
		static bool UIProcess();
		static void UIQuit();
	};

	class OSDFont
	{
		HFONT m_hFont;
		int m_refCount;

		bool __create(const FontDesc &fd);
		~OSDFont();
	public:
		OSDFont(const FontDesc &fontDesc);

		HFONT object() const { return m_hFont; }

		int addRef();
		int release();
	};

	class OSDUIObj
	{
		UIObj &m_ref;

	protected:
		u32 m_id;

	public:
		OSDUIObj(UIObj &ref);
		virtual ~OSDUIObj();
		UIObj& ref() const { return m_ref; }

		u32 id() const { return m_id; }

		virtual void create() { }
		virtual void destroy() { }
		virtual void update() { }

		//
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam) { return FALSE; }
	};

	class OSDControl : public OSDUIObj
	{
		friend LRESULT CALLBACK GenericWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		friend class OSDPainter;
	protected:
		HWND m_hwnd;
		WNDPROC m_wndProc;
		WNDPROC m_originalWndProc;
		HBRUSH m_backBrush;

		bool m_hover; //is mouse over? used to detect mouse enter event

	protected:
		virtual void onPaintEvent(PaintEvent &event);
		virtual void onCloseEvent() { }//event.ignore(); }
		virtual void onKeyPressEvent(KeyEvent &event) { event.ignore(); }
		virtual void onKeyReleaseEvent(KeyEvent &event) { event.ignore(); }
		virtual void onMouseMoveEvent(MouseEvent &event) { event.ignore(); }
		virtual void onMousePressEvent(MouseEvent &event) { event.ignore(); }
		virtual void onMouseReleaseEvent(MouseEvent &event) { event.ignore(); }
		virtual void onMouseDoubleClickEvent(MouseEvent &event) { event.ignore(); }
		virtual void onMouseEnterEvent(MouseEvent &event) { event.ignore(); }
		virtual void onMouseLeaveEvent(MouseEvent &event) { event.ignore(); }
		virtual void onScroll(ScrollEvent &event) { event.ignore(); }

	public:
		friend class Control;
		OSDControl(Control &ref) :OSDUIObj(ref) {
			m_hwnd = NULL;
			m_wndProc = NULL;
			m_originalWndProc = NULL;
			m_backBrush = NULL;
			m_hover = false;
		}
		~OSDControl();
		Control& ref() const { return static_cast<Control&>(OSDUIObj::ref()); }

		//OSDUIObj
		virtual void create();
		virtual void update();
		virtual void destroy();

		//OSDControl
		virtual void setParent(Control *parent);
		virtual void setVisible(bool visible);
		virtual void setEnabled(bool enabled);
		virtual void setPosition(const Point &position);
		virtual void setSize(const Size &size);
		virtual void setVisibilityMode(VisibilityMode visibilityMode);
		virtual Size getFrameSize();
		virtual void setStyle(u32 style);
		virtual void setFont(const FontDesc &desc);
		virtual void setBackgroundColor(const Color& color);
		virtual void setTransparent(bool transparent);
		//	virtual void setDefaultFont();
		virtual void setFocus();

		void redraw();

		//OSD
		HWND handle() { return m_hwnd; }
		WNDPROC wndProc() const { return m_wndProc; }
		WNDPROC originalWndProc() const { return m_originalWndProc; }
		HBRUSH backgroundBrush() const { return m_backBrush; }

		friend void createWindow(OSDControl &osdControl, const TCHAR *windowClass, DWORD exStyle, DWORD style);
		void setWndProc(WNDPROC wndProc) {
			m_wndProc = wndProc;
			WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)wndProc);
			if (!m_originalWndProc)
				m_originalWndProc = oldWndProc;
		}

		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam) { return FALSE; }

		MSG_HANDLER(OnMove) //0x0003
			MSG_HANDLER(OnSize) //0x0005
			MSG_HANDLER(OnSetFocus) //0x0007
			MSG_HANDLER(OnKillFocus) //0x0008
			MSG_HANDLER(OnEnable); //0x000A
		MSG_HANDLER(OnEraseBkgnd) //0x0014
			MSG_HANDLER(OnSetCursor) //0x0020
			MSG_HANDLER(OnNotify) //0x004E
			MSG_HANDLER(OnNCHitTest) //0x0084
			MSG_HANDLER(OnNCPaint) //0x0085
			MSG_HANDLER(OnChar) //0x0102
			MSG_HANDLER(OnCommand) //0x0111
			MSG_HANDLER(OnHScroll) //0x0114
			MSG_HANDLER(OnVScroll) //0x0115
			//	MSG_HANDLER(WM_CTLCOLORMSGBOX,osdControl,OnCtlColorMsgBox) //0x132
			MSG_HANDLER(OnCtlColorEdit) //0x133
			//	MSG_HANDLER(WM_CTLCOLORLISTBOX,osdControl,OnCtlColorListBox) //0x134
			MSG_HANDLER(OnCtlColorBtn) //0x135
			//	MSG_HANDLER(WM_CTLCOLORDLG,osdControl,OnCtlColorDlg) //0x136
			//	MSG_HANDLER(WM_CTLCOLORSCROLLBAR,osdControl,OnCtlColorScrollbar) //0x137
			MSG_HANDLER(OnCtlColorStatic) //0x138
			MSG_HANDLER(OnMouseMove) //0x0200
			MSG_HANDLER(OnLButtonDown)
			MSG_HANDLER(OnLButtonUp)
			MSG_HANDLER(OnRButtonDown)
			MSG_HANDLER(OnRButtonUp)
			MSG_HANDLER(OnMButtonDown)
			MSG_HANDLER(OnMButtonUp)
			MSG_HANDLER(OnMouseWheel)
			MSG_HANDLER(OnMouseLeave) //0x02A3
			MSG_HANDLER(OnPaste) //0x0303
	};



	class OSDWindow : public OSDControl
	{

	protected:

		//Used for Back buffering
		HDC m_backBufferDC;
		HBITMAP m_backBufferBitmap;
		BITMAP m_bmHeader;

		DWORD m_savedStyle; //saved style for child/top-level windows

		//Back Buffering
		void createBackBuffer();
		void flip(HDC hdcDest);
		void onPaint(HDC hdcDest); //main painting function

	protected:
		virtual void onPaintEvent(PaintEvent &event);

	public:
		friend class Window;
		OSDWindow(Window &ref) :OSDControl(ref) {
			m_backBrush = NULL;
			m_backBufferDC = NULL;
			m_backBufferBitmap = NULL;
			memset(&m_bmHeader, 0, sizeof(BITMAP));
		}
		~OSDWindow();
		Window& ref() const { return static_cast<Window&>(OSDControl::ref()); }

		//OSDUIObj
		virtual void create();
		virtual void destroy();
		virtual void update();

		//OSDControl
		virtual void setParent(Control *parent);
		virtual void setSize(const Size &size);
		virtual Size getFrameSize();
		virtual void setBackgroundColor(const Color& color);

		//OSDWindow
		void setTitle(const string& text);
		void setMenuVisible(bool visible);

		void setScrollLength(const Size &contentSize, const Size &workingSize);
		u32 getScrollBarWidth(u32 which);

		//OSDWindow only
		HDC GetDC();
		void ReleaseDC(HDC hdc);

		//Windows messages
		MSG_HANDLER(OnMove)
			MSG_HANDLER(OnSize)
			MSG_HANDLER(OnClose)
			MSG_HANDLER(OnEraseBkgnd) //0x0014
			MSG_HANDLER(OnNotify) //0x004E
			MSG_HANDLER(OnCommand) //0x0111
			MSG_HANDLER(OnHScroll) //0x0114
			MSG_HANDLER(OnVScroll) //0x0115
			MSG_HANDLER(OnMouseWheel)
	};



	class OSDButton : public OSDControl
	{
		//virtual override
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class Button;
		OSDButton(Button &ref);
		Button& ref() const { return static_cast<Button&>(OSDControl::ref()); }

		void setText(const string &text);
		void setTextColor(const Color &color);

		//OSDUIObj methods
		virtual void create();
		virtual void update();
	};



	class OSDRadioButton : public OSDButton
	{
		LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class RadioButton;
		OSDRadioButton(RadioButton &ref);
		RadioButton& ref() const { return static_cast<RadioButton&>(OSDButton::ref()); }

		//OSDUIObj methods
		virtual void create();

		//OSDControl
		virtual Size getAutoSize();

		//OSDRadioButton
		void setChecked(bool checkeded);
		u32 radius();
	};



	class OSDCheckBox : public OSDButton
	{
		LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class CheckBox;
		OSDCheckBox(CheckBox &ref);
		CheckBox& ref() const { return static_cast<CheckBox&>(OSDButton::ref()); }

		//OSDUIObj methods
		virtual void create();
		virtual void update();

		//OSDControl
		virtual Size getAutoSize();

		//OSDCheckBox
		void setChecked(bool checkeded);
	};



	class OSDEditBox : public OSDControl
	{
		MSG_HANDLER(OnSetFocus);
		MSG_HANDLER(OnSetCursor);
		MSG_HANDLER(OnKeyDown); //0x0100
		MSG_HANDLER(OnChar);
		MSG_HANDLER(OnPaste); //0x0303
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void onMouseMoveEvent(MouseEvent &event);
		virtual void onMousePressEvent(MouseEvent &event);
		virtual void onMouseDoubleClickEvent(MouseEvent &event);

	protected:
		bool m_placeholderSet;
		bool m_systemCommand;

	public:
		friend class EditBox;
		OSDEditBox(EditBox &ref);
		EditBox& ref() const { return static_cast<EditBox&>(OSDControl::ref()); }

		virtual void setParent(Control *parent);
		virtual void setStyle(u32 style);

		//
		virtual void create();
		virtual void update();

		//
		void setText(const string &text);
		void setTextColor(const Color &color);
		void setMaxChars(uint maxChars);
		void setEditable(bool editable);
		void setPlaceholderText(const string &text);
		void setPlaceholderTextColor(const Color &color);
		void setPlaceholderTextBehavior(PlaceholderTextBehavior behavior);
		void setPasswordMode(bool passwordMode);
	};



	class OSDComboBox : public OSDControl
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class ComboBox;
		OSDComboBox(ComboBox &ref);
		ComboBox& ref() const { return static_cast<ComboBox&>(OSDControl::ref()); }

		u32 insertItem(ComboBoxItem &item);
		void removeItem(u32 index);
		void reset();
		void sort();
		int setSelection(int row);

		virtual void setSize(const Size &size);
		virtual Size getAutoSize();


		virtual void create();
		virtual void update();
	};





	class OSDLabel : public OSDControl
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		MSG_HANDLER(OnNCHitTest) //0x0084
		MSG_HANDLER(OnEnable); //0x000A
		MSG_HANDLER(OnNCPaint) //0x0085

	protected:
		virtual void onPaintEvent(PaintEvent &event);

	public:
		friend class Label;
		OSDLabel(Label &ref);
		Label& ref() const { return static_cast<Label&>(OSDControl::ref()); }

		void setText(const string& text);
		void setTextColor(const Color &color);
		void setSpacing(int spacing);
		void setJustification(int justification);
		void setAlignment(int alignment);

		Size calcTextSize();

		virtual void create();
		virtual void update();
	};




	class OSDSeparator : public OSDLabel
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		MSG_HANDLER(OnEnable); //0x000A

	protected:
		virtual void onPaintEvent(PaintEvent &event);

	public:
		friend class Separator;
		OSDSeparator(Separator &ref) :OSDLabel(ref) { }
		Separator& ref() const { return static_cast<Separator&>(OSDLabel::ref()); }

		void setLineThickness(u32 thickness);
		void setLineColor(const Color &color);

		virtual void create();
		virtual void update();
	};



	class OSDTextLink : public OSDLabel
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		MSG_HANDLER(OnKillFocus) //0x0008
		MSG_HANDLER(OnSetCursor) //0x0020

	protected:
		virtual void onPaintEvent(PaintEvent &event);
		virtual void onMouseMoveEvent(MouseEvent &event);
		virtual void onMouseLeaveEvent(MouseEvent &event);
		virtual void onMousePressEvent(MouseEvent &event);
		virtual void onMouseReleaseEvent(MouseEvent &event);

	public:
		friend class TextLink;
		OSDTextLink(TextLink &ref) :OSDLabel(ref) { }
		TextLink& ref() const { return static_cast<TextLink&>(OSDLabel::ref()); }

		void setHoverColor(const Color &color);
		void setHoverFont(const FontDesc &fd);
		void setVisitedColor(const Color &color);
		void setVisitedFont(const FontDesc &fd);
		void setProperties(TextLink::Properties props);

		bool m_bClick;
		bool m_bHover;

		virtual void create();
		virtual void update();
	};



	class OSDListView : public OSDControl
	{
		MSG_HANDLER(OnEraseBkgnd);
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void onPaintEvent(PaintEvent &event);

	public:
		friend class ListView;
		OSDListView(ListView &ref);
		ListView& ref() const { return static_cast<ListView&>(OSDControl::ref()); }

		//
		virtual void create();
		virtual void update();

		virtual void setStyle(u32 style);
		virtual void setBackgroundColor(const Color& color);
		virtual void setTransparent(bool transparent);

		//
		int insertItem(ListViewItem &item, uint indexPos);
		void reset();
		void sort();
		bool removeItem(uint index);
		int changeItemIndex(uint curIndex, uint newIndex);
		int selectItem(int index, bool select);

		void setWorkArea(Rect &area);

		void updateItem(ListViewItem &item);
	};

	class OSDGridView : public OSDControl
	{
		MSG_HANDLER(OnEraseBkgnd);
		MSG_HANDLER(OnHScroll);
		MSG_HANDLER(OnVScroll);
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void onPaintEvent(PaintEvent &event);

	public:
		friend class GridView;
		OSDGridView(GridView &ref);
		GridView& ref() const { return static_cast<GridView&>(OSDControl::ref()); }

		void setHeaderVisible(bool visible);
		void setHeaderText(int col, const string &text);
		void setCellCount(int rowCount, int colCount);
		void insertRow(int at);
		void insertColumn(int at);
		void setItem(int row, int column, ListViewItem *item);
		int getColumnWidth(int col);
		void clear();

		virtual void create();
		virtual void update();
	};


	class OSDGroupBox : public OSDControl
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class GroupBox;
		OSDGroupBox(GroupBox& ref);
		GroupBox& ref() const { return reinterpret_cast<GroupBox&>(OSDControl::ref()); }

		//OSDControl methods
		virtual void create();
		virtual void update();

		//OSDGroupBox
		void setText(const string& text);

		//OSD methods
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
		LRESULT OnEraseBkgnd(WPARAM wParam, LPARAM lParam);
	};

	class OSDStatusBar : public OSDControl
	{

	protected:
		virtual void onPaintEvent(PaintEvent &event);

	public:
		friend class StatusBar;
		OSDStatusBar(StatusBar &ref) : OSDControl(ref) { }
		StatusBar& ref() const { return static_cast<StatusBar&>(OSDControl::ref()); }

		void setMargin(const Rect &margin);
		void setText(const string &text);
		void setTextColor(const Color &c);
		void setFormat(u32 format);
		void setFont(const FontDesc &desc);

		//OSDUIObj methods
		virtual void create();
		virtual void update();
	};





	class OSDMenuObject : public OSDUIObj
	{
		MenuObject &ref;

	public:
		friend class MenuObject;
		OSDMenuObject(MenuObject &ref) : OSDUIObj(ref), ref(ref) { }
	};

	class OSDMenuItem : public OSDMenuObject
	{
		MenuItem &ref;

		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class MenuItem;
		OSDMenuItem(MenuItem &ref) : OSDMenuObject(ref), ref(ref) { }

		void setText(const string &text);
	};


	class OSDMenuSeparator : public OSDMenuObject
	{
		MenuSeparator &ref;

	protected:
		void SetRef(MenuSeparator &ref);

	public:
		friend class MenuSeparator;
		OSDMenuSeparator(MenuSeparator &ref) :OSDMenuObject(ref), ref(ref) { };
	};



	class OSDMenu : public OSDMenuObject
	{
		Menu &ref;

	protected:
		HMENU m_hMenu;

	public:
		friend class OSDMenuBar;
		friend class MenuBar;
		friend class Menu;

		OSDMenu(Menu &ref) :OSDMenuObject(ref), ref(ref) { }

		void setText(const string &text);

		void append(Menu& submenu);
		void append(MenuSeparator& separator);
		void append(MenuItem &item);

		virtual void create();
		virtual void destroy();
	};

	class OSDMenuBar : public OSDUIObj
	{
		MenuBar &ref;

	protected:
		HMENU m_hMenu;

	public:
		friend class Menu;
		OSDMenuBar(MenuBar &ref) :OSDUIObj(ref), ref(ref) { };

		void setParent(Control& control);
		void setVisible(bool visible);

		void append(Menu& submenu);
		void append(MenuItem &item);

		virtual void create();
		virtual void destroy();

		//OSD
		HMENU handle() const { return m_hMenu; }
	};



	class OSDFileDialog
	{
		FileDialog &m_ref;

		UINT_PTR OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);
		UINT_PTR OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam);

	public:
		friend class FileDialog;
		friend UINT_PTR CALLBACK _OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
		OSDFileDialog(FileDialog &ref) : m_ref(ref) { }
		FileDialog& ref() const { return m_ref; }

		void setOpenMode(u32 mode) { }
		void setDefaultFileName(const string &fileName) { }
		int addFilter(const string &description, const string &patterns) { return 0; }
		void setCaption(const string &caption) { }
		int open(Control *parent);
	};



	class OSDMessage
	{
		static int winmessage(HWND hwnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
	public:
		static void msg(Window *parent, const string &text);
		static void warning(Window *parent, const string &text);
		static void error(Window *parent, const string &text);
		static Message::message_result question(Window *parent, const string &text);
	};



	class OSDUITools
	{

	public:
		static Size getTextSize(const string& text, const FontDesc &fontDesc);
		static bool chooseColor(Window *parent, Color &retColor, const Color *defaultColor = NULL);
	};


	//todo: change?
	class PaintingDeviceGDI : public PaintingDevice
	{
		void __updateBrush();

		HDC m_handler;
		HPEN m_hPen;
		HBRUSH m_hBrush;
		HBRUSH m_hBackBrush;

	public:
		friend class OSDWindow;
		PaintingDeviceGDI(HDC handler);
		~PaintingDeviceGDI();

		void setPen(const Pen& pen);
		void setBrush(const Brush& brush);
		void setBackgroundBrush(const Brush& backBrush);
		void setBackgroundMode(BackgroundMode bgMode);
		void setFont(const Font& font);

		void line(int x1, int y1, int x2, int y2);
		void rectangle(int x, int y, int width, int height);
		void circle(int centerX, int centerY, int radius);
		void ellipse();
		void fillRect(int x, int y, int width, int height, const Color& color);
		void drawBitmap(int x, int y, const Bitmap &bitmap);
		void plot(int x, int y, const Color& color);
		void drawText(int x, int y, const string &text);
		void setTransform(const Transform& transform);
	};

	class OSDPainter
	{
		Painter &m_ref;
		HPEN m_hPen;
		HBRUSH m_hBrush;
		HBRUSH m_hBackBrush;

		HWND m_hwnd;
		PAINTSTRUCT m_ps;

	public:
		friend class Painter;
		OSDPainter(Painter& ref) : m_ref(ref) { m_hwnd = NULL; }

		bool begin(PaintingDevice *device)
		{
			return true;
		}

		bool begin(Control *control)
		{
			if (!control)
				return false;
			OSDControl &ref = control->osdRef();
			m_hwnd = ref.handle();
			HDC hdc = ::BeginPaint(m_hwnd, &m_ps);

			PaintingDevice *device;
			device = control->m_paintDevice = new PaintingDeviceGDI(hdc);

			device->setPen(m_ref.pen());
			device->setBrush(m_ref.brush());
			device->setFont(m_ref.font());

			return true;
		}

		bool end()
		{
			if (m_hwnd)
				::EndPaint(m_hwnd, &m_ps);
			return true;
		}

	};

}

#include "osdscrollbar.h"
#include "osdscrollview.h"

#endif