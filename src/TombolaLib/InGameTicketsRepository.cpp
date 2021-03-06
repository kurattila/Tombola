#include "InGameTicketsRepository.h"
#include "TicketsBlocksSet.h"
#include "TicketFactory.h"

#include <QStringBuilder>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

InGameTicketsRepository::InGameTicketsRepository()
{

}

void InGameTicketsRepository::Reset(const std::list<std::shared_ptr<Ticket> > &untouchedTickets)
{
    m_UntouchedTickets = untouchedTickets;
    m_WinningTickets.clear();
}

void InGameTicketsRepository::OnTicketDrawnPrepare(const std::shared_ptr<Ticket>& winningTicket)
{
    if (m_UntouchedTickets.empty())
        throw std::invalid_argument("Tickets list must not be empty");

    m_UntouchedTickets.remove(winningTicket);
    m_TicketsBeingTransformedIntoWinningOnes.push_back(winningTicket);
}

void InGameTicketsRepository::OnTicketDrawnCommit(const std::shared_ptr<Ticket>& winningTicket)
{
    m_TicketsBeingTransformedIntoWinningOnes.remove(winningTicket);
    m_WinningTickets.push_front(winningTicket);
}

const std::list<std::shared_ptr<Ticket> > &InGameTicketsRepository::GetUntouchedTickets() const
{
    return m_UntouchedTickets;
}

std::list<std::shared_ptr<Ticket> > InGameTicketsRepository::GetTicketsStillInGame() const
{
    // "still in game" = "untouched" + "being transformed into winning ones"
    std::list<std::shared_ptr<Ticket>> ticketsStillInGame(m_UntouchedTickets);
    ticketsStillInGame.insert(ticketsStillInGame.end(), m_TicketsBeingTransformedIntoWinningOnes.begin(), m_TicketsBeingTransformedIntoWinningOnes.end());
    return ticketsStillInGame;
}

const std::list<std::shared_ptr<Ticket> > &InGameTicketsRepository::GetWinningTicketsHistory() const
{
    return m_WinningTickets;
}

bool InGameTicketsRepository::IsValid() const
{
    return     !m_UntouchedTickets.empty()
            || !m_TicketsBeingTransformedIntoWinningOnes.empty()
            || !m_WinningTickets.empty();
}

IMemento *InGameTicketsRepository::SaveToMemento()
{
    auto mem = new InGameTicketsRepositoryMemento();
    for (const auto& ticket: GetTicketsStillInGame())
    {
        TicketsBlocksSet::TicketsBlockIdentification blockId(ticket->BlockName(), ticket->ColorsSetIndex());
        if (mem->m_MapOfStillInGameTickets.find(blockId) == mem->m_MapOfStillInGameTickets.end())
            mem->m_MapOfStillInGameTickets[blockId] = std::set<int>();
        mem->m_MapOfStillInGameTickets[blockId].insert(ticket->TicketNumber());
    }

    // Even if m_MapOfStillInGameTickets has received no elements, put at least empty elements in it,
    // as m_MapOfStillInGameTickets will also serve for reconstructing both collections at once: m_WinningTickets and m_UntouchedTickets
    for (const auto& ticket: GetWinningTicketsHistory())
    {
        TicketsBlocksSet::TicketsBlockIdentification blockId(ticket->BlockName(), ticket->ColorsSetIndex());
        if (mem->m_MapOfStillInGameTickets.find(blockId) == mem->m_MapOfStillInGameTickets.end())
            mem->m_MapOfStillInGameTickets[blockId] = std::set<int>();
    }

    return mem;
}

