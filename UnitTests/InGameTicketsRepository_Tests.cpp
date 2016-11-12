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

    void GetUntouchedTickets_ReturnsAllTickets_WhenNoTicketsDrawnYet();

    void GetWinningTicketsHistory_ReturnsEmptyList_WhenNoTicketsDrawnYet();
    void GetWinningTicketsHistory_Returns2Tickets_When2TicketsDrawnAlready();

    void OnTicketDrawnBegin_WillThrow_WhenTicketsListEmpty();
    void OnTicketDrawnBegin_WillRemoveWinningTicketFromUntouchedTickets_Always();
    void OnTicketDrawnBegin_WillEmptyUntouchedTickets_WhenCalledForEverySingleTicket();
    void TicketDrawn_WontBePartOfWinningTickets_WhenOnTicketDrawnEndNotCalledYet();
    void TicketDrawn_WillBecomePartOfWinningTickets_WhenOnTicketDrawnEndCalled();

    // Memento-based behavior
    void RestoreFromMemento_RestoresTwoUntouchedTickets_IfSavedWithOnlyTwoRemainingTickets();
    void RestoreFromMemento_RestoresOneWinningTicket_IfSavedWithOneWinningTickets();
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

void InGameTicketsRepository_Test::GetUntouchedTickets_ReturnsAllTickets_WhenNoTicketsDrawnYet()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Reset(allTickets);

    auto untouchedTickets = repository.GetUntouchedTickets();

    QCOMPARE(untouchedTickets.size(), 3U);
    QCOMPARE(untouchedTickets.front(), ticket1);
    QCOMPARE(*std::next(untouchedTickets.begin()), ticket2);
    QCOMPARE(untouchedTickets.back(), ticket3);
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

void InGameTicketsRepository_Test::OnTicketDrawnBegin_WillRemoveWinningTicketFromUntouchedTickets_Always()
{
    TicketsBlock block(10);
    auto ticket1 = std::make_shared<Ticket>(1, block);
    auto ticket2 = std::make_shared<Ticket>(2, block);
    auto ticket3 = std::make_shared<Ticket>(3, block);
    std::list<std::shared_ptr<Ticket>> allTickets = { ticket1, ticket2, ticket3 };
    InGameTicketsRepository repository;
    repository.Reset(allTickets);

    repository.OnTicketDrawnPrepare(ticket2);

    auto untouchedTickets = repository.GetUntouchedTickets();
    QCOMPARE(untouchedTickets.size(), 2U);
    QCOMPARE(untouchedTickets.front(), ticket1);
    QCOMPARE(untouchedTickets.back(), ticket3);
}

void InGameTicketsRepository_Test::OnTicketDrawnBegin_WillEmptyUntouchedTickets_WhenCalledForEverySingleTicket()
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

    auto untouchedTickets = repository.GetUntouchedTickets();
    QCOMPARE(untouchedTickets.size(), 0U);
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

    QCOMPARE(repository.GetUntouchedTickets().size(), 2U);
    QCOMPARE(repository.GetUntouchedTickets().front()->TicketNumber(), 3);
    QCOMPARE(repository.GetUntouchedTickets().back()->TicketNumber(), 1);
}

void InGameTicketsRepository_Test::RestoreFromMemento_RestoresOneWinningTicket_IfSavedWithOneWinningTickets()
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
    origiRepository.OnTicketDrawnPrepare(origiTicket2);
    origiRepository.OnTicketDrawnCommit(origiTicket2);
    std::unique_ptr<IMemento> memento(origiRepository.SaveToMemento());

    TicketsBlocksSet allBlocks;
    auto block = allBlocks.AddBlock(3);
    block->Name = origiBlock->Name;
    auto block2 = allBlocks.AddBlock(1);
    block2->Name = origiBlock2->Name;
    InGameTicketsRepository repository;
    repository.RestoreFromMemento(memento.get(), &allBlocks);

    QCOMPARE(repository.GetWinningTicketsHistory().size(), 1U);
    QCOMPARE(repository.GetWinningTicketsHistory().front()->TicketNumber(), 2);
}


#include "InGameTicketsRepository_Tests.moc"

DECLARE_TEST(InGameTicketsRepository_Test)
