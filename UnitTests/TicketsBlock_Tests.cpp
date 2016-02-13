#include "AutoTest.h"
#include "tombolalib_global.h"

#include "TicketsBlock.h"

class TicketsBlock_Test : public QObject
{
    Q_OBJECT
public:
    TicketsBlock_Test()
    {
    }

private slots:
    void GetSoldTicketIds_WillReturnEmptyList_ByDefault();
    void GetSoldTicketIds_WillReturn1To5_WhenAllTicketsSoldInAFiveTicketBlock();
    void ToggleSoldTicket_WillMarkTicketAsSold_WhenNotSoldYet();
    void ToggleSoldTicket_WillMarkTicketAsNotSold_WhenAlreadySold();
    void ToggleSoldTicket_WillMarkLastTicketAsSold_When100TicketsInBlock();
    void ToggleSoldTicket_WontMarkAnyTicketsAsSold_When100TicketsInBlockButMarkingMinus5thTicket();
    void ToggleSoldTicket_WontMarkAnyTicketsAsSold_When100TicketsInBlockButMarking101thTicket();

    void SetTicketSold_WillSetTicketAsSold_WhenTrueSpecifiedAndTicketWasUnsoldBefore();
    void SetTicketSold_WillLeaveTicketAsSold_WhenTrueSpecifiedTicketWasSoldBefore();
    void SetTicketSold_WillClearTicketSoldFlag_WhenFalseSpecifiedAndTicketWasUnsoldBefore();
    void SetTicketSold_WillClearTicketSoldFlag_WhenFalseSpecifiedAndTicketWasSoldBefore();
    void SetTicketSold_WontTouchSoldState_WhenTicketOutsideTheValidRangeSpecified();

    void RangeOperationInProgress_WillSetStartingTicketAsSold_WhenNoOtherTicketsWereTouched();
    void RangeOperationInProgress_WillSetStartingTicketAsSold_WhenSingleTicketsTouchedSeveralTimes();
    void RangeOperationInProgress_WillSetAllTicketFromBeginToEndAsSold_WhenBeginLowerThanEnd();
    void RangeOperationInProgress_WillSetAllTicketFromBeginToEndAsSold_WhenEndLowerThanBegin();
    void RangeOperationInProgress_WontChangeTicketsBeyondEnd_EvenIfTheyGotTouchedDuringDragging();
    void RangeOperationInProgress_WillSetUntouchedTickets_WhenTouchingTicketsFromDifferentRows();
    void RangeOperationInProgress_WillMarkTicketsAsUnsold_WhenStartingFromSoldTicket();
    void RangeOperationInProgress_WillRestorePreviousTicketsState_WhenTicketTouchedButFinallyNotIncludedInRangeOperation();
    void RangeOperationInProgress_WillIgnoreAnyRequests_WhenHigherThanRangeTicketsSpecified();
    void RangeOperationInProgress_WillIgnoreAnyRequests_WhenLowerThanRangeTicketsSpecified();
    void RangeOperationInProgress_WillIgnoreAnyRequests_WhenBeginRangeOperationWasNotAtValidTicketId();
    void RangeOperationInProgress_WillReverseAllTicketsOutsideTheRange_EvenIfRevertingWholeRowsIsNeeded();
};



void TicketsBlock_Test::GetSoldTicketIds_WillReturnEmptyList_ByDefault()
{
    TicketsBlock block(100);

    auto soldTickets = block.GetSoldTicketIds();

    QCOMPARE(soldTickets.size(), 0U);
}

void TicketsBlock_Test::GetSoldTicketIds_WillReturn1To5_WhenAllTicketsSoldInAFiveTicketBlock()
{
    TicketsBlock block(5);
    for (int ticketId = 1; ticketId <= 5; ticketId++)
        block.ToggleSoldTicket(ticketId);;

    auto soldTickets = block.GetSoldTicketIds();

    QCOMPARE(soldTickets.size(), 5U);
    QCOMPARE(*std::next(soldTickets.begin(), 0), 1);
    QCOMPARE(*std::next(soldTickets.begin(), 1), 2);
    QCOMPARE(*std::next(soldTickets.begin(), 2), 3);
    QCOMPARE(*std::next(soldTickets.begin(), 3), 4);
    QCOMPARE(*std::next(soldTickets.begin(), 4), 5);
}

void TicketsBlock_Test::ToggleSoldTicket_WillMarkTicketAsSold_WhenNotSoldYet()
{
    TicketsBlock block(100);

    block.ToggleSoldTicket(10);
    block.ToggleSoldTicket(11);

    auto soldTickets = block.GetSoldTicketIds();
    QCOMPARE(soldTickets.size(), 2U);
}

