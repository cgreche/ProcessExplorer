
//UITools
// File: uitools.cpp
// Last edit: 05/01/2018 00:22 (UTC-3)
// Author: CGR

#include "uibase.h"

namespace uilib {

	void Message::msg(Window *parent, const string &text)
	{
		OSDMessage::msg(parent, text);
	}


	void Message::warning(Window *parent, const string &text)
	{
		OSDMessage::warning(parent, text);
	}

	void Message::error(Window *parent, const string &text)
	{
		OSDMessage::error(parent, text);
	}


	Message::message_result Message::question(Window *parent, const string &text)
	{
		return OSDMessage::question(parent, text);
	}


	const string *UITools::openSaveFile(Window &parent, unsigned long openMode, const char *filename, const char *filter, std::vector<string> *pFileList, int *pFileCount)
	{
		//todo: deprecate
		static string ret;

		FileDialog fileDialog;
		fileDialog.setOpenMode((FileDialog::OpenMode)openMode);
		fileDialog.setDefaultFileName(filename);
		fileDialog.addFilter(filter, filter);

		int res = fileDialog.open(&parent);
		if (res) {
			if (pFileList)
				*pFileList = fileDialog.selectedFileNames();
			if (pFileCount)
				*pFileCount = fileDialog.selectedCount();
			ret = fileDialog.currentPath();
			return &ret;
		}

		return NULL;
	}

	Size UITools::getTextSize(const string &text, const FontDesc &fontDesc)
	{
		return OSDUITools::getTextSize(text, fontDesc);
	}

	bool UITools::chooseColor(Window *parent, Color &retColor, const Color *defaultColor)
	{
		return OSDUITools::chooseColor(parent, retColor, defaultColor);
	}

}