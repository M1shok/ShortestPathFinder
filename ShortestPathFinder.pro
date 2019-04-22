#-------------------------------------------------
#
# Project created by QtCreator 2018-12-01T12:21:08
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShortestPathFinder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    vertex.cpp \
    point.cpp \
    pathfinder.cpp \
    presentor.cpp \
    exception.cpp \
    fileexception.cpp \
    mainscene.cpp \
    graphicscell.cpp \
    csvroom.cpp \
    roomdb.cpp \
    languagesdb.cpp \
    savingdialog.cpp \
    mazegenerator.cpp \
    maingraphicsview.cpp \
    settingsdialog.cpp \
    signalpoint.cpp \
    pathfinderresult.cpp \
    roomtabledialog.cpp \
    memoryexception.cpp \
    algorythmexception.cpp

HEADERS += \
        mainwindow.h \
    vertex.h \
    point.h \
    pathfinder.h \
    presentor.h \
    exception.h \
    fileexception.h \
    mainscene.h \
    graphicscell.h \
    csvroom.h \
    roomdb.h \
    languagesdb.h \
    savingdialog.h \
    mazegenerator.h \
    maingraphicsview.h \
    settingsdialog.h \
    signalpoint.h \
    pathfinderresult.h \
    roomtabledialog.h \
    memoryexception.h \
    algorythmexception.h

FORMS += \
    savingdialog.ui \
    roomtabledialog.ui \
    mainwindow.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
