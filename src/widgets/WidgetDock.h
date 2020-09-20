#pragma once

#ifndef __WIDGET_DOCK_H__
#define __WIDGET_DOCK_H__

#include <QDockWidget.h>

/*
 */
template <class Widget> class WidgetDock {

	private:
		QDockWidget* m_pDock;
		Widget* m_pWidget;

	public:
		/*
		 */
		WidgetDock(QMainWindow* main, QString title, Qt::DockWidgetArea area, Qt::WindowFlags flags = 0)
			: m_pDock(new QDockWidget(title, main, flags))
			, m_pWidget(new Widget(m_pDock))
		{

			m_pDock->setWidget(m_pWidget);
			main->addDockWidget(area, m_pDock);
		}

		/*
		 */
		WidgetDock(QMainWindow* main, Widget* pWidget, QString title, Qt::DockWidgetArea area, Qt::WindowFlags flags = 0)
			: m_pDock(new QDockWidget(title, main, flags))
			, m_pWidget(pWidget)
		{
			m_pWidget->setParent(m_pDock);
			m_pDock->setWidget(m_pWidget);
			main->addDockWidget(area, m_pDock);
		}

		Widget* widget() const { return m_pWidget; }
		QDockWidget* dock() const { return m_pDock; }

protected:
	virtual void paintEvent(QEvent* event) {
		m_pWidget->update();
		m_pWidget->resize(m_pDock->size());
	}
};

#endif /* __WIDGET_DOCK_H__ */