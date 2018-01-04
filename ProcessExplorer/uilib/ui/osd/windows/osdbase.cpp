
//OSDBase
// File: windows/osdbase.cpp
// Last edit: 05/08/2014 00:34 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

#define WINBASEID 100

	std::list<OSDUIObj*> *OSDBase::objects;
	u32 OSDBase::nextId;

	extern LRESULT CALLBACK UIWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	extern LRESULT CALLBACK UIFrameProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	extern TCHAR g_szWindowClassName[];
	extern TCHAR g_szFrameClassName[];

	void OSDBase::UIInit()
	{
		static bool init = false;
		if (init)
			return;
		init = true;

		::CoInitialize(0);
		::InitCommonControls();

		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = ::DefWindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = ::GetModuleHandle(0);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = g_szWindowClassName;

		if (!::RegisterClass(&wc))
		{
			::MessageBox(0, "Failed to register Class", 0, MB_ICONERROR);
			exit(0);
		}

		objects = new std::list<OSDUIObj*>;
		if (!objects)
		{
			::MessageBox(0, "OSD initialization error: out of memory", 0, MB_ICONERROR);
			return;
		}

		nextId = WINBASEID;
	}

	HWND GetNextTabStopControl(HWND fromHwnd)
	{
		HWND next = ::GetWindow(fromHwnd, GW_HWNDNEXT);
		for (;;) {
			if (IsWindowEnabled(next)) {
				DWORD style = ::GetWindowLong(next, GWL_STYLE);
				if (style & WS_TABSTOP) {
					break;
				}
			}

			next = ::GetWindow(next, GW_HWNDNEXT);
			if (!next)
				break;
		}

		return next;
	}

	bool OSDBase::UIProcess()
	{
		MSG msg;
		while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {

			HWND hwndParent = ::GetParent(msg.hwnd);
			HWND hwndToUse = hwndParent ? hwndParent : msg.hwnd;

			if (msg.message == WM_KEYFIRST && msg.wParam == VK_TAB) {
				if (msg.hwnd == ::GetWindow(msg.hwnd, GW_HWNDLAST)) {
					HWND testNext = ::GetWindow(msg.hwnd, GW_HWNDNEXT);
					//	HWND firstWnd = ::GetWindow(hwndToUse,GW_HWNDFIRST);
					//	::SetFocus(firstWnd);
				}
			}


			if (msg.message == WM_KEYFIRST) {
				//			HWND test = ::GetWindow(hwndToUse,GW_HWNDNEXT);
				//			::ShowWindow(test,SW_HIDE);
			}

			BOOL res = ::IsDialogMessage(hwndToUse, &msg);
			//little hacky
			if (msg.message == WM_KEYDOWN || msg.wParam == VK_ESCAPE)
				int a = 1;
			if(!res || (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else {
				int a = 1;
				/*
				if(msg.message != WM_MOUSEMOVE
					&& msg.message != WM_TIMER
					&& msg.message != WM_NCMOUSEMOVE //0xA0
					&& msg.message != WM_KEYUP
					&& msg.message != WM_SYSKEYDOWN //0x0104
					&& msg.message != WM_SYSCOMMAND
					&& msg.message != 0x118
					&& msg.message != WM_MOUSEMOVE
					&& msg.message != WM_LBUTTONDOWN
					&& msg.message != WM_LBUTTONUP //0x202
					&& msg.message != WM_PAINT
					&& msg.message != WM_MOUSELEAVE
				) {
					int a = 1;

					char test[100];
					::GetWindowText(msg.hwnd,test,100);

					if(msg.message == WM_KEYFIRST && msg.wParam == VK_TAB) {
						if(GetNextTabStopControl(msg.hwnd) == NULL) {
							HWND next;
							for(;;) {
								next = ::GetWindow(hwndToUse,GW_HWNDNEXT);
								if(!next) {
								//	next = ::GetParent(hwndToUse);
								//	if(!next) {
										next = GetWindow(hwndToUse,GW_HWNDFIRST);
								//	}
								}

								hwndToUse = next;
								if(!hwndToUse)
									break;

								HWND child = ::GetWindow(hwndToUse,GW_CHILD);
								for(;;) {
									DWORD style = ::GetWindowLong(child,GWL_STYLE);
									if(style & WS_TABSTOP) {
										break;
									}

									child = GetNextTabStopControl(child);
									break;
								}

								if(child) {
									::SetFocus(child);
									break;
								}
							}
						}
					}
				}
				*/
				}

			if (msg.message == WM_QUIT) {
				if (objects)
					delete objects;
				return false;
			}

		}

		return true;
	}


	void OSDBase::UIQuit()
	{
		::PostQuitMessage(0);
	}




	OSDUIObj *OSDBase::find(u32 id)
	{
		std::list<OSDUIObj*>::iterator it;
		for (it = objects->begin(); it != objects->end(); ++it)
		{
			OSDUIObj *obj = *it;
			if (obj->id() == id)
				return obj;
		}
		return NULL;
	}

}