#ifndef TICKETSSERIALIZER_H
#define TICKETSSERIALIZER_H

#include <memory>
#include <QIODevice>

#include "tombolalib_global.h"

class TombolaDocument;
class TOMBOLALIBSHARED_EXPORT TicketsSerializer
{
    TombolaDocument& m_TombolaDocument;
public:
    TicketsSerializer(TombolaDocument& document);

    void Write(QIODevice* outputDevice);
    void Read(QIODevice* inputDevice);
};

#endif // TICKETSSERIALIZER_H
