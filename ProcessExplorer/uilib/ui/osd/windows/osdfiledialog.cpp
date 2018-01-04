
//OSDFileDialog
// File: osdfiledialog.cpp
// Last edit: 08/08/2014 02:10 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

#include <stdio.h>
#include <tchar.h>

namespace uilib {

	UINT_PTR CALLBACK _OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		HWND hParentDlg = ::GetParent(hdlg); //hDlg is just a wrapper, actual dialog handle is the parent of this

		switch (uiMsg) {
		case WM_DESTROY: /*0x02*/ break;
		case WM_SIZE: /*0x05*/ break;

		case WM_SHOWWINDOW: /*0x18*/ break;
		case WM_CHILDACTIVATE: /*0x22*/ break;
		case WM_SETFONT: /*0x30*/ break;
		case WM_WINDOWPOSCHANGING: /*0x46*/ break;
		case WM_WINDOWPOSCHANGED: /*0x47*/ break;
		case WM_NOTIFY: /*0x4E*/
		{
			LPOFNOTIFY lpofn = (LPOFNOTIFY)lParam;
			OSDFileDialog *fileDialog = (OSDFileDialog*)lpofn->lpOFN->lCustData;
			return fileDialog->OnNotify(hParentDlg, wParam, lParam);
		}

		case WM_NCDESTROY: /*0x82*/ break;
		case WM_NCCALCSIZE: /*0x83*/ break;
		case WM_INITDIALOG: //0x110
		{
			LPOPENFILENAME lpofn = (LPOPENFILENAME)lParam;
			OSDFileDialog *fileDialog = (OSDFileDialog*)lpofn->lCustData;
			return fileDialog->OnInitDialog(hParentDlg, wParam, lParam);
		}

		default:
		{
			int msg = uiMsg;
			break;
		}
		}

