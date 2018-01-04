
//UITools
// File: uitools.h
// Last edit: 07/08/2014 02:38 (UTC-3)
// Author: CGR

#ifndef __UITOOLS_H__
#define __UITOOLS_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

#define OPEN 0
#define SAVE 1
#define OPENMULTI 2

	class Message
	{
	public:
		enum message_result { cancel = 0, yes, no };
		static void msg(Window *parent, const string &text);
		static void warning(Window *parent, const string &text);
		static void error(Window *parent, const string &text);
		static Message::message_result question(Window *parent, const string &text);
	};


	class UITools
	{
	public:
		static const string *openSaveFile(Window &parent, unsigned long mode, const char *filename, const char *filter, std::vector<string> *pFileList, int *pFileCount);
		static Size getTextSize(const string &text, const FontDesc &fontDesc);
		static bool chooseColor(Window *parent, Color &retColor, const Color *defaultColor = NULL);
	};

}

#endif