#-------------------------------------------------
#
# Project created by QtCreator 2013-10-23T12:18:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeManager
TEMPLATE = app


SOURCES += main.cpp\
        manager.cpp \
    basetask.cpp \
    event.cpp \
    add_new_event_dialog.cpp

HEADERS  += manager.h \
    basetask.h \
    event.h \
    add_new_event_dialog.h

FORMS    += manager.ui

win32:RC_FILE = TimeManager.rc
