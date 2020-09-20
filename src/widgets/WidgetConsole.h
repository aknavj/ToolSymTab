#ifndef __WIDGET_CONSOLE_H__
#define __WIDGET_CONSOLE_H__

#include <QAction>
#include <QWidget>
#include <qtoolbar.h>
#include <QLayout>
#include <QListWidget>

// forward declaration
template<class T> class WidgetDock;
class WidgetConsoleToolbar;

/*
 */
class WidgetConsole : public QWidget {

	Q_OBJECT

public:

	/*
	 */
	class WidgetConsoleToolbar : public QToolBar {

	public:
		WidgetConsoleToolbar(QWidget* parent = nullptr);
		~WidgetConsoleToolbar();

		void addActions();
		void removeActions();

		// private:
		QAction* m_copyTextAction;
	};

	/*
	 */
	WidgetConsole(QWidget* parent = nullptr);
	virtual ~WidgetConsole();

	static WidgetConsole* get();

	void addLine(QString str, int type = 0);

public slots:
	void clearLines();
	void copyText();

protected:
	// Qt Events
	void timerEvent(QTimerEvent* event);
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);

private:
	// friend zone
	template<class Widget> friend class WidgetDock;
	friend class Log;

	WidgetConsoleToolbar* m_toolbar;
	QListWidget* m_listview;
	int timer_update;

	void save_plaintext(QString filename);
	void save_xml(QString filename);
	void save_html(QString filename);
};

#endif /* __WIDGET_CONSOLE_H__ */