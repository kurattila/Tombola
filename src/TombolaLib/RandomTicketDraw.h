#ifndef RANDOMTICKETDRAW_H
#define RANDOMTICKETDRAW_H

#include "tombolalib_global.h"
#include <memory>
#include <list>
#include <vector>

class Ticket;

class TOMBOLALIBSHARED_EXPORT RandomTicketDraw
{
    mutable int m_RemainingSteps = 0;
public:
    RandomTicketDraw();

    void Init(int stepsCountOfDraw, const std::list<std::shared_ptr<Ticket>>& untouchedTickets);
    const std::shared_ptr<Ticket>& GetWinningTicket() const;
    const std::shared_ptr<Ticket>& GetNextPresentableTicket() const;
    bool IsDone() const;

//    const std::shared_ptr<Ticket>& InitToGetWinningTicket(const std::list<std::shared_ptr<Ticket>>& untouchedTickets) const;

protected:
    virtual void shuffleTickets(std::vector<std::shared_ptr<Ticket>>& tickets);

    std::vector<std::shared_ptr<Ticket>> m_Tickets;
};

#endif // RANDOMTICKETDRAW_H
