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
    void onPrizeDrawingStartUp_WillClearSpinningStatuses_WhenCalledRepeatedly();

    void remainingPrizesCount_ReturnsTotalPrizesCount_WhenNoTicketDrawExecutedYet();
    void remainingPrizesCount_WillReturnOneLess_WhenOneTicketAchievedTheWinningState();
    void remainingPrizesCount_WillReturnZero_For3PrizesWhen3TicketsAchievedTheWinningState();
    void remainingPrizesCount_WillReturnTen_IfPrizesCountChangedToTenBeforeStartUpWasCalled();

    void onTriggerByUser_WillQueryTicketDrawLeftOnly_ByDefault();
    void onTriggerByUser_WillQueryTicketDrawRight_WhenTicketDrawLeftRejectsTheRequest();
    void onTriggerByUser_WontQueryTicketDrawRight_WhenTicketDrawLeftAcceptsTheRequest();
    void onTriggerByUser_DoesNothing_WhenAllPrizesDrawnAlready();
    void onTriggerByUser_DoesNothing_WhenOnePrizeLeftOnlyButOneDrawStillSpinning();

    void minAllowedRemainingPrizesCount_IsZero_ByDefault();
    void minAllowedRemainingPrizesCount_IsOne_WhenLeftTicketSpinning();
    void minAllowedRemainingPrizesCount_IsTwo_WhenBothTicketsSpinning();
    void minAllowedRemainingPrizesCount_ComesBackToOneAgain_WhenOneOf2TicketsStopsSpinning();
    void minAllowedRemainingPrizesCount_ComesBackToZeroAgain_WhenBothTicketsStopSpinning();
    void minAllowedRemainingPrizesCount_ComesBackToZeroYetBeforeRemainingPrizesCountGoesToZero_WhenTicketForLastPrizeStopsSpinning();
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawExecutor.onPrizeDrawingStartUp();

    QCOMPARE(ticketDrawLeft->PrivateInGameRepository(), ticketDrawRight->PrivateInGameRepository());
    QCOMPARE(ticketDrawLeft->PrivateInGameRepository()->GetTicketsStillInGame().size(), 1U);
    QCOMPARE(ticketDrawRight->PrivateInGameRepository()->GetTicketsStillInGame().size(), 1U);
}

void TicketDrawExecutor_Test::onPrizeDrawingStartUp_WillClearSpinningStatuses_WhenCalledRepeatedly()
{
    TombolaDocument document;
    document.PrizesCount = 2;
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();
    ticketDrawExecutor.onTriggerByUser(); // spin up LEFT
    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Rejected; // LEFT will reject while spinning
    ticketDrawExecutor.onTriggerByUser(); // spin up RIGHT
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Rejected; // RIGHT will reject while spinning
    ticketDrawLeft->CalledOnTriggerByUser = false;
    ticketDrawRight->CalledOnTriggerByUser = false;

    ticketDrawExecutor.onPrizeDrawingStartUp();

    ticketDrawExecutor.onTriggerByUser(); // will call ticketDrawLeft->onTriggerByUser() only if not spinning, because only 2 prizes exist
    ticketDrawExecutor.onTriggerByUser(); // will call ticketDrawRight->onTriggerByUser() only if not spinning, because only 2 prizes exist
    QVERIFY(ticketDrawLeft->CalledOnTriggerByUser);
    QVERIFY(ticketDrawRight->CalledOnTriggerByUser);
}

void TicketDrawExecutor_Test::remainingPrizesCount_ReturnsTotalPrizesCount_WhenNoTicketDrawExecutedYet()
{
    const int totalPrizesCount = 50;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
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

void TicketDrawExecutor_Test::minAllowedRemainingPrizesCount_IsZero_ByDefault()
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();

    int minAllowedRemainingPrizes = ticketDrawExecutor.property("minAllowedRemainingPrizesCount").toInt();

    QCOMPARE(minAllowedRemainingPrizes, 0);
}

void TicketDrawExecutor_Test::minAllowedRemainingPrizesCount_IsOne_WhenLeftTicketSpinning()
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(minAllowedRemainingPrizesCountChanged()));

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    // ticketDrawLeft->ForceEmitTicketWinningPositionRequested();
    int minAllowedRemainingPrizes = ticketDrawExecutor.property("minAllowedRemainingPrizesCount").toInt();

    QCOMPARE(minAllowedRemainingPrizes, 1);
    QCOMPARE(signalSpy.count(), 1);
}

