
#include "Logger.h"

#include <QTime>
#include <QFile>
#include <QTextStream>

QtMessageHandler Logger::DefaultMsgHandler;
QString Logger::LogFileName;

void Logger::loggingMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    DefaultMsgHandler(type, context, msg);

    QString txt;
    switch (type)
    {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        break;
    }

    auto datestamp = QDate::currentDate().toString("yyyy MMM dd");
    auto timestamp = QTime::currentTime().toString("hh:mm:ss");

    QFile outFile(LogFileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&outFile);
    textStream.setCodec("UTF-8");
    textStream << "[" << datestamp << " - " << timestamp << "] " << txt << endl;
}

void Logger::Init(const QString &filename)
{
    LogFileName = filename;
    DefaultMsgHandler = qInstallMessageHandler(loggingMsgHandler);
}
