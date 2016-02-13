#include "AutoTest.h"
#include "tombolalib_global.h"

#include <QSignalSpy>
#include "TombolaDocument.h"
#include "TicketDrawExecutor.h"

class TicketDrawExecutor_Test : public QObject
{
    Q_OBJECT
public:
    TicketDrawExecutor_Test()
    {
    }

private slots:
    void onPrizeDrawingStartUp_WillCallInitOfSingleTicketDrawVMs_Always();
    void onPrizeDrawingStartUp_WillPassNonEmptyRepositoryToSingleTicketDrawVMs_Always();

    void remainingPrizesCount_ReturnsTotalPrizesCount_WhenNoTicketDrawExecutedYet();
    void remainingPrizesCount_WillReturnOneLess_WhenOneTicketAchievedTheWinningState();
    void remainingPrizesCount_WillReturnZero_For3PrizesWhen3TicketsAchievedTheWinningState();
    void remainingPrizesCount_WillReturnTen_IfPrizesCountChangedToTenBeforeStartUpWasCalled();

    void onTriggerByUser_WillQueryTicketDrawLeftOnly_ByDefault();
    void onTriggerByUser_WillQueryTicketDrawRight_WhenTicketDrawLeftRejectsTheRequest();
    void onTriggerByUser_WontQueryTicketDrawRight_WhenTicketDrawLeftAcceptsTheRequest();
    void onTriggerByUser_DoesNothing_WhenAllPrizesDrawnAlready();
    void onTriggerByUser_DoesNothing_WhenOnePrizeLeftOnlyButOneDrawStillSpinning();
};

class Fake_SingleTicketDraw_ViewModel : public SingleTicketDraw_ViewModel
{
public:
    Fake_SingleTicketDraw_ViewModel(QObject* parent = 0)
        : SingleTicketDraw_ViewModel(parent)
    {}
    bool CalledInit = false;
    virtual void Init(InGameTicketsRepository* inGameTicketsRepository) override
    {
        SingleTicketDraw_ViewModel::Init(inGameTicketsRepository);
        CalledInit = true;
    }
    bool CalledOnTriggerByUser = false;
    SingleTicketDraw_ViewModel::ResultOfUserTrigger ForcedOnTriggerByUserResult;
    virtual SingleTicketDraw_ViewModel::ResultOfUserTrigger onTriggerByUser() override
    {
        CalledOnTriggerByUser = true;
        return ForcedOnTriggerByUserResult;
    }
    InGameTicketsRepository* PrivateInGameRepository()
    {
        return m_InGameTicketsRepository;
    }
    void ForceEmitTicketWinningPositionRequested()
    {
        emit ticketWinningPositionRequested();
    }
};

void TicketDrawExecutor_Test::onPrizeDrawingStartUp_WillCallInitOfSingleTicketDrawVMs_Always()
{
    TombolaDocument document;
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawExecutor.onPrizeDrawingStartUp();

    QVERIFY(ticketDrawLeft->CalledInit);
    QVERIFY(ticketDrawRight->CalledInit);
}

void TicketDrawExecutor_Test::onPrizeDrawingStartUp_WillPassNonEmptyRepositoryToSingleTicketDrawVMs_Always()
{
    TombolaDocument document;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawExecutor.onPrizeDrawingStartUp();

    QCOMPARE(ticketDrawLeft->PrivateInGameRepository(), ticketDrawRight->PrivateInGameRepository());
    QCOMPARE(ticketDrawLeft->PrivateInGameRepository()->GetTicketsStillInGame().size(), 1U);
    QCOMPARE(ticketDrawRight->PrivateInGameRepository()->GetTicketsStillInGame().size(), 1U);
}

void TicketDrawExecutor_Test::remainingPrizesCount_ReturnsTotalPrizesCount_WhenNoTicketDrawExecutedYet()
{
    const int totalPrizesCount = 50;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    int remainingPrizes = ticketDrawExecutor.property("remainingPrizesCount").toInt();

    QCOMPARE(remainingPrizes, totalPrizesCount);
}

void TicketDrawExecutor_Test::remainingPrizesCount_WillReturnOneLess_WhenOneTicketAchievedTheWinningState()
{
    const int totalPrizesCount = 50;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(remainingPrizesCountChanged()));

    ticketDrawLeft->ForceEmitTicketWinningPositionRequested();
    int remainingPrizes = ticketDrawExecutor.property("remainingPrizesCount").toInt();

    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(remainingPrizes, totalPrizesCount - 1);
}

