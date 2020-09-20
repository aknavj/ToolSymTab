#ifndef __UTILS_H__
#define __UTILS_H__

#include <QString>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessagebox>

/*
 */
namespace QtUtils {

	/*
	 */
	class QMessageBoxDump : public QWidget {

		private:
			QLineEdit m_txtDump;
			QPushButton m_sendreport;
			QPushButton m_cancel;

		public:

			QMessageBoxDump(QString title, QString message, QWidget* parent = nullptr) : QWidget(parent) {

			}

			virtual ~QMessageBoxDump() {

			}
	};

	enum {
		FILE_NEW = 0,
		FILE_OPEN,
		FILE_SAVE,
		NUM_FILES,
	};

	// dialog message simple
	int dialogMessageYesNo(QString title, QString message) {

		QMessageBox msgBox;

		msgBox.setWindowTitle(title);
		msgBox.setText(message);
		msgBox.setStandardButtons(QMessageBox::Yes);
		msgBox.addButton(QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);

		if (msgBox.exec() == QMessageBox::Yes) return 1;
		else return 0;
	}

	// get window widget
	static QWidget* getWidget(const char* classname) {
		QWidget* m = NULL;
		foreach(QWidget * widget, qApp->topLevelWidgets()) {
			if (widget->inherits(classname)) {
				m = widget; break;
			}
		}
		return m;
	}

	int fileDialog(int type, /*QFile fp,*/ QString description, QString name, QString short_ext, QString extension, QString path) {
		QString tmp_name;
		
		QFileDialog dialog;
		dialog.setFileMode(QFileDialog::AnyFile);

		QFile fp(QString(name + short_ext));
		switch (type) {
		case FILE_OPEN: 
			tmp_name = dialog.getOpenFileName(NULL, description, path, extension, &extension);
			tmp_name.replace(short_ext, QString(""));

			
			if (fp.open(QIODevice::ReadOnly)) {
				if (dialogMessageYesNo(QString("Confirm"), QString("Override " + name + "file?")) == 0) {
					fp.close();
					return 0;
				}
				fp.close();
			}


			break;
		case FILE_SAVE: break;
		default: break;
		}
	}

	// get main window
	static QMainWindow* getMainWindow(const char* classname) {
		QWidgetList widgets = qApp->topLevelWidgets();
		for (QWidgetList::iterator i = widgets.begin(); i != widgets.end(); ++i)
			if ((*i)->objectName() == classname)
				return (QMainWindow*)(*i);
		return NULL;
	}
}

#endif /* __UTILS_H__ */