void TicketDrawExecutor_Test::minAllowedRemainingPrizesCount_IsTwo_WhenBothTicketsSpinning()
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(minAllowedRemainingPrizesCountChanged()));

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    // ticketDrawLeft->ForceEmitTicketWinningPositionRequested();
    // ticketDrawRight->ForceEmitTicketWinningPositionRequested();
    int minAllowedRemainingPrizes = ticketDrawExecutor.property("minAllowedRemainingPrizesCount").toInt();

    QCOMPARE(minAllowedRemainingPrizes, 2);
    QCOMPARE(signalSpy.count(), 2);
}

void TicketDrawExecutor_Test::minAllowedRemainingPrizesCount_ComesBackToOneAgain_WhenOneOf2TicketsStopsSpinning()
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(minAllowedRemainingPrizesCountChanged()));

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    ticketDrawLeft->ForceEmitTicketWinningPositionRequested(); // WinningPosition achieved
    // ticketDrawRight->ForceEmitTicketWinningPositionRequested();
    int minAllowedRemainingPrizes = ticketDrawExecutor.property("minAllowedRemainingPrizesCount").toInt();

    QCOMPARE(minAllowedRemainingPrizes, 1);
    QCOMPARE(signalSpy.count(), 3);
}

void TicketDrawExecutor_Test::minAllowedRemainingPrizesCount_ComesBackToZeroAgain_WhenBothTicketsStopSpinning()
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
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();
    QSignalSpy signalSpy(&ticketDrawExecutor, SIGNAL(minAllowedRemainingPrizesCountChanged()));

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawRight->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    ticketDrawLeft->ForceEmitTicketWinningPositionRequested(); // WinningPosition achieved
    ticketDrawRight->ForceEmitTicketWinningPositionRequested(); // WinningPosition achieved
    int minAllowedRemainingPrizes = ticketDrawExecutor.property("minAllowedRemainingPrizesCount").toInt();

    QCOMPARE(minAllowedRemainingPrizes, 0);
    QCOMPARE(signalSpy.count(), 4);
}

void TicketDrawExecutor_Test::minAllowedRemainingPrizesCount_ComesBackToZeroYetBeforeRemainingPrizesCountGoesToZero_WhenTicketForLastPrizeStopsSpinning()
{
    // Bugfix unit test:
    // When the last ticket is spinning, which will draw the last prize, then the SpinBox's "minimumValue" property
    // shall be updated yet _before_ its "value" property. Otherwise, "value" couldn't go down to 0, when "minimumValue"
    // was still 1.

    const int totalPrizesCount = 1;
    TombolaDocument document;
    document.PrizesCount = totalPrizesCount;
    auto block = document.AllTicketsBlocksSet->AddBlock();
    block->SetTicketSold(5, true);
    auto ticketDrawLeft = new Fake_SingleTicketDraw_ViewModel();
    auto ticketDrawRight = new Fake_SingleTicketDraw_ViewModel();
    TicketDrawExecutor ticketDrawExecutor(document, ticketDrawLeft, ticketDrawRight); // will auto-delete ticketDrawLeft and ticketDrawRight
    ticketDrawExecutor.onPrizeDrawingStartUp();
    int minAllowedValueBeforeLastTicketStop = 100;
    connect(&ticketDrawExecutor, &TicketDrawExecutor::remainingPrizesCountChanged, [&]
    {
        auto prop = ticketDrawExecutor.property("minAllowedRemainingPrizesCount");
        if (prop.isValid()) // beware of mistyping property name
            minAllowedValueBeforeLastTicketStop = prop.toInt();
    });

    ticketDrawLeft->ForcedOnTriggerByUserResult = SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted;
    ticketDrawExecutor.onTriggerByUser(); // started but WinningPosition not achieved yet
    ticketDrawLeft->ForceEmitTicketWinningPositionRequested(); // WinningPosition achieved

    QCOMPARE(minAllowedValueBeforeLastTicketStop, 0);
}


#include "TicketDrawExecutor_Tests.moc"

DECLARE_TEST(TicketDrawExecutor_Test)

