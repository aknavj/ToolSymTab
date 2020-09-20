#ifndef __LOG_H__
#define __LOG_H__

/*
 */
class Log {

		Log();
		~Log();

		

	public:

		enum {
			MESSAGE = 0,
			WARNING,
			FATAL,
			ERROR,
			OK,
			NUM_TYPES,
		};

		static int init(const char* name);
		static void shutdown();

		static void format(int type, const char* fmt, ...);

		static void message(const char* format, ...);
		static void warning(const char* format, ...);
		static void fatal(const char* format, ...);
		static void error(const char* format, ...);
		static void ok(const char* format, ...);
};

#endif /* __LOG_H__ */