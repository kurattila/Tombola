#include "TicketDrawExecutor.h"
#include "TicketFactory.h"
#include "TombolaDocument.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

TicketDrawExecutor::TicketDrawExecutor(  TombolaDocument& document
                                       , SingleTicketDraw_ViewModel* ticketDrawLeft
                                       , SingleTicketDraw_ViewModel* ticketDrawRight
                                       , QObject *parent)
    : QObject(parent)
    , m_Document(document)
{
    m_TicketDrawLeft.reset(ticketDrawLeft);
    m_TicketDrawRight.reset(ticketDrawRight);

    connect(  m_TicketDrawLeft.get(), SIGNAL(ticketWinningPositionRequested(const std::shared_ptr<Ticket>&))
            , this, SLOT(onTicketWinningPositionRequested(const std::shared_ptr<Ticket>&)));
    connect(  m_TicketDrawRight.get(), SIGNAL(ticketWinningPositionRequested(const std::shared_ptr<Ticket>&))
            , this, SLOT(onTicketWinningPositionRequested(const std::shared_ptr<Ticket>&)));
}

void TicketDrawExecutor::onPrizeDrawingStartUp()
{
    if (m_PrizeDrawingRunning == false)
    {
        std::list<std::shared_ptr<Ticket>> inGameTickets;
        TicketFactory ticketFactory;

        auto block = m_Document.AllTicketsBlocksSet->GetFirstBlock();
        while (block)
        {
            auto ticketsOfBlock = ticketFactory.CreateTicketsFromBlock(*block);
            inGameTickets.insert(inGameTickets.end(), ticketsOfBlock.begin(), ticketsOfBlock.end());
            block = m_Document.AllTicketsBlocksSet->GetNextBlock(block);
        }
        m_InGameTicketsRepository.Reset(inGameTickets);

        m_PrizeDrawingRunning = true;
        m_CurrentlySpinningCount = 0;
        setRemainingPrizesCount(m_Document.PrizesCount); // do _not_ initialize in ctor; it might have changed since the ctor
    }

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

void TicketDrawExecutor::onPrizeDrawingAborted()
{
    m_PrizeDrawingRunning = false;
}

bool TicketDrawExecutor::IsPrizeDrawingRunning() const
{
    return m_PrizeDrawingRunning;
}

IMemento *TicketDrawExecutor::SaveToMemento()
{
    TicketDrawExecutorMemento* memento = new TicketDrawExecutorMemento();
    memento->PrizeDrawingRunning = m_PrizeDrawingRunning;
    memento->CurrentlySpinningCount = m_CurrentlySpinningCount;
    memento->RemainingPrizesCount = remainingPrizesCount();
    memento->InGameTicketsRepoMemento.reset(m_InGameTicketsRepository.SaveToMemento());
    return memento;
}

void TicketDrawExecutor::RestoreFromMemento(const IMemento* memento, void* /*context*/)
{
    const TicketDrawExecutorMemento* mem = dynamic_cast<const TicketDrawExecutorMemento*>(memento);
    if (!mem)
        return;

    m_PrizeDrawingRunning = mem->PrizeDrawingRunning;
    if (m_PrizeDrawingRunning)
    {
        m_CurrentlySpinningCount = mem->CurrentlySpinningCount;
        m_RemainingPrizesCount = mem->RemainingPrizesCount;
        void* context = m_Document.AllTicketsBlocksSet.get();
        m_InGameTicketsRepository.RestoreFromMemento(mem->InGameTicketsRepoMemento.get(), context);
    }
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
    if (m_RemainingPrizesCount != remaining)
    {
        int coercedRemaining = std::min(remaining, maxAllowedRemainingPrizesCount());
        m_RemainingPrizesCount = coercedRemaining;
        emit remainingPrizesCountChanged();
    }
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

int TicketDrawExecutor::maxAllowedRemainingPrizesCount() const
{
    if (!m_InGameTicketsRepository.IsValid()) // not initialized yet: unable to tell any maximum
    {
        return 1000;
    }
    else
    {
        int untouchedCount = m_InGameTicketsRepository.GetUntouchedTickets().size();
        return untouchedCount + m_CurrentlySpinningCount;
    }
}

void TicketDrawExecutor::onTicketWinningPositionRequested(const std::shared_ptr<Ticket>& /*ticket*/)
{
    setCurrentlySpinningCount(m_CurrentlySpinningCount - 1);
    setRemainingPrizesCount(m_RemainingPrizesCount - 1);
//    m_InGameTicketsRepository.OnTicketDrawnCommit(ticket);
}



QString TicketDrawExecutorMemento::StartElementXmlName = "PrizeDrawing";

TicketDrawExecutorMemento::TicketDrawExecutorMemento()
{

}

TicketDrawExecutorMemento::~TicketDrawExecutorMemento()
{

}

void TicketDrawExecutorMemento::Read(QXmlStreamReader& xmlReader)
{
    if (xmlReader.name() != TicketDrawExecutorMemento::StartElementXmlName)
        throw std::invalid_argument("Awaiting PrizeDrawing tag in XML");

    if (xmlReader.attributes().hasAttribute("Running"))
        PrizeDrawingRunning = xmlReader.attributes().value("Running").toInt();
    if (xmlReader.attributes().hasAttribute("RemainingPrizesCount"))
        RemainingPrizesCount = xmlReader.attributes().value("RemainingPrizesCount").toInt();

    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        auto tokenType = xmlReader.readNext();
        if (tokenType == QXmlStreamReader::StartElement && xmlReader.name() == InGameTicketsRepositoryMemento::StartElementXmlName)
        {
            InGameTicketsRepoMemento.reset(new InGameTicketsRepositoryMemento());
            InGameTicketsRepoMemento->Read(xmlReader);
        }
        else if (tokenType == QXmlStreamReader::EndElement && xmlReader.name() == TicketDrawExecutorMemento::StartElementXmlName)
        {
            break;
        }
    }
}

void TicketDrawExecutorMemento::Write(QXmlStreamWriter& xmlWriter)
{
    xmlWriter.writeStartElement(TicketDrawExecutorMemento::StartElementXmlName);
    xmlWriter.writeAttribute("Running", QString::number(PrizeDrawingRunning));
    xmlWriter.writeAttribute("RemainingPrizesCount", QString::number(RemainingPrizesCount));

    InGameTicketsRepoMemento->Write(xmlWriter);

    xmlWriter.writeEndElement();
}
