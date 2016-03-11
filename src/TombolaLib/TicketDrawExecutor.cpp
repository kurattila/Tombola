#include "TicketDrawExecutor.h"
#include "TicketFactory.h"
#include "TombolaDocument.h"

TicketDrawExecutor::TicketDrawExecutor(  TombolaDocument& document
                                       , SingleTicketDraw_ViewModel* ticketDrawLeft
                                       , SingleTicketDraw_ViewModel* ticketDrawRight
                                       , QObject *parent)
    : QObject(parent)
    , m_Document(document)
{
    m_TicketDrawLeft.reset(ticketDrawLeft);
    m_TicketDrawRight.reset(ticketDrawRight);

    connect(m_TicketDrawLeft.get(), SIGNAL(ticketWinningPositionRequested()), this, SLOT(onTicketWinningPositionRequested()));
    connect(m_TicketDrawRight.get(), SIGNAL(ticketWinningPositionRequested()), this, SLOT(onTicketWinningPositionRequested()));
}

void TicketDrawExecutor::onPrizeDrawingStartUp()
{
    setRemainingPrizesCount(m_Document.PrizesCount); // do _not_ initialize in ctor; it might have changed since the ctor
    m_CurrentlySpinningCount = 0;

    std::list<std::shared_ptr<Ticket>> inGameTickets;
    TicketFactory ticketFactory;

    auto block = m_Document.AllTicketsBlocksSet->GetFirstBlock();
    while (block)
    {
        auto ticketsOfBlock = ticketFactory.CreateTicketsFromBlock(*block);
        inGameTickets.insert(inGameTickets.end(), ticketsOfBlock.begin(), ticketsOfBlock.end());
        block = m_Document.AllTicketsBlocksSet->GetNextBlock(block);
    }
    m_InGameTicketsRepository.Init(inGameTickets);


    m_TicketDrawLeft->Init(&m_InGameTicketsRepository);
    m_TicketDrawRight->Init(&m_InGameTicketsRepository);
}

void TicketDrawExecutor::onTriggerByUser()
{
    if (m_RemainingPrizesCount - m_CurrentlySpinningCount <= 0)
        return;

    SingleTicketDraw_ViewModel::ResultOfUserTrigger leftRightResult = m_TicketDrawLeft->onTriggerByUser();
    if (leftRightResult == SingleTicketDraw_ViewModel::ResultOfUserTrigger::Rejected)
        leftRightResult = m_TicketDrawRight->onTriggerByUser();

    if (leftRightResult == SingleTicketDraw_ViewModel::ResultOfUserTrigger::Accepted)
        setCurrentlySpinningCount(m_CurrentlySpinningCount + 1);
}

SingleTicketDraw_ViewModel *TicketDrawExecutor::ticketDrawLeft()
{
    return m_TicketDrawLeft.get();
}

SingleTicketDraw_ViewModel *TicketDrawExecutor::ticketDrawRight()
{
    return m_TicketDrawRight.get();
}

void TicketDrawExecutor::setRemainingPrizesCount(int remaining)
{
    m_RemainingPrizesCount = remaining;
    emit remainingPrizesCountChanged();
}

void TicketDrawExecutor::setCurrentlySpinningCount(int ticketsSpinningNow)
{
    m_CurrentlySpinningCount = ticketsSpinningNow;
    emit minAllowedRemainingPrizesCountChanged();
}

int TicketDrawExecutor::remainingPrizesCount() const
{
    return m_RemainingPrizesCount;
}

int TicketDrawExecutor::minAllowedRemainingPrizesCount() const
{
    return m_CurrentlySpinningCount;
}

void TicketDrawExecutor::onTicketWinningPositionRequested()
{
    setCurrentlySpinningCount(m_CurrentlySpinningCount - 1);
    setRemainingPrizesCount(m_RemainingPrizesCount - 1);
}
