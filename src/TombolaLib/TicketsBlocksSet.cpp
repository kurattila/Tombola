
#include "TicketsBlock.h"
#include "TicketsBlockFactory.h"
#include "TicketsBlocksSet.h"
#include <stdexcept>

TicketsBlocksSet::TicketsBlocksSet()
{

}

std::shared_ptr<TicketsBlock> TicketsBlocksSet::GetFirstBlock() const
{
    if (m_AllBlocks.empty())
        return nullptr;

    return m_AllBlocks.front();
}

std::shared_ptr<TicketsBlock> TicketsBlocksSet::GetPrevBlock(std::shared_ptr<TicketsBlock> block) const
{
    std::shared_ptr<TicketsBlock> prevBlock;

    auto itFound = std::find_if(m_AllBlocks.begin(), m_AllBlocks.end(), [block](const std::shared_ptr<TicketsBlock>& other){return block.get() == other.get();});
    if (   itFound != m_AllBlocks.end()
        && itFound != m_AllBlocks.begin())
    {
        prevBlock = *std::prev(itFound, 1);
    }

    return prevBlock;
}

std::shared_ptr<TicketsBlock> TicketsBlocksSet::GetNextBlock(std::shared_ptr<TicketsBlock> block) const
{
    std::shared_ptr<TicketsBlock> nextBlock;

    if (!m_AllBlocks.empty())
    {
        auto itFound = std::find_if(m_AllBlocks.begin(), m_AllBlocks.end(), [block](const std::shared_ptr<TicketsBlock>& other){return block.get() == other.get();});
        if (itFound != m_AllBlocks.end() && std::distance(itFound, m_AllBlocks.end()) >= 2)
            nextBlock = *std::next(itFound, 1);
    }

    return nextBlock;
}

std::shared_ptr<TicketsBlock> TicketsBlocksSet::AddBlock()
{
    auto newBlockRaw = m_TicketsBlockFactory.CreateSingleBlock( TicketsBlockFactory::DefaultTicketsPerBlock );

    std::shared_ptr<TicketsBlock> newBlock(newBlockRaw);
    m_AllBlocks.push_back(newBlock);

    return m_AllBlocks.back();
}

std::shared_ptr<TicketsBlock> TicketsBlocksSet::DeleteBlock(std::shared_ptr<TicketsBlock> blockToDelete)
{
    std::shared_ptr<TicketsBlock> nearestBlock;

    auto itFound = std::find_if(m_AllBlocks.begin(), m_AllBlocks.end(), [blockToDelete](const std::shared_ptr<TicketsBlock>& other){return blockToDelete.get() == other.get();});
    if (itFound != m_AllBlocks.end())
    {
        int deletedIndex = std::distance(m_AllBlocks.begin(), itFound);
        m_AllBlocks.erase(itFound);

        if (!m_AllBlocks.empty())
        {
            int nearestBlockIndex = std::min(deletedIndex, static_cast<int>(m_AllBlocks.size()) - 1);
            nearestBlock = *std::next(m_AllBlocks.begin(), nearestBlockIndex);
        }
    }

    return nearestBlock;
}

int TicketsBlocksSet::GetBlocksCount() const
{
    return static_cast<int>( m_AllBlocks.size() );
}

int TicketsBlocksSet::FindBlockIndex(std::shared_ptr<TicketsBlock> block) const
{
    int blockIndex = 0;
    auto itFound = std::find_if(m_AllBlocks.begin(), m_AllBlocks.end(), [block](const std::shared_ptr<TicketsBlock>& other){return block.get() == other.get();});
    if (itFound != m_AllBlocks.end())
        blockIndex = std::distance(m_AllBlocks.begin(), itFound);
    return blockIndex;
}

std::shared_ptr<TicketsBlock> TicketsBlocksSet::GetBlock(int blockIndex) const
{
    return m_AllBlocks[blockIndex];
}