		//From MSDN:
		//If the hook procedure returns zero, the default dialog box procedure processes the message. 
		//If the hook procedure returns a nonzero value, the default dialog box procedure ignores the message.
		//For the CDN_SHAREVIOLATION and CDN_FILEOK notification messages, the hook procedure should return a nonzero value to indicate that it has used the SetWindowLong function to set a nonzero DWL_MSGRESULT value.
		return 0;
	}


	UINT_PTR OSDFileDialog::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
	{
		FileDialog &fileDialog = ref();
		if (!fileDialog.m_selectedFileNames.empty())
			fileDialog.m_selectedFileNames.clear();
		fileDialog.m_currentPath = "";
		fileDialog.m_result = 0;
		return 0;
	}

	UINT_PTR OSDFileDialog::OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam)
	{
		LPOFNOTIFY lpofn = (LPOFNOTIFY)lParam;
		int code = lpofn->hdr.code;

		FileDialog &fileDialog = ref();

		switch (code) {
		case CDN_INITDONE:
			break;

		case CDN_SELCHANGE:
			break;

		case CDN_FOLDERCHANGE:
			break;

		case CDN_SHAREVIOLATION:
			break;

		case CDN_HELP:
			break;

		case CDN_FILEOK:
		{
			long msgResult;
			string folderPath;
			string fileNames; //list of selected files
			string fileName; //found file name from fileNames

			msgResult = ::SendMessage(hDlg, CDM_GETFOLDERPATH, (WPARAM)0, (LPARAM)NULL);
			if (msgResult < 0) {
				fileDialog.m_result = 2;
				break;
			}

			folderPath.resize(msgResult);
			msgResult = ::SendMessage(hDlg, CDM_GETFOLDERPATH, (WPARAM)msgResult, (LPARAM)folderPath.data());
			if (msgResult < 0) {
				fileDialog.m_result = 2;
				break;
			}

			//Get files
			msgResult = ::SendMessage(hDlg, CDM_GETSPEC, (WPARAM)0, (LPARAM)NULL);
			if (msgResult < 0) {
				fileDialog.m_result = 2;
				break;
			}

			fileNames.resize(msgResult);
			msgResult = ::SendMessage(hDlg, CDM_GETSPEC, (WPARAM)msgResult, (LPARAM)fileNames.data());
			if (msgResult < 0) {
				fileDialog.m_result = 2;
				break;
			}

			if (lpofn->lpOFN->Flags & OFN_ALLOWMULTISELECT) {

				char *c = fileNames.data();
				char *fnameStart = 0;
				char *fnameEnd = 0;
				int inQuotes = false;
				int charFound = false;
				int fileCount = 0;

				while (*c) {
					if (*c == ' ') {
						if (!inQuotes && charFound) {
							++fileCount;
							fnameEnd = c;
							charFound = false;
						}
					}
					else if (*c == '\"') {
						if (inQuotes) {
							if (charFound) {
								++fileCount;
								fnameEnd = c;
								charFound = false;
							}
						}
						else {
							inQuotes = true;
							charFound = false;
						}
					}
					else {
						charFound = true;
						if (!fnameStart)
							fnameStart = c;
					}

					if (fnameEnd) {

						//"remove" white spaces
						while (fnameEnd > fnameStart) {
							if (*fnameEnd == ' ' || *fnameEnd == '\"')
								--fnameEnd;
							else
								break;
						}

						fileName = "";
						int count = (fnameEnd + 1) - fnameStart;
						fileName.append(fnameStart, count);
						fileDialog.m_selectedFileNames.push_back(fileName);

						fnameStart = NULL;
						fnameEnd = NULL;
					}

					++c;
				}

				if (fnameStart) {
					fnameEnd = c - 1;
					//"remove" white spaces
					while (fnameEnd > fnameStart) {
						if (*fnameEnd == ' ' || *fnameEnd == '\"')
							--fnameEnd;
						else
							break;
					}
					fileName = "";
					int count = (fnameEnd + 1) - fnameStart;
					fileName.append(fnameStart, count);
					fileDialog.m_selectedFileNames.push_back(fileName);
				}

			} /*OFN_ALLOWMULTISELECT*/
			else {
				fileName = fileNames;
				fileDialog.m_selectedFileNames.push_back(fileName);
			}

			fileDialog.m_currentPath = folderPath;
			fileDialog.m_result = 1;
			::EndDialog(hDlg, 1);
			break;
		}

		case CDN_TYPECHANGE:
			break;

		case CDN_INCLUDEITEM:
			break;

		default:
		{
			int c = code;
			break;
		}
		}

		return 0;
	}

	int OSDFileDialog::open(Control *parent)
	{
		BOOL res;

		string filter;
		string defFileName = ref().m_defaultFileName;

		defFileName.reserve(MAX_PATH);

		//Lets build Windows compatible filter string
		uint pos = 0;
		for (uint i = 0; i < ref().m_filterPatterns.size(); ++i) {
			filter.insert(pos, ref().m_filterDescription[i]);
			pos += ref().m_filterDescription[i].size();
			filter.insert(pos, ref().m_filterPatterns[i]);
			pos += ref().m_filterPatterns[i].size();
		}
		filter.insert(pos, '\0');


		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = parent ? parent->osdRef().handle() : NULL;
		ofn.lpstrFilter = filter.constData();
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = defFileName.data();
		ofn.nMaxFile = max(defFileName.bufferSize(), MAX_PATH);
		ofn.hInstance = ::GetModuleHandle(NULL);
		ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_DONTADDTORECENT;
		ofn.lpfnHook = _OFNHookProc;
		ofn.lpstrTitle = ref().m_caption.data();
		ofn.lCustData = (LPARAM)this;

		u32 openMode = ref().m_openMode;

		if (openMode == SAVE) {
			ofn.Flags |= OFN_OVERWRITEPROMPT;
			res = ::GetSaveFileName(&ofn);
		}
		else
		{
			if (openMode == OPENMULTI)
				ofn.Flags |= OFN_ALLOWMULTISELECT;

			ofn.Flags |= OFN_FILEMUSTEXIST;
			res = ::GetOpenFileName(&ofn);

			if (res == FALSE) {
				/*
	#define FNERR_BUFFERTOOSMALL 0x3003
				DWORD err = ::CommDlgExtendedError();
				if(err == FNERR_BUFFERTOOSMALL) {
					ofn.nMaxFile = 256;
					res = ::GetOpenFileName(&ofn) != 0;
				}
				*/
			}
		}

		return res != FALSE;
	}

}