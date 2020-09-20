#include "Dialog.h"
#include "utils/Utils.h"
#include "utils/Log.h"

#include "widgets/WidgetDock.h"

#ifdef _DEBUG
	#define DEFAULT_APP_NAME QString("DLL Tool Symbolic Table Viewer (Debug) build")
#else
	#define DEFAULT_APP_NAME QString("DLL Tool Symbolic Table Viewer")
#endif

struct DialogData {
    Dialog* dialog;
};

/*
 */
struct DialogData* dialog = nullptr;

/*
 */
Dialog::Dialog(QWidget* parent) : QMainWindow(parent) {
    assert(dialog == NULL && "Dialog::Dialog(): instance is already initialized!\n");
	
    dialog = new DialogData;
    memset(dialog, 0, sizeof(DialogData));
    dialog->dialog = this;

	this->setWindowTitle(DEFAULT_APP_NAME);

	// note: Status Bar is disabled in *.ui file!
	this->setAnimated(true);
	this->setAcceptDrops(true);
	this->setDockNestingEnabled(true);
	this->setDockOptions(QMainWindow::AllowTabbedDocks);

	// construct editor gui
	addActions();
	addMenus();
	addToolBars();
	addDialogs();
	addWidgets();
	addDockWidgets(this);

//	setCentralWidget(ed);

	this->show();

	// initialize interpreterers
	//PyInterpreter::init();
}

/*
 */
Dialog::~Dialog() {

	// shutdown interpreters
	//Interpreter::shutdown();

	// construct editor gui
	removeDockWidgets(this);
	removeWidgets();
	removeDialogs();
	removeToolBars();
	removeActions();
	removeMenus();

	//PyInterpreter::shutdown();

	Log::shutdown();
}

/*
 */
void Dialog::addMenus() {

}

/*
 */
void Dialog::addToolBars() {
}

/*
 */
void Dialog::addDialogs() {

}

/*
 */
void Dialog::addWidgets() {

}

/*
 */
void Dialog::addDockWidgets(QMainWindow* parent) {
    m_dll_widget_a = new WidgetDock<DLLParser>(this, "DLL Symbol Table Viewer", Qt::DockWidgetArea::RightDockWidgetArea, 0);
}

/*
 */
void Dialog::removeMenus() {

}

/*
 */
void Dialog::removeToolBars() {
}

/*
 */
void Dialog::removeDialogs() {
}

/*
 */
void Dialog::removeWidgets() {
}

/*
 */
void Dialog::removeDockWidgets(QMainWindow* parent) {

	parent->removeDockWidget(m_dll_widget_a->dock());
	delete m_dll_widget_a;
}

/******************************************************************************\
*
* Actions
*
\******************************************************************************/

/*
 */
void Dialog::addActions() {

}

/*
 */
void Dialog::removeActions() {

}

/******************************************************************************\
*
* Static members
*
\******************************************************************************/

/*
 */
Dialog* Dialog::get() {
    if (dialog->dialog == NULL) return NULL;
    return dialog->dialog;
}
