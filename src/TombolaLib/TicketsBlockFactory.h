#ifndef TICKETSBLOCKFACTORY_H
#define TICKETSBLOCKFACTORY_H

#include "tombolalib_global.h"
#include <list>

class QString;
class TicketsBlock;

class TOMBOLALIBSHARED_EXPORT TicketsBlockFactory
{
    mutable int m_SingleBlocksCreated = 0;
public:
    TicketsBlockFactory();

    static int DefaultTicketsPerBlock;

    std::list<TicketsBlock> CreateBlocks(const QString& blockNamesPackedInRows, int ticketsPerBlock) const;
    TicketsBlock* CreateSingleBlock(int ticketsPerBlock) const;
};

#endif // TICKETSBLOCKFACTORY_H
