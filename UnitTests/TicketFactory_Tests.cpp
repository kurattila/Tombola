#include "AutoTest.h"
#include "tombolalib_global.h"

#include <memory>
#include "TicketsBlock.h"
#include "TicketFactory.h"
#include "Ticket.h"

class TicketFactory_Test : public QObject
{
    Q_OBJECT
public:
    TicketFactory_Test()
    {
    }

private slots:
    void CreateTicketsFromBlock_ReturnsEmptyList_WhenBlockHasNoSoldTickets();
    void CreateTicketsFromBlock_ReturnsListOf3Tickets_WhenBlockHas3SoldTickets();
    void CreateTicketsFromBlock_SetsTicketIdOfEachTicket_Always();
    void CreateTicketsFromBlock_SetsBlockNameOfEachTicket_Always();
    void CreateTicketsFromBlock_SetsColorsSetIndexOfEachTicket_Always();
};



void TicketFactory_Test::CreateTicketsFromBlock_ReturnsEmptyList_WhenBlockHasNoSoldTickets()
{
    TicketFactory ticketFactory;
    TicketsBlock block(50);

    auto tickets = ticketFactory.CreateTicketsFromBlock(block);

    QCOMPARE(tickets.size(), 0UL);
}

void TicketFactory_Test::CreateTicketsFromBlock_ReturnsListOf3Tickets_WhenBlockHas3SoldTickets()
{
    TicketFactory ticketFactory;
    TicketsBlock block(50);
    block.ToggleSoldTicket(5);
    block.ToggleSoldTicket(20);
    block.ToggleSoldTicket(3);

    auto tickets = ticketFactory.CreateTicketsFromBlock(block);

    QCOMPARE(tickets.size(), 3UL);
}

void TicketFactory_Test::CreateTicketsFromBlock_SetsTicketIdOfEachTicket_Always()
{
    TicketFactory ticketFactory;
    TicketsBlock block(50);
    block.ToggleSoldTicket(5);
    block.ToggleSoldTicket(20);
    block.ToggleSoldTicket(3);

    auto tickets = ticketFactory.CreateTicketsFromBlock(block);

    auto ticket3 = *std::next(tickets.begin(), 0);
    auto ticket5 = *std::next(tickets.begin(), 1);
    auto ticket20 = *std::next(tickets.begin(), 2);
    QCOMPARE(ticket3->TicketNumber(), 3);
    QCOMPARE(ticket5->TicketNumber(), 5);
    QCOMPARE(ticket20->TicketNumber(), 20);
}

void TicketFactory_Test::CreateTicketsFromBlock_SetsBlockNameOfEachTicket_Always()
{
    TicketFactory ticketFactory;
    TicketsBlock block1(50);
    TicketsBlock block2(50);
    block1.Name = "one";
    block2.Name = "two";
    block1.ToggleSoldTicket(1);
    block2.ToggleSoldTicket(2);

    auto ticketsOfBlockOne = ticketFactory.CreateTicketsFromBlock(block1);
    auto ticketsOfBlockTwo = ticketFactory.CreateTicketsFromBlock(block2);

    auto ticket1 = *std::next(ticketsOfBlockOne.begin(), 0);
    auto ticket2 = *std::next(ticketsOfBlockTwo.begin(), 0);
    QCOMPARE(ticket1->BlockName(), QString("one"));
    QCOMPARE(ticket2->BlockName(), QString("two"));
}

void TicketFactory_Test::CreateTicketsFromBlock_SetsColorsSetIndexOfEachTicket_Always()
{
    TicketFactory ticketFactory;
    TicketsBlock block(50);
    block.ToggleSoldTicket(5);
    block.ColorsSetIndex = 20;

    auto tickets = ticketFactory.CreateTicketsFromBlock(block);

    auto ticket = *tickets.begin();
    QCOMPARE(ticket->ColorsSetIndex(), 20);
}

#include "TicketFactory_Tests.moc"

DECLARE_TEST(TicketFactory_Test)
