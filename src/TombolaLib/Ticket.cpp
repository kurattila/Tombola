
#include "Ticket.h"


Ticket::Ticket(int ticketNumber, const TicketsBlock& ownerBlock)
    : m_TicketNumber(ticketNumber)
    , m_OwnerBlock(ownerBlock)
{
}

int Ticket::TicketNumber() const
{
    return m_TicketNumber;
}

const QString &Ticket::BlockName() const
{
    return m_OwnerBlock.Name;
}

int Ticket::ColorsSetIndex() const
{
    return m_OwnerBlock.ColorsSetIndex;
}
