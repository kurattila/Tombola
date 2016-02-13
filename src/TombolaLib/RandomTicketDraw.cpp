
#include <random>
#include <algorithm>

#include <QDebug>

#include "RandomTicketDraw.h"
#include "Ticket.h"
#include "BlockColorsSet.h"

RandomTicketDraw::RandomTicketDraw()
{

}

void RandomTicketDraw::Init(int stepsCountOfDraw, const std::list<std::shared_ptr<Ticket>>& untouchedTickets)
{
    m_RemainingSteps = stepsCountOfDraw;
    m_Tickets = std::vector<std::shared_ptr<Ticket>> { untouchedTickets.begin(), untouchedTickets.end() };
    shuffleTickets(m_Tickets);

    if (!m_Tickets.empty())
    {
        auto winning = m_Tickets.back();
        qInfo() << "Winning Ticket:" << BlockColorsSet::GetColorName(winning->ColorsSetIndex()) << winning->BlockName() << winning->TicketNumber();
    }

    if (stepsCountOfDraw > static_cast<int>(m_Tickets.size()) && !untouchedTickets.empty())
    {
        // Some "padding" might be needed at the beginning if too few tickets available:
        // e.g. transform a vector of ticket IDs of 1,2,3 into 3,1,2,3,1,2,3,1,2,3 when 10 steps required
        std::list<std::shared_ptr<Ticket>> repetitionsInsertedToBeginning;
        auto itTickets = std::prev(m_Tickets.end());
        while(repetitionsInsertedToBeginning.size() + m_Tickets.size() < static_cast<size_t>(stepsCountOfDraw))
        {
            repetitionsInsertedToBeginning.push_front(*itTickets);
            if (itTickets == m_Tickets.begin())
                itTickets = m_Tickets.end();
            --itTickets;
        }

        m_Tickets.insert(m_Tickets.begin(), repetitionsInsertedToBeginning.begin(), repetitionsInsertedToBeginning.end());
    }
}

const std::shared_ptr<Ticket>& RandomTicketDraw::GetWinningTicket() const
{
    if (m_Tickets.empty())
        throw std::invalid_argument("Tickets List must not be empty");

    return m_Tickets.back();
}

const std::shared_ptr<Ticket>& RandomTicketDraw::GetNextPresentableTicket() const
{
    if (m_Tickets.empty())
        throw std::invalid_argument("Tickets List must not be empty");

    auto& ticketStep = *std::prev(m_Tickets.end(), m_RemainingSteps);
    m_RemainingSteps--;

    return ticketStep;
}

bool RandomTicketDraw::IsDone() const
{
    return m_RemainingSteps == 0;
}

void RandomTicketDraw::shuffleTickets(std::vector<std::shared_ptr<Ticket>>& tickets)
{
//    std::random_device rd;
//    std::mt19937 g(rd());

    std::random_shuffle(tickets.begin(), tickets.end());
}

//const std::shared_ptr<Ticket>& RandomTicketDraw::InitToGetWinningTicket(const std::list<std::shared_ptr<Ticket>>& untouchedTickets) const
//{
//    if (untouchedTickets.empty())
//        throw std::invalid_argument("Tickets List must not be empty");

//    return untouchedTickets.front();
//}
