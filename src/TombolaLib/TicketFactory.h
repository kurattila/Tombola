#ifndef TICKETFACTORY_H
#define TICKETFACTORY_H

#include "tombolalib_global.h"

#include <list>
#include <memory>

class Ticket;
class TicketsBlock;

class TOMBOLALIBSHARED_EXPORT TicketFactory
{
public:
    virtual std::list< std::shared_ptr<Ticket> > CreateTicketsFromBlock(const TicketsBlock& block) const;
};

#endif // TICKETFACTORY_H
