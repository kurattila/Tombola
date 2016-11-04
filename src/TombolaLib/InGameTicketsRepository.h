#ifndef INGAMETICKETSREPOSITORY_H
#define INGAMETICKETSREPOSITORY_H

#include "tombolalib_global.h"
#include "Ticket.h"
#include "TicketsBlocksSet.h"
#include <list>
#include <map>
#include <set>
#include <memory>
#include "IMemento.h"
#include <QString>

class TOMBOLALIBSHARED_EXPORT InGameTicketsRepositoryMemento : public IMemento
{
    friend class InGameTicketsRepository;

    std::map<TicketsBlocksSet::TicketsBlockIdentification, std::set<int>> m_MapOfStillInGameTickets;

public:
    static QString StartElementXmlName;

    InGameTicketsRepositoryMemento();
    virtual ~InGameTicketsRepositoryMemento();

    // IMemento interface
    virtual void Read(QXmlStreamReader& xmlReader);
    virtual void Write(QXmlStreamWriter& xmlWriter);
};

class TOMBOLALIBSHARED_EXPORT InGameTicketsRepository : public IMementoOriginator
{
    std::list<std::shared_ptr<Ticket>> m_UntouchedTickets;
    std::list<std::shared_ptr<Ticket>> m_TicketsBeingTransformedIntoWinningOnes;
    std::list<std::shared_ptr<Ticket>> m_WinningTickets;

public:
    InGameTicketsRepository();

    void Reset(const std::list<std::shared_ptr<Ticket>>& untouchedTickets);
    void OnTicketDrawnPrepare(const std::shared_ptr<Ticket>& winningTicket);
    void OnTicketDrawnCommit (const std::shared_ptr<Ticket>& winningTicket);
    const std::list<std::shared_ptr<Ticket>>& GetTicketsStillInGame() const;
    const std::list<std::shared_ptr<Ticket>>& GetWinningTicketsHistory() const;
    bool IsValid() const;

    // IMementoOriginator interface
    virtual IMemento* SaveToMemento() override;
    virtual void RestoreFromMemento(const IMemento* memento, void* context = nullptr) override;
};

#endif // INGAMETICKETSREPOSITORY_H
