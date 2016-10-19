#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T20:45:25
#
#-------------------------------------------------

include(../../defaults.pri)

QT       += core gui network

TARGET = TombolaLib
TEMPLATE = lib

DEFINES += TOMBOLALIB_LIBRARY

SOURCES += tombolalib.cpp \
    TicketsBlockFactory.cpp \
    Ticket.cpp \
    TicketFactory.cpp \
    RandomTicketDraw.cpp \
    TicketsBlock.cpp \
    BlockColorsSet.cpp \
    TicketsSellingPoint_ViewModel.cpp \
    TicketsBlocksSet.cpp \
    BlockColorsSet_ViewModel.cpp \
    TicketsSerializer.cpp \
    Controller.cpp \
    TicketsBlockSelection_ViewModel.cpp \
    SingleTicketDraw_ViewModel.cpp \
    InGameTicketsRepository.cpp \
    TombolaDocument.cpp \
    TicketDrawExecutor.cpp \
    Logger.cpp \
    TranslationLanguages_ViewModel.cpp \
    CrashHandler.cpp

HEADERS += tombolalib.h\
        tombolalib_global.h \
    TicketsBlockFactory.h \
    Ticket.h \
    TicketFactory.h \
    RandomTicketDraw.h \
    TicketsBlock.h \
    BlockColorsSet.h \
    TicketsSellingPoint_ViewModel.h \
    TicketsBlocksSet.h \
    BlockColorsSet_ViewModel.h \
    TicketsSerializer.h \
    Controller.h \
    TicketsBlockSelection_ViewModel.h \
    SingleTicketDraw_ViewModel.h \
    InGameTicketsRepository.h \
    TombolaDocument.h \
    TicketDrawExecutor.h \
    Logger.h \
    IMemento.h \
    TranslationLanguages_ViewModel.h \
    CrashHandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
