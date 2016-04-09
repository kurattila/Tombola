#include "TombolaDocument.h"
#include "TicketsBlock.h"
#include "TicketDrawExecutor.h"

#include <QStringBuilder>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

TombolaDocument::TombolaDocument(QObject *parent)
    : QObject(parent)
{
    AllTicketsBlocksSet = std::make_shared<TicketsBlocksSet>();
}

TombolaDocument::~TombolaDocument()
{

}

IMemento *TombolaDocument::SaveToMemento()
{
    auto memento = new TombolaMemento();
    memento->PrizesCount = PrizesCount;
    memento->AllTicketsBlocksSet = AllTicketsBlocksSet;

    IMemento* prizeDrawingMemento = nullptr;
    emit ticketDrawExecutorSaveToMemento(prizeDrawingMemento);
    memento->TicketDrawExecMemento.reset(prizeDrawingMemento);

    return memento;
}

void TombolaDocument::RestoreFromMemento(const IMemento *memento, void* /*context*/)
{
    const TombolaMemento* mem = dynamic_cast<const TombolaMemento*>(memento);
    if (!mem)
        throw std::logic_error("Awaiting TombolaMemento instance");

    PrizesCount = mem->PrizesCount;
    AllTicketsBlocksSet = mem->AllTicketsBlocksSet;
    emit ticketDrawExecutorRestoreFromMemento(mem->TicketDrawExecMemento.get());
}

TombolaMemento::TombolaMemento()
    : AllTicketsBlocksSet(std::make_shared<TicketsBlocksSet>())
    , TicketDrawExecMemento(new TicketDrawExecutorMemento())
{

}

TombolaMemento::~TombolaMemento()
{

}

void TombolaMemento::Read(QXmlStreamReader &xmlReader)
{
    if (xmlReader.name() != "Tombola")
        throw std::invalid_argument("Awaiting Tombola tag in XML");

    if (xmlReader.attributes().hasAttribute("PrizesCount"))
        PrizesCount = xmlReader.attributes().value("PrizesCount").toInt();

    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        auto tokenType = xmlReader.readNext();
        if (tokenType == QXmlStreamReader::StartElement && xmlReader.name() == "TicketsBlock")
        {
            auto block = AllTicketsBlocksSet->AddBlock();
            block->Name = xmlReader.attributes().value("Name").toString();
            block->ColorsSetIndex = xmlReader.attributes().value("ColorsSetIndex").toInt();
            QString ticketTokens = xmlReader.readElementText();
            auto ticketIds = ticketTokens.split(";", QString::SkipEmptyParts);
            for (auto it = ticketIds.constBegin(); it != ticketIds.constEnd(); ++it)
                block->SetTicketSold(it->toInt(), true);
        }
        else if (tokenType == QXmlStreamReader::StartElement && xmlReader.name() == TicketDrawExecutorMemento::StartElementXmlName)
        {
            TicketDrawExecMemento->Read(xmlReader);
        }

        if (tokenType == QXmlStreamReader::EndElement && xmlReader.name() == "Tombola")
            break;
    }
}

void TombolaMemento::Write(QXmlStreamWriter &xmlWriter)
{
    xmlWriter.writeStartElement("Tombola");

    xmlWriter.writeAttribute("PrizesCount", QString::number(PrizesCount));
    auto block = AllTicketsBlocksSet->GetFirstBlock();
    while(block)
    {
        xmlWriter.writeStartElement("TicketsBlock");
        xmlWriter.writeAttribute("Name", block->Name);
        xmlWriter.writeAttribute("ColorsSetIndex", QString::number(block->ColorsSetIndex));
        QString tickets;
        for (int ticket: block->GetSoldTicketIds())
            tickets = tickets % QString::number(ticket) % ";";
        xmlWriter.writeCharacters(tickets);
        xmlWriter.writeEndElement(); // </TicketsBlock>

        block = AllTicketsBlocksSet->GetNextBlock(block);
    }
    TicketDrawExecMemento->Write(xmlWriter);

    xmlWriter.writeEndElement(); // </Tombola>
}
