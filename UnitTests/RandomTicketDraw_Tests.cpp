#include "AutoTest.h"
#include "tombolalib_global.h"

#include <memory>
#include <list>
#include "RandomTicketDraw.h"
#include "Ticket.h"
#include "TicketsBlock.h"

class RandomTicketDraw_Test : public QObject
{
    Q_OBJECT
public:
    RandomTicketDraw_Test()
    {
    }

private slots:
//    void InitToGetWinningTicket_WillThrow_WhenTicketsListEmpty();
//    void InitToGetWinningTicket_ReturnsFirstTicket_WhenOnlyOneTicketAvailable();
    void Init_WillShuffleTickets_Always();

    void GetNextPresentableTicket_WillThrow_WhenTicketsListEmpty();
    void GetNextPresentableTicket_ReturnsLast3Randomized_When3StepsOf5TicketsRequired();
    void GetNextPresentableTicket_CyclesThroughAll3ExistingTickets_When5StepsRequiredButOnly3TicketsExist();

    void GetWinningTicket_WillThrow_WhenTicketsListEmpty();
    void GetWinningTicket_ReturnsLastTicketOfRandomizedVector_WhenTicketsListNotEmpty();

    void IsDone_ReturnsFalse_WhenSomeStepsStillRemain();
    void IsDone_ReturnsTrue_WhenNoRemainingSteps();
};

class Fake_RandomTicketDraw : public RandomTicketDraw
{
public:
    const std::vector<std::shared_ptr<Ticket>>& RandomizedTickets() const
    {
        return m_Tickets;
    }
protected:
    virtual void shuffleTickets(std::vector<std::shared_ptr<Ticket>>& tickets) override
    {
        std::swap(tickets.front(), tickets.back());
    }
};

//void RandomTicketDraw_Test::InitToGetWinningTicket_WillThrow_WhenTicketsListEmpty()
//{
//    std::list<std::shared_ptr<Ticket>> allTickets;
//    RandomTicketDraw draw;

//    QVERIFY_EXCEPTION_THROWN(draw.InitToGetWinningTicket(allTickets), std::invalid_argument);
//}

//void RandomTicketDraw_Test::InitToGetWinningTicket_ReturnsFirstTicket_WhenOnlyOneTicketAvailable()
//{
//    auto theOnlyTicket = std::make_shared<Ticket>(15);
//    std::list<std::shared_ptr<Ticket>> allTickets = {theOnlyTicket};
//    RandomTicketDraw draw;

//    const std::shared_ptr<Ticket>& winningTicket = draw.InitToGetWinningTicket(allTickets);

//    QCOMPARE(winningTicket.get(), theOnlyTicket.get());
//}

void RandomTicketDraw_Test::Init_WillShuffleTickets_Always()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> ticketsList = { ticket1, ticket2, ticket3 };
    Fake_RandomTicketDraw spyDraw;

    spyDraw.Init(10, ticketsList); // fake randomization will exchange ticket1 with ticket3

    QVERIFY(spyDraw.RandomizedTickets().back() != ticket3);
}

void RandomTicketDraw_Test::GetNextPresentableTicket_WillThrow_WhenTicketsListEmpty()
{
    std::list<std::shared_ptr<Ticket>> ticketsList;
    RandomTicketDraw draw;
    draw.Init(10, ticketsList);

    QVERIFY_EXCEPTION_THROWN(draw.GetNextPresentableTicket(), std::invalid_argument);
}

void RandomTicketDraw_Test::GetNextPresentableTicket_ReturnsLast3Randomized_When3StepsOf5TicketsRequired()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    auto ticket4 = std::make_shared<Ticket>(4, block);
    auto ticket5 = std::make_shared<Ticket>(5, block);
    std::list<std::shared_ptr<Ticket>> ticketsList = { ticket1, ticket2, ticket3, ticket4, ticket5 };
    Fake_RandomTicketDraw spyDraw;
    spyDraw.Init(3, ticketsList); // fake randomization will exchange ticket1 with ticket5

    auto ticketStep1 = spyDraw.GetNextPresentableTicket();
    auto ticketStep2 = spyDraw.GetNextPresentableTicket();
    auto ticketStep3 = spyDraw.GetNextPresentableTicket();

    QVERIFY(spyDraw.IsDone());
    QCOMPARE(ticketStep1, ticket3);
    QCOMPARE(ticketStep2, ticket4);
    QCOMPARE(ticketStep3, ticket1);
}

void RandomTicketDraw_Test::GetNextPresentableTicket_CyclesThroughAll3ExistingTickets_When5StepsRequiredButOnly3TicketsExist()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> ticketsList = { ticket1, ticket2, ticket3 };
    Fake_RandomTicketDraw spyDraw;
    spyDraw.Init(5, ticketsList); // fake randomization will exchange ticket1 with ticket3

    auto ticketStep1 = spyDraw.GetNextPresentableTicket();
    auto ticketStep2 = spyDraw.GetNextPresentableTicket();
    auto ticketStep3 = spyDraw.GetNextPresentableTicket();
    auto ticketStep4 = spyDraw.GetNextPresentableTicket();
    auto ticketStep5 = spyDraw.GetNextPresentableTicket();

    QVERIFY(spyDraw.IsDone());
    QCOMPARE(ticketStep1, ticket2);
    QCOMPARE(ticketStep2, ticket1);
    QCOMPARE(ticketStep3, ticket3);
    QCOMPARE(ticketStep4, ticket2);
    QCOMPARE(ticketStep5, ticket1);
}

void RandomTicketDraw_Test::GetWinningTicket_WillThrow_WhenTicketsListEmpty()
{
    std::list<std::shared_ptr<Ticket>> ticketsList;
    RandomTicketDraw draw;
    draw.Init(10, ticketsList);

    QVERIFY_EXCEPTION_THROWN(draw.GetWinningTicket(), std::invalid_argument);
}

void RandomTicketDraw_Test::GetWinningTicket_ReturnsLastTicketOfRandomizedVector_WhenTicketsListNotEmpty()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> ticketsList = { ticket1, ticket2, ticket3 };
    Fake_RandomTicketDraw spyDraw;
    spyDraw.Init(5, ticketsList); // fake randomization will exchange ticket1 with ticket3,
                                  // so that ticket1 becomes the last, the winning ticket

    auto winningTicket = spyDraw.GetWinningTicket();

    QCOMPARE(winningTicket, ticket1);
}

void RandomTicketDraw_Test::IsDone_ReturnsFalse_WhenSomeStepsStillRemain()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> ticketsList = { ticket1, ticket2, ticket3 };
    RandomTicketDraw draw;
    draw.Init(3, ticketsList);

    // present 2 tickets when prize draw consists of 3 tickets presented
    draw.GetNextPresentableTicket();
    draw.GetNextPresentableTicket();

    QCOMPARE(draw.IsDone(), false);
}

void RandomTicketDraw_Test::IsDone_ReturnsTrue_WhenNoRemainingSteps()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> ticketsList = { ticket1, ticket2, ticket3 };
    RandomTicketDraw draw;
    draw.Init(3, ticketsList);

    // present all 3 tickets when prize draw consists of 3 tickets presented
    draw.GetNextPresentableTicket();
    draw.GetNextPresentableTicket();
    draw.GetNextPresentableTicket();

    QCOMPARE(draw.IsDone(), true);
}

#include "RandomTicketDraw_Tests.moc"

DECLARE_TEST(RandomTicketDraw_Test)