void TicketsBlock_Test::ToggleSoldTicket_WillMarkTicketAsNotSold_WhenAlreadySold()
{
    TicketsBlock block(100);
    block.ToggleSoldTicket(10);
    block.ToggleSoldTicket(11);

    block.ToggleSoldTicket(10);

    auto soldTickets = block.GetSoldTicketIds();
    QCOMPARE(soldTickets.size(), 1U);
    QCOMPARE(*soldTickets.cbegin(), 11);
}

void TicketsBlock_Test::ToggleSoldTicket_WillMarkLastTicketAsSold_When100TicketsInBlock()
{
    TicketsBlock block(100);
    block.ToggleSoldTicket(100);

    auto soldTickets = block.GetSoldTicketIds();

    QCOMPARE(soldTickets.size(), 1U);
    QCOMPARE(*soldTickets.begin(), 100);
}

void TicketsBlock_Test::ToggleSoldTicket_WontMarkAnyTicketsAsSold_When100TicketsInBlockButMarkingMinus5thTicket()
{
    TicketsBlock block(100);
    block.ToggleSoldTicket(-5);

    auto soldTickets = block.GetSoldTicketIds();

    QCOMPARE(soldTickets.size(), 0U);
}

void TicketsBlock_Test::ToggleSoldTicket_WontMarkAnyTicketsAsSold_When100TicketsInBlockButMarking101thTicket()
{
    TicketsBlock block(100);
    block.ToggleSoldTicket(101);

    auto soldTickets = block.GetSoldTicketIds();

    QCOMPARE(soldTickets.size(), 0U);
}

void TicketsBlock_Test::SetTicketSold_WillSetTicketAsSold_WhenTrueSpecifiedAndTicketWasUnsoldBefore()
{
    TicketsBlock block(2);

    block.SetTicketSold(2, true);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    QCOMPARE(*block.GetSoldTicketIds().begin(), 2);
}

void TicketsBlock_Test::SetTicketSold_WillLeaveTicketAsSold_WhenTrueSpecifiedTicketWasSoldBefore()
{
    TicketsBlock block(2);
    block.ToggleSoldTicket(2);

    block.SetTicketSold(2, true);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    QCOMPARE(*block.GetSoldTicketIds().begin(), 2);
}

void TicketsBlock_Test::SetTicketSold_WillClearTicketSoldFlag_WhenFalseSpecifiedAndTicketWasUnsoldBefore()
{
    TicketsBlock block(2);
    block.ToggleSoldTicket(1);

    block.SetTicketSold(2, false);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    QCOMPARE(*block.GetSoldTicketIds().begin(), 1);
}

void TicketsBlock_Test::SetTicketSold_WillClearTicketSoldFlag_WhenFalseSpecifiedAndTicketWasSoldBefore()
{
    TicketsBlock block(2);
    block.ToggleSoldTicket(1);
    block.ToggleSoldTicket(2);

    block.SetTicketSold(2, false);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    QCOMPARE(*block.GetSoldTicketIds().begin(), 1);
}

void TicketsBlock_Test::SetTicketSold_WontTouchSoldState_WhenTicketOutsideTheValidRangeSpecified()
{
    TicketsBlock block(100);

    block.SetTicketSold(-1, true);
    block.SetTicketSold(101, true);

    QCOMPARE(block.GetSoldTicketIds().size(), 0U);
}

void TicketsBlock_Test::RangeOperationInProgress_WillSetStartingTicketAsSold_WhenNoOtherTicketsWereTouched()
{
    TicketsBlock block(3);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.EndRangeOperation(2);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    QCOMPARE(*block.GetSoldTicketIds().begin(), 2);
}

void TicketsBlock_Test::RangeOperationInProgress_WillSetStartingTicketAsSold_WhenSingleTicketsTouchedSeveralTimes()
{
    TicketsBlock block(3);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2); // twice
    block.RangeOperationInProgress(2); // twice
    block.EndRangeOperation(2);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    QCOMPARE(*block.GetSoldTicketIds().begin(), 2);
}

void TicketsBlock_Test::RangeOperationInProgress_WillSetAllTicketFromBeginToEndAsSold_WhenBeginLowerThanEnd()
{
    TicketsBlock block(5);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.RangeOperationInProgress(3);
    block.RangeOperationInProgress(4);
    block.EndRangeOperation(4);

    QCOMPARE(block.GetSoldTicketIds().size(), 3U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 2);
    QCOMPARE(*(++it), 3);
    QCOMPARE(*(++it), 4);
}

void TicketsBlock_Test::RangeOperationInProgress_WillSetAllTicketFromBeginToEndAsSold_WhenEndLowerThanBegin()
{
    TicketsBlock block(5);

    block.BeginRangeOperation(4);
    block.RangeOperationInProgress(4);
    block.RangeOperationInProgress(3);
    block.RangeOperationInProgress(2);
    block.EndRangeOperation(2);

    QCOMPARE(block.GetSoldTicketIds().size(), 3U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 2);
    QCOMPARE(*(++it), 3);
    QCOMPARE(*(++it), 4);
}

