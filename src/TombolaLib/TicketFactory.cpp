#include "TicketFactory.h"
#include "TicketsBlock.h"
#include "Ticket.h"
#include <memory>

std::list<std::shared_ptr<Ticket> > TicketFactory::CreateTicketsFromBlock(const TicketsBlock& block) const
{
    std::list<std::shared_ptr<Ticket> > tickets;
    for (int ticketId: block.GetSoldTicketIds())
    {
        auto newTicket = std::make_shared<Ticket>(ticketId, block);
        tickets.push_back(newTicket);
    }
    return tickets;
}
