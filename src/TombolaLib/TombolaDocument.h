#ifndef TOMBOLADOCUMENT_H
#define TOMBOLADOCUMENT_H

#include "tombolalib_global.h"

#include "TicketsBlocksSet.h"
#include <memory>

class TOMBOLALIBSHARED_EXPORT TombolaDocument
{
    static const int DefaultPrizesCount = 100;
public:
    TombolaDocument();

    int PrizesCount = DefaultPrizesCount;
    std::shared_ptr<TicketsBlocksSet> AllTicketsBlocksSet;
};

#endif // TOMBOLADOCUMENT_H
