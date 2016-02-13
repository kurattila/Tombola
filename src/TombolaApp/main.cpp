#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <memory>

#include "Controller.h"
#include "TombolaDocument.h"
#include "TicketsBlocksSet.h"
#include "TicketsSellingPoint_ViewModel.h"
#include "BlockColorsSet_ViewModel.h"
#include "TicketDrawExecutor.h"
#include "SingleTicketDraw_ViewModel.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Logger::Init("Tombola.log.txt");
    qInfo() << "\n\n" << "========== Startup ==========";

    TombolaDocument document;
    Controller controller(document);
    controller.OnAppStartup();

    TicketsSellingPoint_ViewModel ticketsSellingPoint_ViewModel(document);
//    qmlRegisterType<BlockColorsSet_ViewModel>("com.quatso.tombola", 1, 0, "BlockColorSetListModel");
    TicketDrawExecutor ticketDrawExecutor(  document
                                          , new SingleTicketDraw_ViewModel()
                                          , new SingleTicketDraw_ViewModel());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ticketsSellingPoint", QVariant::fromValue(&ticketsSellingPoint_ViewModel));
    engine.rootContext()->setContextProperty("ticketDrawExecutor", QVariant::fromValue(&ticketDrawExecutor));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QIcon appIcon(":///Images/TombolaAppIcon.png");
    app.setWindowIcon(appIcon);

    int appRetVal = app.exec();
    controller.OnAppExit();
    return appRetVal;
}
