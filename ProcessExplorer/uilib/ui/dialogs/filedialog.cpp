
//FileDialog
// File name: filedialog.cpp
// Last edit: 29/11/2014 12:55 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	FileDialog::FileDialog()
		: m_osdRef(*new OSDFileDialog(*this)),
		m_openMode(Open),
		m_result(0)
	{

	}

	FileDialog::~FileDialog()
	{
	}

	void FileDialog::setOpenMode(OpenMode mode)
	{
		m_openMode = mode;
		m_osdRef.setOpenMode(mode);
	}

	void FileDialog::setDefaultFileName(const string &fileName)
	{
		m_defaultFileName = fileName;
		m_osdRef.setDefaultFileName(fileName);
	}

	int FileDialog::addFilter(const string &description, const string &patterns)
	{
		int retIndex = m_filterPatterns.size();
		m_filterDescription.push_back(description);
		m_filterPatterns.push_back(patterns);
		m_osdRef.addFilter(description, patterns);
		return retIndex;
	}

	void FileDialog::getFilter(int index, string *description, string *patterns)
	{
		if (index >= (int)m_filterPatterns.size())
			return;
		if (description)
			*description = m_filterDescription[index];
		if (patterns)
			*patterns = m_filterPatterns[index];
	}

	void FileDialog::setCaption(const string &caption)
	{
		m_caption = caption;
		m_osdRef.setCaption(caption);
	}

	int FileDialog::open(Control *parent)
	{
		return m_osdRef.open(parent);
	}

}