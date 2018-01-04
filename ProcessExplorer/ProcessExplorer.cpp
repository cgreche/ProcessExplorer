#define UNICODE

#include <windows.h>
#include <sstream>
#include "ProcessSpawner.h"

#include <uilib/ui/uibase.h>

using namespace uilib;

class MainWindow : public Window {
	VerticalLayout layout;
	EditBox editFilter;
	GridView grid;
	HorizontalLayout layoutLoadDll;
		EditBox editDllPath;
		Button buttonLoadDll;

	std::vector<ProcessBasicInfo> m_processList;

public:
	friend void editFilter_onChange(EditBox &edit);
	MainWindow();

	void step() {
		if (editFilter.text().length() > 0) {
			string &filter = editFilter.text();

		}
	}

	virtual void onCloseEvent() {
		UI::UIQuit();
	}
};


static void editFilter_onChange(EditBox &edit) {
	MainWindow *mainWindow = (MainWindow*)edit.param();
	mainWindow->grid.clear();
	mainWindow->grid.setColumnCount(2);
	for (int i = 0; i < mainWindow->m_processList.size(); ++i) {
		std::stringstream ss;
		std::string str;
		std::string filter = edit.text().data();
		str = mainWindow->m_processList[i].name;
		if (str == "" || str.find(filter) != -1) {
			mainWindow->grid.insertRow();
			int index = mainWindow->grid.rowCount() - 1;
			mainWindow->grid.setItem(index, 0, new ListViewItem(string::fromInteger(mainWindow->m_processList[i].id)));
			mainWindow->grid.setItem(index, 1, new ListViewItem(mainWindow->m_processList[i].name));
		}
	}
}

static void editFilter_onAction(GridView &grid) {
	MainWindow *mainWindow = (MainWindow*)grid.param();
}

static void load_onClick(Button &button) {
	MainWindow *mainWindow = (MainWindow*)button.param();
	std::vector<string> fileList;
	UITools::openSaveFile(*mainWindow, OPEN, "", "*.dll", &fileList, NULL);
}

MainWindow::MainWindow() {
	editFilter.setOnChange(editFilter_onChange);
	editFilter.setParam(this);
	grid.setColumnCount(2);
	grid.setHeaderVisible(true);
	grid.setHeaderText(0, "id");
	grid.setHeaderText(1, "name");
	grid.setStyle(grid.style() | CS_Border);

	buttonLoadDll.setText("Load");
	buttonLoadDll.setOnClick(load_onClick);
	buttonLoadDll.setParam(this);

	layoutLoadDll.append(editDllPath);
	layoutLoadDll.append(buttonLoadDll);

	layout.append(editFilter);
	layout.append(grid);
	layout.append(layoutLoadDll);
	layout.setMargin(10);
	setLayout(&layout);

	std::stringstream ss;
	std::string str;

	ProcessSpawner::GetProcessList(m_processList);

	for (int i = 0; i < m_processList.size(); ++i) {
		ss << m_processList[i].id;
		ss << " - ";
		ss << m_processList[i].name;
		ss << "\n";

		grid.insertRow();
		grid.setItem(i, 0, new ListViewItem(string::fromInteger(m_processList[i].id)));
		grid.setItem(i, 1, new ListViewItem(m_processList[i].name));
	}

	/*
	CProcess* process = ProcessSpawner::Create("F:/Steam/steamapps/common/StreetFighterV/StreetFighterV.exe", CThread::SUSPENDED);
	CThread* thread = process->threadList()[0];
	thread->resume();
	::MessageBoxA(NULL, ss.str().c_str(), 0, MB_TASKMODAL);
	*/
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainWindow mainWindow;
	mainWindow.setSize(Size(800, 600));
	mainWindow.setVisible(true);

	while (uilib::UI::UIProcess()) {
		mainWindow.step();
	}

	return 0;
}
