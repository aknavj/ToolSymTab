#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include <time.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <assert.h>

/*
 */
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>

#include <QWidget>
#include <QDockWidget>
#include <QLabel>
#include <QToolBar>
#include <QTreeView>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

#define USE_ASSERT
#define DONT_USE_EXCEPTION

/*
 */
#ifdef __GNUC__	
	#define LIKELY(EXP)		__builtin_expect((EXP),1)
	#define UNLIKELY(EXP)	__builtin_expect((EXP),0)
#else
	#define LIKELY(EXP)		(EXP)
	#define UNLIKELY(EXP)	(EXP)
#endif

 /*
  */
#ifdef USE_ASSERT
	#include "Log.h"
	#ifdef assert
		#undef assert
	#endif
	#ifdef NDEBUG
		#define assert(EXP)	(static_cast<void>(0))
	#else
		#if _LINUX
			#define assert(EXP) { if(UNLIKELY(EXP)) { } else { Log::fatal("%s:%d: %s: Assertion: '%s'\n",__FILE__,__LINE__,__ASSERT_FUNCTION,#EXP); } }
		#elif defined(_MACOS) || defined(_IOS)
			#define assert(EXP) { if(UNLIKELY(EXP)) { } else { Log::fatal("%s:%d: %s: Assertion: '%s'\n",__FILE__,__LINE__,__PRETTY_FUNCTION__,#EXP); } }
		#else
			#define assert(EXP) { if(UNLIKELY(EXP)) { } else { Log::fatal("%s:%d: Assertion: '%s'\n",__FILE__,__LINE__,#EXP); } }
		#endif
	#endif
#endif

  /*
   */
#ifdef DONT_USE_EXCEPTION
	#include "Log.h"
	#define NUM_EXCEPTIONS	32
	extern volatile int exception_depth;
	extern jmp_buf exception_buf[NUM_EXCEPTIONS];
	extern const char* exception_data[NUM_EXCEPTIONS];
	
	#define TRY { exception_depth++; assert(exception_depth < NUM_EXCEPTIONS && "try(): stack overflow"); if(setjmp(exception_buf[exception_depth]) == 0) { {
	#define CATCH(EXP) } exception_depth--; } else { EXP = exception_data[exception_depth--]; assert(exception_depth >= 1 && "catch(): stack underflow"); {
	#define THROW(EXP) { if(exception_depth <= 1) Log::fatal("%s:%d: throw(): unhandled exception: %s",__FILE__,__LINE__,EXP); exception_data[exception_depth] = EXP; longjmp(exception_buf[exception_depth],1); }
	#define TRY_END } } }
#else
	#define TRY try
	#define THROW(EXP) throw(EXP)
	#define CATCH(EXP) catch(EXP)
	#define TRY_END
#endif

#endif /* __BASE_H__ */
