#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include "tombolalib_global.h"
#include <QObject>

#include <fstream>
#include <sstream>

#ifdef _WIN32
    #include <Windows.h>
    #include <Dbghelp.h>
#endif

class TOMBOLALIBSHARED_EXPORT CrashHandler : public QObject
{
    Q_OBJECT

#ifdef _WIN32
    static void make_minidump(EXCEPTION_POINTERS* e);
    static LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS * exceptionInfo);
#endif

public:
    explicit CrashHandler(QObject *parent = 0);

    Q_INVOKABLE void InvokeSimulatedCrash();
};

#endif // CRASHHANDLER_H
