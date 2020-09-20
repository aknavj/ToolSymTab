#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <QString>

/*
 */
class Console {

		Console();
		~Console();

	public:

		static void format(int type, QString fmt);
		static void message(QString str);
		static void warning(QString str);
		static void error(QString str);
		static void fatal(QString str);
		static void ok(QString str);

	private:
		friend class WidgetConsole;
};

#endif /* */