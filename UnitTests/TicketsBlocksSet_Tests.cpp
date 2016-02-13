#include "AutoTest.h"
#include "tombolalib_global.h"

#include "TicketsBlock.h"
#include "TicketsBlocksSet.h"
#include <stdexcept>

class TicketsBlocksSet_Test : public QObject
{
    Q_OBJECT
public:
    TicketsBlocksSet_Test()
    {
    }

private slots:
    void GetFirstBlock_ReturnsNullptr_WhenNoBlocksExistYet();
    void GetFirstBlock_WillReturnFirstBlock_WhenSingleBlocksExistOnly();

    void GetPrevBlock_WillReturnNullptr_WhenNoBlocksExistYet();
    void GetPrevBlock_WillReturnNullptr_ForFirstExistingBlock();
    void GetPrevBlock_WillReturnSecondBlock_ForThirdExistingBlock();

    void GetNextBlock_WillReturnNullptr_WhenNoBlocksExistYet();
    void GetNextBlock_WillReturnNullptr_ForFirstExistingBlock();
    void GetNextBlock_WillReturnThirdBlock_ForSecondExistingBlock();

    void DeleteBlock_WontThrow_WhenNoBlockSpecified();
    void DeleteBlock_DeletesTheOnlyBlock_WhenOneBlockExistsOnly();
    void DeleteBlock_DeletesTheSpecifiedBlockOnly_WhenSeveralBlocksExist();
    void DeleteBlock_ReturnsNullptr_WhenTheOnlyBlockDeleted();
    void DeleteBlock_ReturnsFirstBlock_WhenTheSecondOfTwoBlocksDeleted();
    void DeleteBlock_ReturnsThirdBlock_WhenTheSecondOfThreeBlocksDeleted();

    void FindBlockIndex_WillReturnZero_WhenBlocksDoesNotExist();
    void FindBlockIndex_WillReturnTwo_ForThirdBlockWhenFourBlocksExist();
};




void TicketsBlocksSet_Test::GetFirstBlock_ReturnsNullptr_WhenNoBlocksExistYet()
{
    TicketsBlocksSet set;

    QVERIFY(set.GetFirstBlock() == nullptr);
}

void TicketsBlocksSet_Test::GetFirstBlock_WillReturnFirstBlock_WhenSingleBlocksExistOnly()
{
    TicketsBlocksSet set;

    auto newBlock = set.AddBlock();

    QCOMPARE(set.GetFirstBlock(), newBlock);
}

void TicketsBlocksSet_Test::GetPrevBlock_WillReturnNullptr_WhenNoBlocksExistYet()
{
    TicketsBlocksSet set;

    auto prev = set.GetPrevBlock(nullptr);

    QVERIFY(prev == nullptr);
}

void TicketsBlocksSet_Test::GetPrevBlock_WillReturnNullptr_ForFirstExistingBlock()
{
    TicketsBlocksSet set;
    auto firstBlock = set.AddBlock();

    auto prev = set.GetPrevBlock(firstBlock);

    QVERIFY(prev == nullptr);
}

void TicketsBlocksSet_Test::GetPrevBlock_WillReturnSecondBlock_ForThirdExistingBlock()
{
    TicketsBlocksSet set;
    auto firstBlock = set.AddBlock(); Q_UNUSED(firstBlock);
    auto secondBlock = set.AddBlock();
    auto thirdBlock = set.AddBlock();

    auto prev = set.GetPrevBlock(thirdBlock);

    QCOMPARE(secondBlock, prev);
}

void TicketsBlocksSet_Test::GetNextBlock_WillReturnNullptr_WhenNoBlocksExistYet()
{
    TicketsBlocksSet set;

    auto next = set.GetNextBlock(nullptr);

    QVERIFY(next == nullptr);
}

