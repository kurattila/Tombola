#include "AutoTest.h"
#include "tombolalib_global.h"

#include <QSignalSpy>
#include "SingleTicketDraw_ViewModel.h"
#include "InGameTicketsRepository.h"

class SingleTicketDraw_ViewModel_Test : public QObject
{
    Q_OBJECT
public:
    SingleTicketDraw_ViewModel_Test()
    {
    }

private slots:
    void Init_WillClearPreviousWinningTickets_WhenPrizeDrawingRepeated();
    void Init_WillResetQmlTicketPosition_WhenPrizeDrawingRepeated();
    void Init_WillResetTicketDrawState_WhenPrizeDrawingRepeated();

    void onTriggerByUser_WillAcceptQuery_WhenRightAfterStart();
    void onTriggerByUser_WillRejectQuery_WhenTicketSpinning();
    void onTriggerByUser_WillAcceptQuery_WhenWinningTicketShown();
    void onTriggerByUser_WillRejectQuery_DuringWinningTicketVanishing();
};

void SingleTicketDraw_ViewModel_Test::Init_WillClearPreviousWinningTickets_WhenPrizeDrawingRepeated()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block), std::make_shared<Ticket>(2, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;

    singleTicketDrawVM.Init(&ticketsRepository);
    singleTicketDrawVM.onTriggerByUser();
    singleTicketDrawVM.onWinningTicketStateAchieved();
    singleTicketDrawVM.onVanishedTicketStateAchieved();
    singleTicketDrawVM.Init(&ticketsRepository);

    int winningTicketsCount = singleTicketDrawVM.rowCount();
    QCOMPARE(winningTicketsCount, 0);
}

void SingleTicketDraw_ViewModel_Test::Init_WillResetQmlTicketPosition_WhenPrizeDrawingRepeated()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block), std::make_shared<Ticket>(2, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;

    singleTicketDrawVM.Init(&ticketsRepository);
    singleTicketDrawVM.onTriggerByUser();
    singleTicketDrawVM.onWinningTicketStateAchieved();
    singleTicketDrawVM.onVanishedTicketStateAchieved();
    QSignalSpy signalSpy(&singleTicketDrawVM, SIGNAL(ticketStartupPositionRequested()));
    singleTicketDrawVM.Init(&ticketsRepository);

    QCOMPARE(signalSpy.count(), 1);
}

void SingleTicketDraw_ViewModel_Test::Init_WillResetTicketDrawState_WhenPrizeDrawingRepeated()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block), std::make_shared<Ticket>(2, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;

    singleTicketDrawVM.Init(&ticketsRepository);
    singleTicketDrawVM.onTriggerByUser();
    singleTicketDrawVM.Init(&ticketsRepository);

    auto result = singleTicketDrawVM.onTriggerByUser();
    QCOMPARE(result, SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted); // accepted, since the 2nd Init() call changed state from AwaitingWinningTicket to NotStarted
}

void SingleTicketDraw_ViewModel_Test::onTriggerByUser_WillAcceptQuery_WhenRightAfterStart()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;
    singleTicketDrawVM.Init(&ticketsRepository);

    auto result = singleTicketDrawVM.onTriggerByUser();

    QCOMPARE(result, SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted);
}

void SingleTicketDraw_ViewModel_Test::onTriggerByUser_WillRejectQuery_WhenTicketSpinning()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;
    singleTicketDrawVM.Init(&ticketsRepository);

    singleTicketDrawVM.onTriggerByUser(); // will make the ticket spin
    auto result = singleTicketDrawVM.onTriggerByUser(); // already spinning: reject query

    QCOMPARE(result, SingleTicketDraw_ViewModel::ResultOfUserTrigger::Rejected);
}

void SingleTicketDraw_ViewModel_Test::onTriggerByUser_WillAcceptQuery_WhenWinningTicketShown()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;
    singleTicketDrawVM.Init(&ticketsRepository);

    singleTicketDrawVM.onTriggerByUser(); // will make the ticket spin
    singleTicketDrawVM.onWinningTicketStateAchieved(); // spinning just stopped
    auto result = singleTicketDrawVM.onTriggerByUser(); // user can make it vanish: accept query

    QCOMPARE(result, SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted);
}

void SingleTicketDraw_ViewModel_Test::onTriggerByUser_WillRejectQuery_DuringWinningTicketVanishing()
{
    TicketsBlock block(100);
    std::list<std::shared_ptr<Ticket>> tickets = { std::make_shared<Ticket>(1, block) };
    InGameTicketsRepository ticketsRepository;
    ticketsRepository.Init(tickets);
    SingleTicketDraw_ViewModel singleTicketDrawVM;
    singleTicketDrawVM.Init(&ticketsRepository);

    singleTicketDrawVM.onTriggerByUser(); // will make the ticket spin
    singleTicketDrawVM.onWinningTicketStateAchieved(); // spinning just stopped
    singleTicketDrawVM.onTriggerByUser(); // user can make it vanish + autostart spin: so accept query
    auto result = singleTicketDrawVM.onTriggerByUser(); // can't accept user input during vanishing!
                                                        // Accepting would mean to TicketDrawExecutor that it has made our ticket spin a second time which is impossible

    QCOMPARE(result, SingleTicketDraw_ViewModel::ResultOfUserTrigger::Rejected);
}

#include "SingleTicketDraw_ViewModel_Tests.moc"

DECLARE_TEST(SingleTicketDraw_ViewModel_Test)
