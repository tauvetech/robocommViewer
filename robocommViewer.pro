#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T12:26:21
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robocommViewer
TEMPLATE = app

CONFIG += qwt

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui
