#define UNICODE

#include <windows.h>
#include <sstream>
#include "ProcessSpawner.h"

#include <uilib/ui/uibase.h>

using namespace uilib;

class MainWindow : public Window {
	VerticalLayout layout;
	HorizontalLayout layoutFilter;
		Label labelFilter;
		EditBox editFilter;
	GridView grid;
	HorizontalLayout layoutLoadDll;
		Label labelLoadDll;
		EditBox editDllPath;
		Button buttonSearchDll;
		Button buttonLoadDll;
	HorizontalLayout layoutLoadProcess;
		Label labelLoadProcess;
		EditBox editProcessPath;
		Button buttonSearchProcess;
		Button buttonLoadProcess;

	std::vector<ProcessBasicInfo> m_processList;

public:
	friend void editFilter_onChange(EditBox &edit);
	friend void buttonSearchDll_onClick(Button &button);
	friend void buttonLoadDll_onClick(Button &button);
	friend void buttonSearchProcess_onClick(Button &button);
	friend void buttonLoadProcess_onClick(Button &button);
	MainWindow();

	virtual void onCloseEvent() {
		UI::UIQuit();
	}
};


static void editFilter_onChange(EditBox &edit) {
	MainWindow *mainWindow = (MainWindow*)edit.param();
	mainWindow->grid.deleteRows();
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

static void buttonSearchDll_onClick(Button &button) {
	MainWindow *mainWindow = (MainWindow*)button.param();
	std::vector<string> fileList;
	const string *retPath = UITools::openSaveFile(*mainWindow, OPEN, "", "*.dll", &fileList, NULL);
	if (fileList.size() > 0) {
		string path = *retPath;
		path += "\\";
		path += fileList[0];
		mainWindow->editDllPath.setText(path);
	}
}

static void buttonLoadDll_onClick(Button &button) {
	MainWindow *mainWindow = (MainWindow*)button.param();
	string &dllPath = mainWindow->editDllPath.text();
	if (dllPath == "")
		return;

	//todo: get selected row
	if (mainWindow->grid.selectedRow() == -1)
		return;
	ListViewItem* item = mainWindow->grid.item(mainWindow->grid.selectedRow(), 0);
	if (item) {
		int pid = item->text().toInteger();
		CProcess* process = ProcessSpawner::Open(pid);
		if (!process) {
			Message::error(NULL, "Couldn't open process");
			return;
		}

		//bool is32BitProcess = !process->is64BitProcess();

		FARPROC loadLibraryAddr = ::GetProcAddress(::GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		BYTE *stub = process->alloc(256);
		process->write(stub, (void*)dllPath.data(), dllPath.length());
		CThread* thread = process->createThread((LPTHREAD_START_ROUTINE)loadLibraryAddr, stub, CThread::NORMAL);
		if (thread) {
			thread->wait();
			thread->close();
		}
		process->free(stub);
		process->close();
	}
}

static void buttonSearchProcess_onClick(Button &button) {
	MainWindow *mainWindow = (MainWindow*)button.param();
	std::vector<string> fileList;
	const string *retPath = UITools::openSaveFile(*mainWindow, OPEN, "", "*.exe", &fileList, NULL);
	if (fileList.size() > 0) {
		string path = *retPath;
		path += "\\";
		path += fileList[0];
		mainWindow->editProcessPath.setText(path);
	}
}

static void buttonLoadProcess_onClick(Button &button) {
	MainWindow *mainWindow = (MainWindow*)button.param();
	string &appName = mainWindow->editProcessPath.text();
	string &dllName = mainWindow->editDllPath.text();
	if (appName == "" || dllName == "")
		return;
	CProcess *process = ProcessSpawner::Create(appName.data(), CThread::SUSPENDED);
	if (process) {
		CThread *mainThread = process->threadList()[0];

		FARPROC loadLibraryAddr = ::GetProcAddress(::GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		BYTE *stub = process->alloc(256);
		process->write(stub, (void*)dllName.data(), dllName.length());
		CThread* thread = process->createThread((LPTHREAD_START_ROUTINE)loadLibraryAddr, stub, CThread::NORMAL);
		if (thread) {
			thread->wait();
			thread->close();
		}
		process->free(stub);

		mainThread->resume();
		mainThread->wait();
		process->close();
	}
}

MainWindow::MainWindow() {
	labelFilter.setText("Filter");
	editFilter.setOnChange(editFilter_onChange);
	editFilter.setParam(this);
	grid.setColumnCount(2);
	grid.setHeaderVisible(true);
	grid.setHeaderText(0, "id");
	grid.setHeaderText(1, "name");
	grid.setStyle(grid.style() | CS_Border);

	labelLoadDll.setText("Load DLL");
	buttonSearchDll.setText("Search");
	buttonSearchDll.setOnClick(buttonSearchDll_onClick);
	buttonSearchDll.setParam(this);
	buttonLoadDll.setText("Load");
	buttonLoadDll.setOnClick(buttonLoadDll_onClick);
	buttonLoadDll.setParam(this);

	labelLoadProcess.setText("Load Process");
	buttonSearchProcess.setText("Search");
	buttonSearchProcess.setOnClick(buttonSearchProcess_onClick);
	buttonSearchProcess.setParam(this);
	buttonLoadProcess.setText("Load");
	buttonLoadProcess.setOnClick(buttonLoadProcess_onClick);
	buttonLoadProcess.setParam(this);

	layoutFilter.setAlignment(Layout::center_align);
	layoutFilter.append(labelFilter);
	layoutFilter.append(editFilter);

	layoutLoadDll.setAlignment(Layout::center_align);
	layoutLoadDll.append(labelLoadDll);
	layoutLoadDll.append(editDllPath);
	layoutLoadDll.append(buttonSearchDll);
	layoutLoadDll.append(buttonLoadDll);

	layoutLoadProcess.setAlignment(Layout::center_align);
	layoutLoadProcess.append(labelLoadProcess);
	layoutLoadProcess.append(editProcessPath);
	layoutLoadProcess.append(buttonSearchProcess);
	layoutLoadProcess.append(buttonLoadProcess);

	layout.append(layoutFilter);
	layout.append(grid); 
	layout.append(layoutLoadDll);
	layout.append(layoutLoadProcess);
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

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainWindow mainWindow;
	mainWindow.setSize(Size(800, 600));
	mainWindow.setVisible(true);

	while (uilib::UI::UIProcess()) {

	}

	return 0;
}