void TicketsBlocksSet_Test::GetNextBlock_WillReturnNullptr_ForFirstExistingBlock()
{
    TicketsBlocksSet set;
    auto firstBlock = set.AddBlock();

    auto next = set.GetNextBlock(firstBlock);

    QVERIFY(next == nullptr);
}

void TicketsBlocksSet_Test::GetNextBlock_WillReturnThirdBlock_ForSecondExistingBlock()
{
    TicketsBlocksSet set;
    auto firstBlock = set.AddBlock(); Q_UNUSED(firstBlock);
    auto secondBlock = set.AddBlock();
    auto thirdBlock = set.AddBlock();

    auto next = set.GetNextBlock(secondBlock);

    QCOMPARE(thirdBlock, next);
}

void TicketsBlocksSet_Test::DeleteBlock_WontThrow_WhenNoBlockSpecified()
{
    TicketsBlocksSet set;

    set.DeleteBlock(nullptr);

    // Implicit Assert: will pass if 'DeleteBlock()' above doesn't throw
}

void TicketsBlocksSet_Test::DeleteBlock_DeletesTheOnlyBlock_WhenOneBlockExistsOnly()
{
    TicketsBlocksSet set;
    auto theOnlyBlock = set.AddBlock();

    set.DeleteBlock(theOnlyBlock);

    QCOMPARE(set.GetBlocksCount(), 0);
}

void TicketsBlocksSet_Test::DeleteBlock_DeletesTheSpecifiedBlockOnly_WhenSeveralBlocksExist()
{
    TicketsBlocksSet set;
    auto first = set.AddBlock();
    auto second = set.AddBlock();
    auto third = set.AddBlock();

    set.DeleteBlock(first);

    QCOMPARE(set.GetFirstBlock(), second);
    QCOMPARE(set.GetNextBlock(second), third);
    QVERIFY(set.GetNextBlock(third) == nullptr);
}

void TicketsBlocksSet_Test::DeleteBlock_ReturnsNullptr_WhenTheOnlyBlockDeleted()
{
    TicketsBlocksSet set;
    auto theOnlyBlock = set.AddBlock();

    auto nearestBlock = set.DeleteBlock(theOnlyBlock);

    QVERIFY(nearestBlock == nullptr);
}

void TicketsBlocksSet_Test::DeleteBlock_ReturnsFirstBlock_WhenTheSecondOfTwoBlocksDeleted()
{
    TicketsBlocksSet set;
    auto first = set.AddBlock();
    auto second = set.AddBlock();

    auto nearestBlock = set.DeleteBlock(second);

    QVERIFY(nearestBlock == first);
}

void TicketsBlocksSet_Test::DeleteBlock_ReturnsThirdBlock_WhenTheSecondOfThreeBlocksDeleted()
{
    TicketsBlocksSet set;
    auto first = set.AddBlock(); Q_UNUSED(first);
    auto second = set.AddBlock();
    auto third = set.AddBlock();

    auto nearestBlock = set.DeleteBlock(second);

    QVERIFY(nearestBlock == third);
}

void TicketsBlocksSet_Test::FindBlockIndex_WillReturnZero_WhenBlocksDoesNotExist()
{
    TicketsBlocksSet set;
    auto first = set.AddBlock();
    set.DeleteBlock(first);

    int blockIndex = set.FindBlockIndex(first);

    QCOMPARE(blockIndex, 0);
}

void TicketsBlocksSet_Test::FindBlockIndex_WillReturnTwo_ForThirdBlockWhenFourBlocksExist()
{
    TicketsBlocksSet set;
    auto first = set.AddBlock(); Q_UNUSED(first);
    auto second = set.AddBlock(); Q_UNUSED(second);
    auto third = set.AddBlock();
    auto fourth = set.AddBlock(); Q_UNUSED(fourth);

    int blockIndex = set.FindBlockIndex(third);

    QCOMPARE(blockIndex, 2);
}

#include "TicketsBlocksSet_Tests.moc"

DECLARE_TEST(TicketsBlocksSet_Test)


