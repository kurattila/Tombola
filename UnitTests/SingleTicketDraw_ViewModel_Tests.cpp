#include "AutoTest.h"
#include "tombolalib_global.h"

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
    void onTriggerByUser_WillAcceptQuery_WhenRightAfterStart();
    void onTriggerByUser_WillRejectQuery_WhenTicketSpinning();
    void onTriggerByUser_WillAcceptQuery_WhenWinningTicketShown();
    void onTriggerByUser_WillRejectQuery_DuringWinningTicketVanishing();
};

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
