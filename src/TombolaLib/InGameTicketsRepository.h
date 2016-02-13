#ifndef INGAMETICKETSREPOSITORY_H
#define INGAMETICKETSREPOSITORY_H

#include "tombolalib_global.h"
#include "Ticket.h"
#include <list>
#include <memory>

class TOMBOLALIBSHARED_EXPORT InGameTicketsRepository
{
    std::list<std::shared_ptr<Ticket>> m_UntouchedTickets;
    std::list<std::shared_ptr<Ticket>> m_WinningTickets;

public:
    InGameTicketsRepository();

    void Init(const std::list<std::shared_ptr<Ticket>>& untouchedTickets);
    void OnTicketDrawn(std::shared_ptr<Ticket> winningTicket);
    const std::list<std::shared_ptr<Ticket>>& GetTicketsStillInGame() const;
    const std::list<std::shared_ptr<Ticket>>& GetWinningTicketsHistory() const;
};

#endif // INGAMETICKETSREPOSITORY_H
