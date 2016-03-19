#include "TombolaDocument.h"
#include "TicketsSerializer.h"

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

    std::unique_ptr<IMemento> memento(m_TombolaDocument.SaveToMemento());
    memento->Write(xmlWriter);

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
        if (tokenType == QXmlStreamReader::StartElement)
        {
            QString startElementName = xmlReader.name().toString();
            if (startElementName == "Tombola")
            {
                TombolaMemento memento;
                memento.Read(xmlReader);

                m_TombolaDocument.RestoreFromMemento(&memento);
            }
        }
    }
}
