#ifndef LOGGER_H
#define LOGGER_H

#include "tombolalib_global.h"
#include <QtGlobal>

class TOMBOLALIBSHARED_EXPORT Logger
{
    static QtMessageHandler DefaultMsgHandler;
    static void loggingMsgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    static QString LogFileName;

public:
    static void Init(const QString& filename);
};

#endif // LOGGER_H
