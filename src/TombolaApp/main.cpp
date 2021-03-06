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
#include "DynamicTranslation.h"
#include "CrashHandler.h"

int main(int argc, char *argv[])
{
    Logger::Init("Tombola.log.txt");
    qInfo() << "\n\n" << "========== Startup ==========";

    CrashHandler crashHandler;

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Tombola");     // QSettings init early enough
    QCoreApplication::setOrganizationName("Kúr Attila"); // QSettings init early enough

    DynamicTranslation dynamicTranslation(app);

    TombolaDocument document;

//    qmlRegisterType<BlockColorsSet_ViewModel>("com.quatso.tombola", 1, 0, "BlockColorSetListModel");
    TicketDrawExecutor ticketDrawExecutor(  document
                                          , new SingleTicketDraw_ViewModel()
                                          , new SingleTicketDraw_ViewModel());

    Controller controller(document, ticketDrawExecutor);
    controller.OnAppStartup();

    TicketsSellingPoint_ViewModel ticketsSellingPoint_ViewModel(document);
    ticketsSellingPoint_ViewModel.Init(ticketDrawExecutor.IsPrizeDrawingRunning());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ticketsSellingPoint", QVariant::fromValue(&ticketsSellingPoint_ViewModel));
    engine.rootContext()->setContextProperty("ticketDrawExecutor", QVariant::fromValue(&ticketDrawExecutor));
    engine.rootContext()->setContextProperty("dynamicTranslation", &dynamicTranslation);
    engine.rootContext()->setContextProperty("crashHandler", &crashHandler);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QIcon appIcon(":///Images/TombolaAppIcon.png");
    app.setWindowIcon(appIcon);

    int appRetVal = app.exec();
    controller.OnAppExit();
    return appRetVal;
}
