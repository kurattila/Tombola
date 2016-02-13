#ifndef TICKET_H
#define TICKET_H

#include <QString>

#include "tombolalib_global.h"

#include "TicketsBlock.h"

class TOMBOLALIBSHARED_EXPORT Ticket
{
    int m_TicketNumber = 0;
    const TicketsBlock& m_OwnerBlock;

public:
    Ticket(int ticketNumber, const TicketsBlock& ownerBlock);

    int TicketNumber() const;
    const QString& BlockName() const;
    int ColorsSetIndex() const;
};

#endif // TICKET_H
