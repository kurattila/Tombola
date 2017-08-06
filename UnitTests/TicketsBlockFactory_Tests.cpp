#include "AutoTest.h"
#include "tombolalib_global.h"

#include "TicketsBlock.h"
#include "TicketsBlockFactory.h"
#include <sstream>
#include <memory>

class TicketsBlockFactory_Test : public QObject
{
    Q_OBJECT
public:
    TicketsBlockFactory_Test()
    {
    }

private slots:
    void CreateBlocks_WillReturnEmptyList_WhenZeroTicketsPerBlockRequested();
    void CreateBlocks_WillReturnEmptyList_WhenNoWordsSpecifiedAsBlockNames();
    void CreateBlocks_WillCreateOneBlock_WhenOneWordSpecifiedOnlyAsBlockName();
    void CreateBlocks_WillCreate3Blocks_When3RowsSpecifiedAsBlockNames();
    void CreateBlocks_WillCreateAllBlocksWithSameNumberOfTickets_When50TicketsPerBlockSpecified();
    void CreateBlocks_WillCreateBlocksWithBlockName_WhenRowsSpecifiedAsBlockNames();
    void CreateBlocks_WontCreateBlocksForEmptyNames_WhenAlsoEmptyRowsSpecifiedAsBlockNames();

    void CreateSingleBlock_WillCreateBlockWith20Tickets_When20TicketsSpecified();
    void CreateSingleBlock_WillCreateBlockCalledUntitled_WhenVeryFirstBlockCreated();
    void CreateSingleBlock_WillCreateBlockCalledUntitled_WhenSecondAndLaterBlocksCreated();
};



void TicketsBlockFactory_Test::CreateBlocks_WillReturnEmptyList_WhenZeroTicketsPerBlockRequested()
{
    TicketsBlockFactory blockFactory;

    auto blocks = blockFactory.CreateBlocks(QString::fromStdWString(L"KACSA"), 0);

    QCOMPARE(blocks.size(), 0UL);
}

void TicketsBlockFactory_Test::CreateBlocks_WillReturnEmptyList_WhenNoWordsSpecifiedAsBlockNames()
{
    TicketsBlockFactory blockFactory;

    auto blocks = blockFactory.CreateBlocks(QString::fromStdWString(L""), 10);

    QCOMPARE(blocks.size(), 0UL);
}

void TicketsBlockFactory_Test::CreateBlocks_WillCreateOneBlock_WhenOneWordSpecifiedOnlyAsBlockName()
{
    TicketsBlockFactory blockFactory;

    auto blocks = blockFactory.CreateBlocks(QString::fromStdWString(L"KACSA"), 10);

    QCOMPARE(blocks.size(), 1UL);
}

void TicketsBlockFactory_Test::CreateBlocks_WillCreate3Blocks_When3RowsSpecifiedAsBlockNames()
{
    TicketsBlockFactory blockFactory;
    std::wstringstream ss;
    ss << L"KACSA" << std::endl << L"KUTYA" << std::endl << L"DINO";
    QString blockNames = QString::fromStdWString(ss.str());

    auto blocks = blockFactory.CreateBlocks(blockNames, 10);

    QCOMPARE(blocks.size(), 3UL);
}

void TicketsBlockFactory_Test::CreateBlocks_WillCreateAllBlocksWithSameNumberOfTickets_When50TicketsPerBlockSpecified()
{
    TicketsBlockFactory blockFactory;
    std::wstringstream ss;
    ss << L"KACSA" << std::endl << L"KUTYA";
    QString blockNames = QString::fromStdWString(ss.str());
    int ticketsPerBlock = 50;

    auto blocks = blockFactory.CreateBlocks(blockNames, ticketsPerBlock);

    auto kacsaBlock = *blocks.begin();
    auto kutyaBlock = *std::next(blocks.begin(), 1);
    kacsaBlock.ToggleSoldTicket(ticketsPerBlock);
    kacsaBlock.ToggleSoldTicket(ticketsPerBlock+1);
    kutyaBlock.ToggleSoldTicket(ticketsPerBlock);
    kutyaBlock.ToggleSoldTicket(ticketsPerBlock+1);
    QCOMPARE(*kacsaBlock.GetSoldTicketIds().begin(), ticketsPerBlock);
    QCOMPARE(*kutyaBlock.GetSoldTicketIds().begin(), ticketsPerBlock);
}

void TicketsBlockFactory_Test::CreateBlocks_WillCreateBlocksWithBlockName_WhenRowsSpecifiedAsBlockNames()
{
    TicketsBlockFactory blockFactory;
    std::wstringstream ss;
    ss << L"KACSA" << std::endl << L"KUTYA" << std::endl << L"DINO";
    QString blockNames = QString::fromStdWString(ss.str());
    int ticketsPerBlock = 50;

    auto blocks = blockFactory.CreateBlocks(blockNames, ticketsPerBlock);

    auto kacsaBlock = *blocks.begin();
    auto kutyaBlock = *std::next(blocks.begin(), 1);
    auto dinoBlock = *std::next(blocks.begin(), 2);
    QCOMPARE(kacsaBlock.Name, QString("KACSA"));
    QCOMPARE(kutyaBlock.Name, QString("KUTYA"));
    QCOMPARE(dinoBlock.Name, QString("DINO"));
}

void TicketsBlockFactory_Test::CreateBlocks_WontCreateBlocksForEmptyNames_WhenAlsoEmptyRowsSpecifiedAsBlockNames()
{
    TicketsBlockFactory blockFactory;
    std::wstringstream ss;
    ss << L"KACSA" << std::endl << L"   " << std::endl << L"KUTYA" << std::endl << std::endl << std::endl << L"DINO";
    QString blockNames = QString::fromStdWString(ss.str());
    int ticketsPerBlock = 50;

    auto blocks = blockFactory.CreateBlocks(blockNames, ticketsPerBlock);

    QCOMPARE(blocks.size(), 3UL);
    auto kacsaBlock = *blocks.begin();
    auto kutyaBlock = *std::next(blocks.begin(), 1);
    auto dinoBlock = *std::next(blocks.begin(), 2);
    QCOMPARE(kacsaBlock.Name, QString("KACSA"));
    QCOMPARE(kutyaBlock.Name, QString("KUTYA"));
    QCOMPARE(dinoBlock.Name, QString("DINO"));
}

void TicketsBlockFactory_Test::CreateSingleBlock_WillCreateBlockWith20Tickets_When20TicketsSpecified()
{
    TicketsBlockFactory blockFactory;

    std::unique_ptr<TicketsBlock> block(blockFactory.CreateSingleBlock(20));

    QCOMPARE(block->GetSoldTicketIds().size(), 0UL);
}

void TicketsBlockFactory_Test::CreateSingleBlock_WillCreateBlockCalledUntitled_WhenVeryFirstBlockCreated()
{
    TicketsBlockFactory blockFactory;

    std::unique_ptr<TicketsBlock> block(blockFactory.CreateSingleBlock(50));

    QCOMPARE(block->Name, QString("Unnamed block"));
}

void TicketsBlockFactory_Test::CreateSingleBlock_WillCreateBlockCalledUntitled_WhenSecondAndLaterBlocksCreated()
{
    TicketsBlockFactory blockFactory;

    blockFactory.CreateSingleBlock(50);
    std::unique_ptr<TicketsBlock> block(blockFactory.CreateSingleBlock(50));

    QCOMPARE(block->Name, QString("Unnamed block 2"));
}

#include "TicketsBlockFactory_Tests.moc"

DECLARE_TEST(TicketsBlockFactory_Test)

