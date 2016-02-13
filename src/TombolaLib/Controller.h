#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tombolalib_global.h"

#include "TicketsSerializer.h"

#include <QObject>
#include <memory>

class TombolaDocument;

class TOMBOLALIBSHARED_EXPORT Controller : public QObject
{
    Q_OBJECT

    static QString XmlFullPath;

    TombolaDocument& m_TombolaDocument;
    TicketsSerializer m_Serializer;

    void checkNewVersion();

public:
    Controller(TombolaDocument& document, QObject* parent = nullptr);

    void OnAppStartup();
    void OnAppExit();
};

#endif // CONTROLLER_H
