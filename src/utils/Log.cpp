#include "Base.h"
#include "Log.h"

FILE* file = NULL;

/*
 */
Log::Log() { 

}

Log::~Log() {

}

/*
 */
int Log::init(const char* name) {
	assert(file == NULL && "Log::init(): is already initialized");

	file = fopen(name, "wb");

	if (file == NULL) return 0;

	// print log header
	fputs("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n", file);
	fputs("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n", file);
	fputs("<html><head>\n", file);
	fputs("<title>Log File</title>\n", file);
	fputs("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n", file);
	fputs("<style type=\"text/css\">\n", file);
	fputs("body { background: #ffffff; color: #a4a4a4; margin-right: 20px; margin-left: 20px; font-size: 14px; font-family: Arial, sans-serif, sans; }\n", file);
	fputs("a { text-decoration: none; }\n", file);
	fputs("a:link { color: #a4a4a4; }\n", file);
	fputs("a:active { color: #ff9900; }\n", file);
	fputs("a:visited { color: #ffffff; }\n", file);
	fputs("a:hover { color: #ff9900; }\n", file);
	fputs("h1 { text-align: center; }\n", file);
	fputs("h2 { color: #a4a4a4; }\n", file);
	fputs(".m, .w, .e, .f { padding: 3px; overflow: auto; }\n", file);
	fputs(".m { background-color: #ffffff; color: #000000; }\n", file);
	fputs(".w { background-color: #ff4b00; color: #000000; }\n", file);
	fputs(".e { background-color: #ff0000; color: #000000; }\n", file);
	fputs(".f { background-color: #ff00ff; color: #000000; }\n", file);
	fputs(".k { background-color: #00ff00; color: #000000; }\n", file);
	fputs("</style></head><body>\n", file);
	fflush(file);

	return 1;
}

void Log::shutdown() {
	// print log footer
	if (file) {
		fputs("<div class=\"m\">", file);
		fputs("</div>\n</body></html>\n", file);
		fflush(file);
		fclose(file);
		file = NULL;
	}
}

/*
 */
void Log::format(int type, const char* fmt, ...) {

	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(stdout, fmt, argptr);
	va_end(argptr);
	
	switch (type) {
		case MESSAGE: message(fmt, argptr); break;
		case WARNING: warning(fmt, argptr); break;
		case ERROR: error(fmt, argptr); break;
		case FATAL: fatal(fmt, argptr); break;
		case OK: ok(fmt, argptr); break;
		default:
			if (type >= Log::NUM_TYPES) {

			} else {

			}
			break;
	}

	fflush(stdout);
}

/*
 */
void Log::message(const char* format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	fflush(stdout);

	if (file) {
		fputs("<div class=\"m\">", file);
		va_start(argptr, format);
		vfprintf(file, format, argptr);
		va_end(argptr);
		fputs("</div>\n", file);
		fflush(file);
	}
}

/*
 */
void Log::warning(const char* format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	fflush(stdout);

	if (file) {
		fputs("<div class=\"w\">", file);
		vfprintf(file, format, argptr);
		fputs("</div>\n", file);
		fflush(file);
	}
}

/*
 */
void Log::fatal(const char* format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
	fflush(stderr);

	if (file) {
		fputs("<div class=\"f\">", file);
		vfprintf(file, format, argptr);
		fputs("</div>\n", file);
		fflush(file);
	}

#ifdef NDEBUG
	exit(EXIT_FAILURE);
#else
	abort();
#endif
}

/*
 */
void Log::error(const char* format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	fflush(stdout);

	if (file) {
		fputs("<div class=\"e\">", file);
		vfprintf(file, format, argptr);
		fputs("</div>\n", file);
		fflush(file);
	}
}

/*
 */
void Log::ok(const char* format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	fflush(stdout);

	if (file) {
		fputs("<div class=\"k\">", file);
		vfprintf(file, format, argptr);
		fputs("</div>\n", file);
		fflush(file);
	}
}