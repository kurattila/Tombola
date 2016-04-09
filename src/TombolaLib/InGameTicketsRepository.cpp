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
}

void InGameTicketsRepository::OnTicketDrawnCommit(const std::shared_ptr<Ticket>& winningTicket)
{
    m_WinningTickets.push_front(winningTicket);
}

const std::list<std::shared_ptr<Ticket> > &InGameTicketsRepository::GetTicketsStillInGame() const
{
    return m_UntouchedTickets;
}

const std::list<std::shared_ptr<Ticket> > &InGameTicketsRepository::GetWinningTicketsHistory() const
{
    return m_WinningTickets;
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
    for (const auto& mapPair: mem->m_MapOfStillInGameTickets)
    {
        const TicketsBlocksSet::TicketsBlockIdentification& id = mapPair.first;
        const std::set<int>& stillInGameTicketsOfBlock = mapPair.second;

        auto block = blocksSet->FindBlock(id);
        for (int ticketNumber: stillInGameTicketsOfBlock)
        {
            auto newTicket = std::make_shared<Ticket>(ticketNumber, *block);
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
