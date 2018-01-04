
// UI Base
// File: uibase.h
// Last edit: 25/10/2017 05:51
// Author: CGR

#ifndef __UIBASE_H__
#define __UIBASE_H__

#pragma warning(disable:4355) //'this' : used in base member initializer list

#include "../types.h"
#include "../lib/point.h"
#include "../lib/size.h"
#include "../lib/rect.h"
#include "../lib/string.h"

#include <vector>
#include <list>

namespace uilib {

	//Mouse buttons state
#define MOUSE_BUTTON_LEFT_PRESSED 1
#define MOUSE_BUTTON_RIGHT_PRESSED 2
#define MOUSE_BUTTON_MIDDLE_PRESSED 4
#define MOUSE_BUTTON_4_PRESSED 8
#define MOUSE_BUTTON_5_PRESSED 16
#define MOUSE_BUTTON_LEFT_RELEASED 32
#define MOUSE_BUTTON_RIGHT_RELEASED 64
#define MOUSE_BUTTON_MIDDLE_RELEASED 128
#define MOUSE_BUTTON_4_RELEASED 256
#define MOUSE_BUTTON_5_RELEASED 512

	class UI
	{
		static bool m_uiinit;

	public:
		static void UIInit();
		static int UIProcess();
		static void UIQuit();
	};

	typedef struct _FontDesc FontDesc;
	extern FontDesc g_pUIdefFontDesc;

	class OSDFont;

	class OSDUIObj;
	class OSDWindowObject;
	class OSDWindow;

	class OSDFrame;
	class OSDLayout;
	class OSDControl;

	class OSDButton;
	class OSDRadionButton;
	class OSDCheckBox;
	class OSDEditBox;
	class OSDLabel;
	class OSDSeparator;
	class OSDTextLink;
	class OSDListView;
	class OSDGridView;
	class OSDComboBox;
	class OSDGroupBox;
	class OSDVerticalScrollBar;
	class OSDHorizontalScrollBar;
	class OSDStatusBar;
	class OSDScrollView;

	class OSDMenuBar;
	class OSDMenuObject;
	class OSDMenu;
	class OSDMenuItem;
	class OSDMenuSeparator;

	class OSDMessage;
	class OSDUITools;

	class OSDFileDialog;
	class OSDPainter;
}

#include "uiobj.h"
#include "font.h"

//Painting
#include "painting/color.h"
#include "painting/palette.h"
#include "painting/rgb.h"
#include "painting/bitmap.h"
#include "painting/transform.h"
#include "painting/paintingdevice.h"
#include "painting/painter.h"

#include "event.h"
//Layouts
#include "layout/layout.h"
#include "layout/linearlayout.h"
#include "layout/horizontallayout.h"
#include "layout/verticallayout.h"

#include "control.h"
#include "window.h"
#include "menu.h"
#include "controls/button.h"
#include "controls/radiobutton.h"
#include "controls/checkbox.h"
#include "controls/editbox.h"
#include "controls/label.h"
#include "controls/separator.h"
#include "controls/textlink.h"
#include "controls/listview.h"
#include "controls/gridview.h"
#include "controls/scrollbar.h"
#include "controls/combobox.h"
#include "controls/groupbox.h"
#include "controls/statusbar.h"
#include "controls/scrollview.h"
#include "uitools.h"
#include "dialogs/filedialog.h"

#ifdef _WIN32
#include "osd\\windows\\osdbase.h"
#endif

#endif