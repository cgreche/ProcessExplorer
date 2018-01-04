
//FileDialog
// File name: filedialog.h
// Last edit: 06/08/2014 08:18 (UTC-3)
// Author: CGR

#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

namespace uilib {

	class FileDialog
	{
		OSDFileDialog &m_osdRef;

	public:
		enum OpenMode { Save, Open, OpenMulti };

		FileDialog();
		~FileDialog();

		int open(Control *parent);

		void setOpenMode(OpenMode mode);
		OpenMode openMode() const { return m_openMode; }

		void setDefaultFileName(const string &fileName);
		int addFilter(const string &description, const string &patterns);
		void clearFilterList();
		void setCaption(const string &caption);

		string& currentPath() { return m_currentPath; }
		void getFilter(int index, string *description, string *patterns);
		uint selectedCount() const { return m_selectedFileNames.size(); }
		std::vector<string> &selectedFileNames() { return m_selectedFileNames; }
		int openResult() const { return m_result; }

		friend class OSDFileDialog;
		OSDFileDialog& osdRef() const { return m_osdRef; }

	protected:
		OpenMode m_openMode;
		string m_currentPath;
		std::vector<string> m_selectedFileNames;
		std::vector<string> m_filterDescription;
		std::vector<string> m_filterPatterns;
		string m_caption;
		string m_defaultFileName;
		int m_result;
	};

}

#endif