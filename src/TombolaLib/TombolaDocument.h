#ifndef TOMBOLADOCUMENT_H
#define TOMBOLADOCUMENT_H

#include "tombolalib_global.h"

#include "TicketsBlocksSet.h"
#include "IMemento.h"
#include <memory>
#include <QObject>

class TOMBOLALIBSHARED_EXPORT TombolaDocument : public QObject
                                              , public IMementoOriginator
{
    Q_OBJECT

    static const int DefaultPrizesCount = 100;
public:
    TombolaDocument(QObject* parent = nullptr);
    virtual ~TombolaDocument();

    int PrizesCount = DefaultPrizesCount;
    std::shared_ptr<TicketsBlocksSet> AllTicketsBlocksSet;

    // IMementoOriginator interface
    virtual IMemento* SaveToMemento() override;
    virtual void RestoreFromMemento(const IMemento* memento, void* context = nullptr) override;

signals:
    void ticketDrawExecutorSaveToMemento(IMemento*& pMemento);
    void ticketDrawExecutorRestoreFromMemento(const IMemento* memento);
};

class TombolaMemento : public IMemento
{
    int PrizesCount = 0;
    std::shared_ptr<TicketsBlocksSet> AllTicketsBlocksSet;
    std::unique_ptr<IMemento> TicketDrawExecMemento;

public:
    TombolaMemento();
    virtual ~TombolaMemento();

    // IMemento interface
    virtual void Read(QXmlStreamReader& xmlReader);
    virtual void Write(QXmlStreamWriter& xmlWriter);

    friend class TombolaDocument;
};

#endif // TOMBOLADOCUMENT_H
