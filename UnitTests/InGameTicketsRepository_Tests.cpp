#include "AutoTest.h"
#include "tombolalib_global.h"

#include <memory>
#include <list>
#include "InGameTicketsRepository.h"
#include "Ticket.h"
#include "TicketsBlock.h"
#include "TicketsBlocksSet.h"

class InGameTicketsRepository_Test : public QObject
{
    Q_OBJECT
public:
    InGameTicketsRepository_Test()
    {
    }

private slots:
    void Reset_WillClearWinningTicketsHistory_WhenInitializedRepeatedly();

    void GetTicketsStillInGame_ReturnsAllTickets_WhenNoTicketsDrawnYet();

    void GetWinningTicketsHistory_ReturnsEmptyList_WhenNoTicketsDrawnYet();
    void GetWinningTicketsHistory_Returns2Tickets_When2TicketsDrawnAlready();

    void OnTicketDrawnBegin_WillThrow_WhenTicketsListEmpty();
    void OnTicketDrawnBegin_WillRemoveWinningTicketFromInGameTickets_Always();
    void OnTicketDrawnBegin_WillEmptyInGameTickets_WhenCalledForEverySingleTicket();
    void TicketDrawn_WontBePartOfWinningTickets_WhenOnTicketDrawnEndNotCalledYet();
    void TicketDrawn_WillBecomePartOfWinningTickets_WhenOnTicketDrawnEndCalled();

    // Memento-based behavior
    void RestoreFromMemento_RestoresTwoUntouchedTickets_IfSavedWithOnlyTwoRemainingTickets();
};

void InGameTicketsRepository_Test::Reset_WillClearWinningTicketsHistory_WhenInitializedRepeatedly()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;

    repository.Reset(allTickets);
    repository.OnTicketDrawnPrepare(ticket1); // puts 'ticket1' into "winning tickets"
    repository.Reset(allTickets); // prize drawing _restarted_!

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
    repository.Reset(allTickets);

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
    repository.Reset(allTickets);

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
    repository.Reset(allTickets);

    repository.OnTicketDrawnPrepare(ticket1);
    repository.OnTicketDrawnPrepare(ticket3);
    repository.OnTicketDrawnCommit(ticket1);
    repository.OnTicketDrawnCommit(ticket3);
    auto winningTickets = repository.GetWinningTicketsHistory();

    // the most recently drawn winning ticket shall be at 'front'
    QCOMPARE(winningTickets.size(), 2U);
    QCOMPARE(winningTickets.front(), ticket3);
    QCOMPARE(winningTickets.back(), ticket1);
}

void InGameTicketsRepository_Test::OnTicketDrawnBegin_WillThrow_WhenTicketsListEmpty()
{
    InGameTicketsRepository repository;

    QVERIFY_EXCEPTION_THROWN(repository.OnTicketDrawnPrepare(std::shared_ptr<Ticket>()), std::invalid_argument);
}

void InGameTicketsRepository_Test::OnTicketDrawnBegin_WillRemoveWinningTicketFromInGameTickets_Always()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Reset(allTickets);

    repository.OnTicketDrawnPrepare(ticket2);

    auto stillInGameTickets = repository.GetTicketsStillInGame();
    QCOMPARE(stillInGameTickets.size(), 2U);
    QCOMPARE(stillInGameTickets.front(), ticket1);
    QCOMPARE(stillInGameTickets.back(), ticket3);
}

void InGameTicketsRepository_Test::OnTicketDrawnBegin_WillEmptyInGameTickets_WhenCalledForEverySingleTicket()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Reset(allTickets);

    repository.OnTicketDrawnPrepare(ticket2);
    repository.OnTicketDrawnPrepare(ticket3);
    repository.OnTicketDrawnPrepare(ticket1);

    auto stillInGameTickets = repository.GetTicketsStillInGame();
    QCOMPARE(stillInGameTickets.size(), 0U);
}

void InGameTicketsRepository_Test::TicketDrawn_WontBePartOfWinningTickets_WhenOnTicketDrawnEndNotCalledYet()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Reset(allTickets);

    repository.OnTicketDrawnPrepare(ticket2);
    repository.OnTicketDrawnPrepare(ticket3);

    auto& winningTickets = repository.GetWinningTicketsHistory();
    QCOMPARE(winningTickets.size(), 0U);
}

void InGameTicketsRepository_Test::TicketDrawn_WillBecomePartOfWinningTickets_WhenOnTicketDrawnEndCalled()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Reset(allTickets);

    repository.OnTicketDrawnPrepare(ticket2);
    repository.OnTicketDrawnPrepare(ticket3);
    repository.OnTicketDrawnCommit(ticket2);
    repository.OnTicketDrawnCommit(ticket3);

    auto& winningTickets = repository.GetWinningTicketsHistory();
    QCOMPARE(winningTickets.size(), 2U);
    QCOMPARE(winningTickets.front(), ticket3);
    QCOMPARE(winningTickets.back(), ticket2);
}

void InGameTicketsRepository_Test::RestoreFromMemento_RestoresTwoUntouchedTickets_IfSavedWithOnlyTwoRemainingTickets()
{
    TicketsBlockFactory blockFactory;
    std::unique_ptr<TicketsBlock> origiBlock(blockFactory.CreateSingleBlock(10));
    std::unique_ptr<TicketsBlock> origiBlock2(blockFactory.CreateSingleBlock(10));
    auto origiTicket1 = std::make_shared<Ticket>(1, *origiBlock);
    auto origiTicket2 = std::make_shared<Ticket>(2, *origiBlock);
    auto origiTicket3 = std::make_shared<Ticket>(3, *origiBlock);
    auto origiTicket4 = std::make_shared<Ticket>(1, *origiBlock2);
    std::list<std::shared_ptr<Ticket>> origiAllTickets = { origiTicket1, origiTicket2, origiTicket3, origiTicket4 };
    InGameTicketsRepository origiRepository;
    origiRepository.Reset(origiAllTickets);
    origiRepository.OnTicketDrawnPrepare(origiTicket1);
    origiRepository.OnTicketDrawnPrepare(origiTicket2);
    origiRepository.OnTicketDrawnCommit(origiTicket1);
    origiRepository.OnTicketDrawnCommit(origiTicket2);
    std::unique_ptr<IMemento> memento(origiRepository.SaveToMemento());

    TicketsBlocksSet allBlocks;
    auto block = allBlocks.AddBlock(10);
    block->Name = origiBlock->Name;
    auto block2 = allBlocks.AddBlock(10);
    block2->Name = origiBlock2->Name;
    InGameTicketsRepository repository;
    repository.RestoreFromMemento(memento.get(), &allBlocks);

    QCOMPARE(repository.GetTicketsStillInGame().size(), 2U);
    QCOMPARE(repository.GetTicketsStillInGame().front()->TicketNumber(), 3);
    QCOMPARE(repository.GetTicketsStillInGame().back()->TicketNumber(), 1);
}


#include "InGameTicketsRepository_Tests.moc"

DECLARE_TEST(InGameTicketsRepository_Test)
