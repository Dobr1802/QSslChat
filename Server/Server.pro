#-------------------------------------------------
#
# Project created by QtCreator 2011-07-11T20:38:20
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    sslserver.cpp \
    registrationdialog.cpp \
    user.cpp

HEADERS  += server.h \
    sslserver.h \
    user.h \
    registrationdialog.h

FORMS    += server.ui \
    registrationdialog.ui
