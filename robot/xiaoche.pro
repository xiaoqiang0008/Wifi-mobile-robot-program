#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T18:27:14
#
#-------------------------------------------------

QT       += core gui

QT += network
greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = xiaoche
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chiledialog.cpp \
    cardialog.cpp \
    manpudialog.cpp \
    tthersdialog.cpp \
    network.cpp \
    camera.cpp \
    mcurvepaint.cpp \
    mmanipulatorcontroler.cpp

HEADERS  += mainwindow.h \
    network.h \
    camera.h \
    mcurvepaint.h \
    mmanipulatorcontroler.h

FORMS    += mainwindow.ui \
    chiledialog.ui \
    cardialog.ui \
    manpudialog.ui \
    tthersdialog.ui

QT += multimedia
QT += multimediawidgets



RESOURCES += \
    resource.qrc

RC_FILE = icon.rc




