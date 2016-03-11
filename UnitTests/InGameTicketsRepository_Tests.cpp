#include "AutoTest.h"
#include "tombolalib_global.h"

#include <memory>
#include <list>
#include "InGameTicketsRepository.h"
#include "Ticket.h"
#include "TicketsBlock.h"

class InGameTicketsRepository_Test : public QObject
{
    Q_OBJECT
public:
    InGameTicketsRepository_Test()
    {
    }

private slots:
    void Init_WillClearWinningTicketsHistory_WhenInitializedRepeatedly();

    void GetTicketsStillInGame_ReturnsAllTickets_WhenNoTicketsDrawnYet();

    void GetWinningTicketsHistory_ReturnsEmptyList_WhenNoTicketsDrawnYet();
    void GetWinningTicketsHistory_Returns2Tickets_When2TicketsDrawnAlready();

    void OnTicketDrawn_WillThrow_WhenTicketsListEmpty();
    void OnTicketDrawn_WillRemoveWinningTicketFromInGameTickets_Always();
    void OnTicketDrawn_WillEmptyInGameTickets_WhenCalledForEverySingleTicket();
};

void InGameTicketsRepository_Test::Init_WillClearWinningTicketsHistory_WhenInitializedRepeatedly()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;

    repository.Init(allTickets);
    repository.OnTicketDrawn(ticket1); // puts 'ticket1' into "winning tickets"
    repository.Init(allTickets); // prize drawing _restarted_!

    auto winningTickets = repository.GetWinningTicketsHistory();
    QCOMPARE(winningTickets.size(), 0U);
}

void InGameTicketsRepository_Test::GetTicketsStillInGame_ReturnsAllTickets_WhenNoTicketsDrawnYet()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Init(allTickets);

    auto stillInGameTickets = repository.GetTicketsStillInGame();

    QCOMPARE(stillInGameTickets.size(), 3U);
    QCOMPARE(stillInGameTickets.front(), ticket1);
    QCOMPARE(*std::next(stillInGameTickets.begin()), ticket2);
    QCOMPARE(stillInGameTickets.back(), ticket3);
}

void InGameTicketsRepository_Test::GetWinningTicketsHistory_ReturnsEmptyList_WhenNoTicketsDrawnYet()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Init(allTickets);

    auto winningTickets = repository.GetWinningTicketsHistory();

    QCOMPARE(winningTickets.size(), 0U);
}

void InGameTicketsRepository_Test::GetWinningTicketsHistory_Returns2Tickets_When2TicketsDrawnAlready()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Init(allTickets);

    repository.OnTicketDrawn(ticket1);
    repository.OnTicketDrawn(ticket3);
    auto winningTickets = repository.GetWinningTicketsHistory();

    // the most recently drawn winning ticket shall be at 'front'
    QCOMPARE(winningTickets.size(), 2U);
    QCOMPARE(winningTickets.front(), ticket3);
    QCOMPARE(winningTickets.back(), ticket1);
}

void InGameTicketsRepository_Test::OnTicketDrawn_WillThrow_WhenTicketsListEmpty()
{
    InGameTicketsRepository repository;

    QVERIFY_EXCEPTION_THROWN(repository.OnTicketDrawn(std::shared_ptr<Ticket>()), std::invalid_argument);
}

void InGameTicketsRepository_Test::OnTicketDrawn_WillRemoveWinningTicketFromInGameTickets_Always()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Init(allTickets);

    repository.OnTicketDrawn(ticket2);

    auto stillInGameTickets = repository.GetTicketsStillInGame();
    QCOMPARE(stillInGameTickets.size(), 2U);
    QCOMPARE(stillInGameTickets.front(), ticket1);
    QCOMPARE(stillInGameTickets.back(), ticket3);
}

void InGameTicketsRepository_Test::OnTicketDrawn_WillEmptyInGameTickets_WhenCalledForEverySingleTicket()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Init(allTickets);

    repository.OnTicketDrawn(ticket2);
    repository.OnTicketDrawn(ticket3);
    repository.OnTicketDrawn(ticket1);

    auto stillInGameTickets = repository.GetTicketsStillInGame();
    QCOMPARE(stillInGameTickets.size(), 0U);
}


#include "InGameTicketsRepository_Tests.moc"

DECLARE_TEST(InGameTicketsRepository_Test)