void TicketsBlock_Test::RangeOperationInProgress_WontChangeTicketsBeyondEnd_EvenIfTheyGotTouchedDuringDragging()
{
    TicketsBlock block(5);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.RangeOperationInProgress(3);
    block.RangeOperationInProgress(4);
    block.RangeOperationInProgress(5); // act as if ticket 5 was touched by mistake only; the user doesn't wish to select it
    block.RangeOperationInProgress(4);
    block.EndRangeOperation(4);

    QCOMPARE(block.GetSoldTicketIds().size(), 3U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 2);
    QCOMPARE(*(++it), 3);
    QCOMPARE(*(++it), 4);
}

void TicketsBlock_Test::RangeOperationInProgress_WillSetUntouchedTickets_WhenTouchingTicketsFromDifferentRows()
{
    TicketsBlock block(5);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.RangeOperationInProgress(5); // jump to an imaginary, different "row" of tickets
    block.EndRangeOperation(5);

    QCOMPARE(block.GetSoldTicketIds().size(), 4U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 2);
    QCOMPARE(*(++it), 3);
    QCOMPARE(*(++it), 4);
    QCOMPARE(*(++it), 5);
}

void TicketsBlock_Test::RangeOperationInProgress_WillMarkTicketsAsUnsold_WhenStartingFromSoldTicket()
{
    TicketsBlock block(5);
    block.SetTicketSold(2, true);
    block.SetTicketSold(3, true);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.RangeOperationInProgress(3);
    block.RangeOperationInProgress(4);
    block.EndRangeOperation(4);

    QCOMPARE(block.GetSoldTicketIds().size(), 0U);
}

void TicketsBlock_Test::RangeOperationInProgress_WillRestorePreviousTicketsState_WhenTicketTouchedButFinallyNotIncludedInRangeOperation()
{
    TicketsBlock block(5);
    block.SetTicketSold(4, true);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.RangeOperationInProgress(3);
    block.RangeOperationInProgress(4);
    block.RangeOperationInProgress(3);
    block.RangeOperationInProgress(2);
    block.EndRangeOperation(2);

    QCOMPARE(block.GetSoldTicketIds().size(), 2U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 2);
    QCOMPARE(*(++it), 4);
}

void TicketsBlock_Test::RangeOperationInProgress_WillIgnoreAnyRequests_WhenHigherThanRangeTicketsSpecified()
{
    TicketsBlock block(5);

    block.BeginRangeOperation(5);
    block.RangeOperationInProgress(5);
    block.RangeOperationInProgress(6);
    block.RangeOperationInProgress(7);
    block.EndRangeOperation(5);

    QCOMPARE(block.GetSoldTicketIds().size(), 1U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 5);
}

void TicketsBlock_Test::RangeOperationInProgress_WillIgnoreAnyRequests_WhenLowerThanRangeTicketsSpecified()
{
    TicketsBlock block(5);

    block.BeginRangeOperation(2);
    block.RangeOperationInProgress(2);
    block.RangeOperationInProgress(1);
    block.RangeOperationInProgress(0);
    block.RangeOperationInProgress(-1);
    block.EndRangeOperation(-1);

    QCOMPARE(block.GetSoldTicketIds().size(), 2U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 1);
    QCOMPARE(*(++it), 2);
}

void TicketsBlock_Test::RangeOperationInProgress_WillIgnoreAnyRequests_WhenBeginRangeOperationWasNotAtValidTicketId()
{
    TicketsBlock block(100);

    block.BeginRangeOperation(105);
    block.RangeOperationInProgress(105);
    block.RangeOperationInProgress(100);
    block.RangeOperationInProgress(99);
    block.EndRangeOperation(99);

    QCOMPARE(block.GetSoldTicketIds().size(), 0U);
}

void TicketsBlock_Test::RangeOperationInProgress_WillReverseAllTicketsOutsideTheRange_EvenIfRevertingWholeRowsIsNeeded()
{
    TicketsBlock block(25);

    // a bug in range selection was fixed, so make sure it can't return again
    block.BeginRangeOperation(20);
    block.RangeOperationInProgress(20);
    block.RangeOperationInProgress(21);
    block.RangeOperationInProgress(6);
    block.RangeOperationInProgress(21);
    block.EndRangeOperation(21);

    QCOMPARE(block.GetSoldTicketIds().size(), 2U);
    auto it = block.GetSoldTicketIds().begin();
    QCOMPARE(*it, 20);
    QCOMPARE(*(++it), 21);
}

#include "TicketsBlock_Tests.moc"

DECLARE_TEST(TicketsBlock_Test)
