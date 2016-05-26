#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T15:35:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat
TEMPLATE = app


SOURCES += main.cpp\
        chat.cpp \
    socket.cpp \
    mythread.cpp

HEADERS  += chat.h \
    socket.h \
    mythread.h

FORMS    += chat.ui