void TicketDrawExecutor_Test::remainingPrizesCount_WillReturnZero_For3PrizesWhen3TicketsAchievedTheWinningState()
{
    const int totalPrizesCount = 3;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    block->SetTicketSold(6, true);
    block->SetTicketSold(7, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(remainingPrizesCountChanged()));

    ticketDrawLeft->ForceEmitTicketWinningPositionRequested();
    ticketDrawRight->ForceEmitTicketWinningPositionRequested();
    ticketDrawRight->ForceEmitTicketWinningPositionRequested();
    int remainingPrizes = ticketDrawExecutor.property("remainingPrizesCount").toInt();

    QCOMPARE(signalSpy.count(), 3);
    QCOMPARE(remainingPrizes, 0);
}

void TicketDrawExecutor_Test::remainingPrizesCount_WillReturnTen_IfPrizesCountChangedToTenBeforeStartUpWasCalled()
{
    const int totalPrizesCount = 5;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(remainingPrizesCountChanged()));

    document.PrizesCount = 10; // TicketDrawExecutor ctor has "seen" 5, now it's 10
    ticketDrawExecutor.onPrizeDrawingStartUp();
    int remainingPrizes = ticketDrawExecutor.property("remainingPrizesCount").toInt();

    QCOMPARE(remainingPrizes, 10);
    QCOMPARE(signalSpy.count(), 1);
}

void TicketDrawExecutor_Test::onTriggerByUser_WillQueryTicketDrawLeftOnly_ByDefault()
{
    const int totalPrizesCount = 3;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    block->SetTicketSold(6, true);
    block->SetTicketSold(7, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawExecutor.onTriggerByUser();

    QCOMPARE(ticketDrawLeft->CalledOnTriggerByUser, true);
    QCOMPARE(ticketDrawRight->CalledOnTriggerByUser, false);
}

void TicketDrawExecutor_Test::onTriggerByUser_WillQueryTicketDrawRight_WhenTicketDrawLeftRejectsTheRequest()
{
    const int totalPrizesCount = 3;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    block->SetTicketSold(6, true);
    block->SetTicketSold(7, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Rejected;
    ticketDrawExecutor.onTriggerByUser();

    QCOMPARE(ticketDrawRight->CalledOnTriggerByUser, true);
}

void TicketDrawExecutor_Test::onTriggerByUser_WontQueryTicketDrawRight_WhenTicketDrawLeftAcceptsTheRequest()
{
    const int totalPrizesCount = 3;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    block->SetTicketSold(6, true);
    block->SetTicketSold(7, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser();

    QCOMPARE(ticketDrawRight->CalledOnTriggerByUser, false);
}

void TicketDrawExecutor_Test::onTriggerByUser_DoesNothing_WhenAllPrizesDrawnAlready()
{
    const int totalPrizesCount = 2;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    block->SetTicketSold(6, true);
    block->SetTicketSold(7, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser(); // spin up LEFT
    ticketDrawExecutor.onTriggerByUser(); // spin up RIGHT
    ticketDrawLeft->ForceEmitTicketWinningPositionRequested();
    ticketDrawRight->ForceEmitTicketWinningPositionRequested();


    ticketDrawLeft->CalledOnTriggerByUser = false;
    ticketDrawRight->CalledOnTriggerByUser = false;
    ticketDrawExecutor.onTriggerByUser();


    QCOMPARE(ticketDrawLeft->CalledOnTriggerByUser, false);
    QCOMPARE(ticketDrawRight->CalledOnTriggerByUser, false);
}

void TicketDrawExecutor_Test::onTriggerByUser_DoesNothing_WhenOnePrizeLeftOnlyButOneDrawStillSpinning()
{
    const int totalPrizesCount = 2;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    block->SetTicketSold(6, true);
    block->SetTicketSold(7, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight);
    ticketDrawExecutor.onPrizeDrawingStartUp();

    // both left&righ started spinning, but only left has a winning ticket yet; right still spinning
    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser();
    ticketDrawExecutor.onTriggerByUser();
    ticketDrawLeft->ForceEmitTicketWinningPositionRequested();


    ticketDrawLeft->CalledOnTriggerByUser = false;
    ticketDrawRight->CalledOnTriggerByUser = false;
    ticketDrawExecutor.onTriggerByUser();


    QCOMPARE(ticketDrawLeft->CalledOnTriggerByUser, false);
    QCOMPARE(ticketDrawRight->CalledOnTriggerByUser, false);
}


#include "TicketDrawExecutor_Tests.moc"

DECLARE_TEST(TicketDrawExecutor_Test)

