#include "widgets/WidgetConsole.h"
#include "Console.h"
#include "utils/Log.h"

/*
 */
Console::Console() {

}

Console::~Console() {

}

 /*
  */
void Console::format(int type, QString str) {
	WidgetConsole* wc = WidgetConsole::get();
	if (wc != NULL) wc->addLine(QString("Warning: ") + str, type);
	Log::warning(str.toStdString().c_str(), type);
}

/*
 */
void Console::message(QString str) {
	WidgetConsole* wc = WidgetConsole::get();
	if (wc != NULL) wc->addLine(str);
	Log::message(str.toStdString().c_str());
}

/*
 */
void Console::warning(QString str) {
	WidgetConsole* wc = WidgetConsole::get();
	if (wc != NULL) wc->addLine(QString("Warning: ") + str, Log::WARNING);
	Log::warning(str.toStdString().c_str());
}

/*
 */
void Console::error(QString str) {
	WidgetConsole* wc = WidgetConsole::get();
	if (wc != NULL) wc->addLine(QString("Error: ") + str, Log::ERROR);
	Log::error(str.toStdString().c_str());
}

/*
 */
void Console::fatal(QString str) {
	WidgetConsole* wc = WidgetConsole::get();
	if (wc != NULL) wc->addLine(QString("Fatal: ") + str, Log::FATAL);
	Log::fatal(str.toStdString().c_str());
}

/*
 */
void Console::ok(QString str) {
	WidgetConsole* wc = WidgetConsole::get();
	if (wc != NULL) wc->addLine(QString("Ok: ") + str, Log::OK);
	Log::ok(str.toStdString().c_str());
}
