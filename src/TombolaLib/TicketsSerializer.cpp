#include "TombolaDocument.h"
#include "TicketsBlock.h"
#include "TicketsBlocksSet.h"
#include "TicketsSerializer.h"

#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

TicketsSerializer::TicketsSerializer(TombolaDocument& document)
    : m_TombolaDocument(document)
{

}

void TicketsSerializer::Write(QIODevice *outputDevice)
{
    QXmlStreamWriter xmlWriter(outputDevice);
    xmlWriter.setCodec("UTF-8"); //this is redundant, but that's how you set encoding
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setAutoFormattingIndent(4);

    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Tombola");
    xmlWriter.writeAttribute("PrizesCount", QString::number(m_TombolaDocument.PrizesCount));

    auto block = m_TombolaDocument.AllTicketsBlocksSet->GetFirstBlock();
    while(block)
    {
        xmlWriter.writeStartElement("TicketsBlock");
        xmlWriter.writeAttribute("Name", block->Name);
        xmlWriter.writeAttribute("ColorsSetIndex", QString::number(block->ColorsSetIndex));
        QString tickets;
        for (int ticket: block->GetSoldTicketIds())
            tickets = tickets % QString::number(ticket) % ";";
        xmlWriter.writeCharacters(tickets);
        xmlWriter.writeEndElement();

        block = m_TombolaDocument.AllTicketsBlocksSet->GetNextBlock(block);
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    // Make it possible to read all the written XML from starting position now
    outputDevice->seek(0);
}

void TicketsSerializer::Read(QIODevice *inputDevice)
{
    QXmlStreamReader xmlReader(inputDevice);
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        auto tokenType = xmlReader.readNext();
        if (tokenType == QXmlStreamReader::StartDocument)
            continue;

        if (tokenType == QXmlStreamReader::StartElement)
        {
            QString startElementName = xmlReader.name().toString();
            if (startElementName == "Tombola")
            {
                if (xmlReader.attributes().hasAttribute("PrizesCount"))
                    m_TombolaDocument.PrizesCount = xmlReader.attributes().value("PrizesCount").toInt();
            }
            else if (startElementName == "TicketsBlock")
            {
                auto block = m_TombolaDocument.AllTicketsBlocksSet->AddBlock();
                block->Name = xmlReader.attributes().value("Name").toString();
                block->ColorsSetIndex = xmlReader.attributes().value("ColorsSetIndex").toInt();
                QString ticketTokens = xmlReader.readElementText();
                auto ticketIds = ticketTokens.split(";", QString::SkipEmptyParts);
                for (auto it = ticketIds.constBegin(); it != ticketIds.constEnd(); ++it)
                    block->SetTicketSold(it->toInt(), true);
            }
        }
        else if (tokenType == QXmlStreamReader::EndElement)
        {
            if (xmlReader.name() == "TicketsBlock")
                continue;
        }
    }
}
