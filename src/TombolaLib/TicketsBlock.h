#ifndef TICKETSBLOCK_H
#define TICKETSBLOCK_H

#include "tombolalib_global.h"

#include <QString>
#include <set>
#include <map>

class TOMBOLALIBSHARED_EXPORT TicketsBlock
{
    static const int NotValidTicketId = -1;

    std::set<int> m_SoldTickets;
    int m_TicketsInBlock = 0;

    std::set<int> m_RangeOperationVictimTickets;
    std::map<int, bool> m_RangeOperationVictimsOriginalState;
    int m_RangeOperationBeginTicket = NotValidTicketId;
    bool m_SoldStateToApplyForRangeOperationVictims = true;

    bool isValidTicketId(int ticketId) const;

public:
    TicketsBlock(int ticketsPerBlock);

    QString Name;
    int ColorsSetIndex = 5; // default to "orange"
    const std::set<int>& GetSoldTicketIds() const;
    void ToggleSoldTicket(int ticketId);
    void SetTicketSold(int ticketId, bool isSold);
    bool IsTicketSold(int ticketId) const;

    void BeginRangeOperation(int startingTicketId);
    void RangeOperationInProgress(int ticketId);
    void EndRangeOperation(int endTicketId);
};

#endif // TICKETSBLOCK_H
