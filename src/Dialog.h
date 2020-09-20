#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMainWindow>

#include "utils/Base.h"

#include "widgets/WidgetDock.h"
#include "DLLParser.h"

/*
 */
class Dialog : public QMainWindow {

	public:
		 Dialog(QWidget* parent = 0);
		~Dialog();

		static Dialog* get();

	private:

		// add objects
		void addMenus();
		void addToolBars();
		void addActions();
		void addDialogs();
		void addWidgets();
		void addDockWidgets(QMainWindow* parent);

		// remove objects
		void removeMenus();
		void removeToolBars();
		void removeActions();
		void removeDialogs();
		void removeWidgets();
		void removeDockWidgets(QMainWindow* parent);

	private:

		// widget definitions
		WidgetDock< DLLParser>* m_dll_widget_a;
		WidgetDock< DLLParser>* m_dll_widget_b;

		int timer_update;			// timer on update editor event
};

/*
 */
extern struct DialogData* designer; // own instance pointer

#endif /* __DIALOG_H__ */
