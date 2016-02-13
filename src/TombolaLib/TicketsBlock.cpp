#include "TicketsBlock.h"

bool TicketsBlock::isValidTicketId(int ticketId) const
{
    return ticketId >= 1 && ticketId <= m_TicketsInBlock;
}

TicketsBlock::TicketsBlock(int ticketsPerBlock)
    : m_TicketsInBlock(ticketsPerBlock)
{
}

const std::set<int>& TicketsBlock::GetSoldTicketIds() const
{
    return m_SoldTickets;
}

void TicketsBlock::ToggleSoldTicket(int ticketId)
{
    if (!isValidTicketId(ticketId))
        return;

    if (!IsTicketSold(ticketId))
        m_SoldTickets.insert(ticketId);
    else
        m_SoldTickets.erase(ticketId);
}

void TicketsBlock::SetTicketSold(int ticketId, bool isSold)
{
    if (!isValidTicketId(ticketId))
        return;

    if (isSold)
        m_SoldTickets.insert(ticketId);
    else
        m_SoldTickets.erase(ticketId);
}

bool TicketsBlock::IsTicketSold(int ticketId) const
{
    return m_SoldTickets.find(ticketId) != m_SoldTickets.end();
}

void TicketsBlock::BeginRangeOperation(int startingTicketId)
{
    m_RangeOperationBeginTicket = NotValidTicketId; // stop RangeOperationInProgress() from messing things up
    if (!isValidTicketId(startingTicketId))
        return;

    m_SoldStateToApplyForRangeOperationVictims = !IsTicketSold(startingTicketId);

    m_RangeOperationVictimsOriginalState.clear();
    m_RangeOperationVictimTickets.clear();
    m_RangeOperationBeginTicket = startingTicketId;
    m_RangeOperationVictimTickets.insert(startingTicketId);
}

void TicketsBlock::RangeOperationInProgress(int ticketId)
{
    if (!isValidTicketId(ticketId) || m_RangeOperationBeginTicket == NotValidTicketId)
        return;

    int rangeLowerEnd = m_RangeOperationBeginTicket;
    int rangeUpperEnd = ticketId;
    if (rangeLowerEnd > rangeUpperEnd)
        std::swap(rangeLowerEnd, rangeUpperEnd);

    // undo changes made by mistake (when user unintentionally touched some tickets during dragging the mouse)
    std::set<int> reversedTickets;
    std::copy_if(  m_RangeOperationVictimTickets.begin()
                 , m_RangeOperationVictimTickets.end()
                 , std::inserter(reversedTickets, reversedTickets.begin())
                 , [rangeLowerEnd, rangeUpperEnd](const int victimId)
    {
        bool outOfRange = victimId < rangeLowerEnd || victimId > rangeUpperEnd;
        return outOfRange;
    });
    for (int oneReversedTicket: reversedTickets)
    {
        SetTicketSold(oneReversedTicket, m_RangeOperationVictimsOriginalState[oneReversedTicket]);
        m_RangeOperationVictimTickets.erase(oneReversedTicket);
        m_RangeOperationVictimsOriginalState.erase(oneReversedTicket);
    }

    // change "sold" state of every ticket between beginning and end
    // (populate every ticket number in between, even if they were never explicitly touched during user's dragging)
    int ticketsToTouchBegin = m_RangeOperationBeginTicket;
    if (!m_RangeOperationVictimTickets.empty())
        ticketsToTouchBegin = *std::max_element(m_RangeOperationVictimTickets.begin(), m_RangeOperationVictimTickets.end());
    int ticketsToTouchEnd = ticketId;
    if (ticketsToTouchEnd < ticketsToTouchBegin)
        std::swap(ticketsToTouchBegin, ticketsToTouchEnd);
    for (int toTouch = ticketsToTouchBegin; toTouch <= ticketsToTouchEnd; ++toTouch)
    {
        m_RangeOperationVictimTickets.insert(toTouch);
        if (m_RangeOperationVictimsOriginalState.find(toTouch) == m_RangeOperationVictimsOriginalState.end())
            m_RangeOperationVictimsOriginalState[toTouch] = IsTicketSold(toTouch); // save the original value, not the value which might have been changed already
        SetTicketSold(toTouch, m_SoldStateToApplyForRangeOperationVictims);
    }
}

void TicketsBlock::EndRangeOperation(int /*endTicketId*/)
{

}
