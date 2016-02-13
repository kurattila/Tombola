#include "TicketsBlockFactory.h"
#include <QString>
#include <QStringList>
#include "TicketsBlock.h"

int TicketsBlockFactory::DefaultTicketsPerBlock = 100;

TicketsBlockFactory::TicketsBlockFactory()
{

}

std::list<TicketsBlock> TicketsBlockFactory::CreateBlocks(const QString& blockNamesPackedInRows, int ticketsPerBlock) const
{
    std::list<TicketsBlock> blocks;

    auto rows = blockNamesPackedInRows.split(QString("\n"));
    for (auto row: rows)
    {
        row.replace(" ", "");
        row.replace("\t", "");
        if (row.size() > 0 && ticketsPerBlock > 0)
        {
            TicketsBlock newBlock(ticketsPerBlock);
            newBlock.Name = row;
            blocks.push_back(std::move(newBlock));
        }
    }

    return blocks;
}

TicketsBlock* TicketsBlockFactory::CreateSingleBlock(int ticketsPerBlock) const
{
    auto newBlock = new TicketsBlock(ticketsPerBlock);
    m_SingleBlocksCreated++;

    if (m_SingleBlocksCreated == 1)
        newBlock->Name = "Névtelen tömb";
    else
        newBlock->Name = QString("Névtelen tömb %1").arg(m_SingleBlocksCreated);

    return newBlock;
}
