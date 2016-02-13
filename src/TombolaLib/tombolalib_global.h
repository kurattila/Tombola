#ifndef TOMBOLALIB_GLOBAL_H
#define TOMBOLALIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOMBOLALIB_LIBRARY)
#  define TOMBOLALIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOMBOLALIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOMBOLALIB_GLOBAL_H