#ifndef TOMBOLADOCUMENT_H
#define TOMBOLADOCUMENT_H

#include "tombolalib_global.h"

#include "TicketsBlocksSet.h"
#include "IMemento.h"
#include <memory>

class TOMBOLALIBSHARED_EXPORT TombolaDocument : public IMementoOriginator
{
    static const int DefaultPrizesCount = 100;
public:
    TombolaDocument();
    virtual ~TombolaDocument();

    int PrizesCount = DefaultPrizesCount;
    std::shared_ptr<TicketsBlocksSet> AllTicketsBlocksSet;

    virtual IMemento* SaveToMemento();
    virtual void RestoreFromMemento(const IMemento* memento);
};

class TombolaMemento : public IMemento
{
    int PrizesCount = 0;
    std::shared_ptr<TicketsBlocksSet> AllTicketsBlocksSet;

public:
    TombolaMemento();
    virtual ~TombolaMemento();
    virtual void Read(QXmlStreamReader& xmlReader);
    virtual void Write(QXmlStreamWriter& xmlWriter);

    friend class TombolaDocument;
};

#endif // TOMBOLADOCUMENT_H