void InGameTicketsRepository::RestoreFromMemento(const IMemento* memento, void* context)
{
    const InGameTicketsRepositoryMemento* mem = dynamic_cast<const InGameTicketsRepositoryMemento*>(memento);
    if (!mem)
        return;

    TicketsBlocksSet* blocksSet = reinterpret_cast<TicketsBlocksSet*>(context);
    if (!blocksSet)
        return;

    std::list<std::shared_ptr<TicketsBlock>> allBlocks;
    auto block = blocksSet->GetFirstBlock();
    while (block.get())
    {
        allBlocks.push_back(block);
        block = blocksSet->GetNextBlock(block);
    }

    m_UntouchedTickets.clear();
    m_WinningTickets.clear();
    for (const auto& mapPair: mem->m_MapOfStillInGameTickets)
    {
        const TicketsBlocksSet::TicketsBlockIdentification& id = mapPair.first;
        const std::set<int>& stillInGameTicketsOfBlock = mapPair.second;

        auto block = blocksSet->FindBlock(id);
        for (int ticketNumber = 1; ticketNumber <= block->GetTicketsInBlock(); ++ticketNumber)
        {
            auto newTicket = std::make_shared<Ticket>(ticketNumber, *block);

            auto itStillInGameTicketFound = stillInGameTicketsOfBlock.find(ticketNumber);
            if (itStillInGameTicketFound == stillInGameTicketsOfBlock.end())
                m_WinningTickets.push_back(newTicket);
            else
                m_UntouchedTickets.push_back(newTicket);
        }
    }
}

QString InGameTicketsRepositoryMemento::StartElementXmlName = "WinningHistory";

InGameTicketsRepositoryMemento::InGameTicketsRepositoryMemento()
{

}

InGameTicketsRepositoryMemento::~InGameTicketsRepositoryMemento()
{

}

void InGameTicketsRepositoryMemento::Read(QXmlStreamReader& xmlReader)
{
    if (xmlReader.name() != InGameTicketsRepositoryMemento::StartElementXmlName)
        throw std::invalid_argument("Awaiting WinningHistory tag in XML");

    m_MapOfStillInGameTickets.clear();
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        auto tokenType = xmlReader.readNext();
        if (tokenType == QXmlStreamReader::StartElement && xmlReader.name() == "TicketsBlockSnapshot")
        {
            QString blockName = xmlReader.attributes().value("Name").toString();
            int colorsSetIndex = xmlReader.attributes().value("ColorsSetIndex").toInt();
            TicketsBlocksSet::TicketsBlockIdentification id = { blockName, colorsSetIndex };

            std::set<int> stillInGameTicketsOfBlock;
            QString ticketTokens = xmlReader.attributes().value("StillInGameTickets").toString();
            auto ticketIds = ticketTokens.split(";", QString::SkipEmptyParts);
            for (auto it = ticketIds.constBegin(); it != ticketIds.constEnd(); ++it)
                stillInGameTicketsOfBlock.insert(it->toInt());

            m_MapOfStillInGameTickets[id] = stillInGameTicketsOfBlock;
        }
        else if (tokenType == QXmlStreamReader::EndElement && xmlReader.name() == InGameTicketsRepositoryMemento::StartElementXmlName)
        {
            break;
        }
    }
}

void InGameTicketsRepositoryMemento::Write(QXmlStreamWriter& xmlWriter)
{
    xmlWriter.writeStartElement(InGameTicketsRepositoryMemento::StartElementXmlName);

    for (const auto& mapPair: m_MapOfStillInGameTickets)
    {
        const TicketsBlocksSet::TicketsBlockIdentification& id = mapPair.first;
        const std::set<int>& stillInGameTicketsOfBlock = mapPair.second;

        xmlWriter.writeStartElement("TicketsBlockSnapshot");
        xmlWriter.writeAttribute("Name", id.first);
        xmlWriter.writeAttribute("ColorsSetIndex", QString::number(id.second));

//        QString winningTickets;
//        for (int ticketNumber: winningTicketsOfBlock)
//            winningTickets = winningTickets % QString::number(ticketNumber) % ";";
//        xmlWriter.writeAttribute("WinningTickets", winningTickets);

        QString stillInGameTickets;
        for (int ticketNumber: stillInGameTicketsOfBlock)
            stillInGameTickets = stillInGameTickets % QString::number(ticketNumber) % ";";
        xmlWriter.writeAttribute("StillInGameTickets", stillInGameTickets);

        xmlWriter.writeEndElement(); // <TicketsBlockSnapshot />
    }

    xmlWriter.writeEndElement(); // end of InGameTicketsRepositoryMemento::StartElementXmlName tag
}
