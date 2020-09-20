#include "../utils/Base.h"
#include "../utils/Log.h"
#include "../Dialog.h"
#include "../Console.h"
#include "WidgetConsole.h"

/*
 */
struct WidgetConsoleData {
	WidgetConsole* m_console; // own pointer
};
static WidgetConsoleData* m_console = nullptr; 

/*
 */
WidgetConsole::WidgetConsole(QWidget* parent) : QWidget(parent) {

	assert(m_console == NULL && "WidgetConsole::WidgetConsole(): is already initialized");

	// struct stuff
	m_console = new WidgetConsoleData;
	memset(m_console, NULL, sizeof(WidgetConsoleData));
	m_console->m_console = this;

	// qt class stuff
	setMinimumSize(640, 150);
	setMaximumSize(1000000, 1000000);

	// FIXME: ugh ugly "hack"..
	m_toolbar = new WidgetConsoleToolbar(this);

	// qt child object stuff
	m_listview = new QListWidget(parent);

	QMainWindow* inner_widget = new QMainWindow(0);
	inner_widget->addToolBar(Qt::TopToolBarArea, m_toolbar);
	
	inner_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	inner_widget->setCentralWidget(m_listview);
	inner_widget->setParent(this);

	QVBoxLayout* m_mainlayout = new QVBoxLayout;
	m_mainlayout->setSpacing(0);
	m_mainlayout->setContentsMargins(0, 0, 0, 0);
	m_mainlayout->addWidget(inner_widget);
	this->setLayout(m_mainlayout);
}

/******************************************************************************\
*
* Events
*
\******************************************************************************/

/*
 */
void WidgetConsole::timerEvent(QTimerEvent* event) {
	Q_UNUSED(event);
	QWidget::update();
	QWidget::timerEvent(event);
}

/*
 */
void WidgetConsole::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
}

/*
 */
void WidgetConsole::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);
}

/******************************************************************************\
*
* Private stuff
*
\******************************************************************************/

/*
 */
WidgetConsole::~WidgetConsole() {
	delete m_console;
	m_console = nullptr;
}

/*
 */
void WidgetConsole::addLine(QString str, int type) {

	if (m_listview != NULL) {

		QListWidgetItem* item = new QListWidgetItem(str, m_listview);

		switch (type) {
			case Log::MESSAGE: break;
			case Log::WARNING: 
			{
				item->setBackgroundColor(QColor(255, 255, 0, 255));
				item->setTextColor(QColor(0, 0, 0, 255));
				break;
			}
			case Log::ERROR: item->setBackgroundColor(QColor(255, 0, 0, 255)); break;
			case Log::FATAL: item->setBackgroundColor(QColor(0, 0, 0, 0)); break;
			case Log::OK:
			{
				item->setBackground(QColor(179, 221, 158, 255));
				item->setTextColor(QColor(0, 0, 0, 255));
				break;
			}
			default:
			{
				if (type >= Log::NUM_TYPES) {

				}
				else {

				}
				break;
			}
		}

		m_listview->addItem(item);
		m_listview->scrollToBottom(); // always scroll to bottom of the list widget
	}
}

/*
 */
void WidgetConsole::clearLines() {
	if (m_listview != NULL) m_listview->clear(); 
}

void WidgetConsole::copyText() {
	Console::message("Test");
}

/******************************************************************************\
*
* Static stuff
*
\******************************************************************************/

/*
 */
WidgetConsole* WidgetConsole::get() {
	if (m_console == NULL) return NULL;
	return m_console->m_console;
}

/******************************************************************************\
*
* WidgetConsoleToolbar
*
\******************************************************************************/

/*
 */
WidgetConsole::WidgetConsoleToolbar::WidgetConsoleToolbar(QWidget* parent) : QToolBar(parent) {

	// add actions
	addActions();
}

/*
 */
WidgetConsole::WidgetConsoleToolbar::~WidgetConsoleToolbar() {

	// remove actions
	removeActions();
}

/*
 */
void WidgetConsole::WidgetConsoleToolbar::addActions() {
	//this->addAction(m_copyTextAction);
}

/*
 */
void WidgetConsole::WidgetConsoleToolbar::removeActions() {
	//this->removeAction(m_copyTextAction);
	//delete m_copyTextAction;
}
