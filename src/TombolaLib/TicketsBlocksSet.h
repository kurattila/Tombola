#ifndef TICKETSBLOCKSSET_H
#define TICKETSBLOCKSSET_H

#include "tombolalib_global.h"
#include <vector>
#include <memory>
#include "TicketsBlockFactory.h"

class TicketsBlock;

class TOMBOLALIBSHARED_EXPORT TicketsBlocksSet
{
    std::vector<std::shared_ptr<TicketsBlock>> m_AllBlocks;
    TicketsBlockFactory m_TicketsBlockFactory;

public:
    using TicketsBlockIdentification = std::pair<QString, int>;

    TicketsBlocksSet();

    std::shared_ptr<TicketsBlock> GetFirstBlock() const;
    std::shared_ptr<TicketsBlock> GetPrevBlock(std::shared_ptr<TicketsBlock> block) const;
    std::shared_ptr<TicketsBlock> GetNextBlock(std::shared_ptr<TicketsBlock> block) const;

    std::shared_ptr<TicketsBlock> AddBlock(int ticketsPerBlock = TicketsBlockFactory::DefaultTicketsPerBlock);
    std::shared_ptr<TicketsBlock> DeleteBlock(std::shared_ptr<TicketsBlock> blockToDelete);

    int GetBlocksCount() const;
    int FindBlockIndex(std::shared_ptr<TicketsBlock> block) const;
    std::shared_ptr<TicketsBlock> GetBlock(int blockIndex) const;
    std::shared_ptr<TicketsBlock> FindBlock(const TicketsBlockIdentification& blockIdToFind) const;
};

#endif // TICKETSBLOCKSSET_H
