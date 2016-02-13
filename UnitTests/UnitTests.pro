#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T20:52:46
#
#-------------------------------------------------

include(../defaults.pri)

QT       += core testlib

TARGET = UnitTests
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    TicketsBlockFactory_Tests.cpp \
    TicketFactory_Tests.cpp \
    RandomTicketDraw_Tests.cpp \
    TicketsBlock_Tests.cpp \
    TicketsBlocksSet_Tests.cpp \
    InGameTicketsRepository_Tests.cpp \
    TicketDrawExecutor_Tests.cpp \
    SingleTicketDraw_ViewModel_Tests.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/TombolaLib/release/ -lTombolaLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/TombolaLib/debug/ -lTombolaLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../src/TombolaLib/ -lTombolaLib

INCLUDEPATH += $$PWD/../src/TombolaLib
DEPENDPATH += $$PWD/../src/TombolaLib

HEADERS += \
    AutoTest.h
