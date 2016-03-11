#include "InGameTicketsRepository.h"

InGameTicketsRepository::InGameTicketsRepository()
{

}

void InGameTicketsRepository::Init(const std::list<std::shared_ptr<Ticket> > &untouchedTickets)
{
    m_UntouchedTickets = untouchedTickets;
    m_WinningTickets.clear();
}

void InGameTicketsRepository::OnTicketDrawn(std::shared_ptr<Ticket> winningTicket)
{
    if (m_UntouchedTickets.empty())
        throw std::invalid_argument("Tickets list must not be empty");

    m_UntouchedTickets.remove(winningTicket);
    m_WinningTickets.push_front(winningTicket);
}

const std::list<std::shared_ptr<Ticket> > &InGameTicketsRepository::GetTicketsStillInGame() const
{
    return m_UntouchedTickets;
}

const std::list<std::shared_ptr<Ticket> > &InGameTicketsRepository::GetWinningTicketsHistory() const
{
    return m_WinningTickets;
}
