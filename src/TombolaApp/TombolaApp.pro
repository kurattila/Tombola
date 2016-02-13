#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T20:41:32
#
#-------------------------------------------------

include(../../defaults.pri)

QT       += qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TombolaApp
TEMPLATE = app


SOURCES += main.cpp

HEADERS  += \
    version.h

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../TombolaLib/release/ -lTombolaLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../TombolaLib/debug/ -lTombolaLib
else:unix:!macx: LIBS += -L$$OUT_PWD/../TombolaLib/ -lTombolaLib

INCLUDEPATH += $$PWD/../TombolaLib
DEPENDPATH += $$PWD/../TombolaLib

DISTFILES += \
    main.qml \
    TombolaApp.rc

RESOURCES += \
    qml.qrc

RC_FILE = TombolaApp.rc
