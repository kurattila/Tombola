
#include "Controller.h"
#include "TombolaDocument.h"
#include "TicketsBlocksSet.h"
#include "TicketDrawExecutor.h"

#include <QFile>
#include <QtNetwork/QHostInfo>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/qnetworkaccessmanager.h>

QString Controller::XmlFullPath("Tombola.xml");

void Controller::checkNewVersion()
{
    auto computerName = QHostInfo::localHostName();
    QString requestString = QString("http://ubytovanie-vm.hyperlink.sk/Tombola-CheckNewVersion.php?computer=%1&version=1.0.0").arg(computerName);

    QNetworkAccessManager * mgr = new QNetworkAccessManager(/*this*/);
//    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinish(QNetworkReply*)));
    connect(mgr, SIGNAL(finished(QNetworkReply*)), mgr, SLOT(deleteLater()));

    mgr->get(QNetworkRequest(QUrl(requestString)));
}

Controller::Controller(TombolaDocument& document, TicketDrawExecutor& ticketDrawExecutor, QObject *parent)
    : QObject(parent)
    , m_TombolaDocument(document)
    , m_TicketDrawExecutor(ticketDrawExecutor)
    , m_Serializer(document)
{ }

void Controller::OnAppStartup()
{
    checkNewVersion();

    connect(&m_TombolaDocument, &TombolaDocument::ticketDrawExecutorSaveToMemento
          , [this](IMemento*& memento){ memento = m_TicketDrawExecutor.SaveToMemento(); });
    connect(&m_TombolaDocument, &TombolaDocument::ticketDrawExecutorRestoreFromMemento
          , [this](const IMemento* memento){ return m_TicketDrawExecutor.RestoreFromMemento(memento); });

    // Read from XML
    std::unique_ptr<QFile> file(new QFile(Controller::XmlFullPath));
    if (file->exists() && file->open(QIODevice::ReadOnly | QIODevice::Text))
        m_Serializer.Read(file.get());

    // Ensure that at least one TicketsBlock exists on app startup (should the XML not even exist, for example)
    if (m_TombolaDocument.AllTicketsBlocksSet->GetBlocksCount() == 0)
        m_TombolaDocument.AllTicketsBlocksSet->AddBlock();
}

void Controller::OnAppExit()
{
    // Write to XML
    std::unique_ptr<QFile> file(new QFile(Controller::XmlFullPath));
    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_Serializer.Write(file.get());
        file->flush();
    }
}
