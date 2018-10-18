#-------------------------------------------------
#
# Project created by QtCreator 2018-08-31T10:39:13
#
#-------------------------------------------------

QT       += core gui
QT +=xml
QT += network
CONFIG += qaxcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
UI_DIR +=../DOWNLOAD
TARGET = DOWNLOAD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialbase.cpp \
    qextserialport.cpp \
    win_qextserialport.cpp \
    libmodbus/modbus.c \
    libmodbus/modbus-data.c \
    libmodbus/modbus-rtu.c \
    libmodbus/modbus-tcp.c \
    mycombobox.cpp \
    mythread.cpp \
    loghandle.cpp \
    confighandle.cpp

HEADERS  += mainwindow.h \
    win_qextserialport.h \
    qextserialbase.h \
    qextserialport.h \
    libmodbus/config.h \
    libmodbus/modbus.h \
    libmodbus/modbus-private.h \
    libmodbus/modbus-rtu.h \
    libmodbus/modbus-rtu-private.h \
    libmodbus/modbus-tcp.h \
    libmodbus/modbus-tcp-private.h \
    libmodbus/modbus-version.h \
    mycombobox.h \
    mythread.h \
    loghandle.h \
    confighandle.h
RC_FILE = download.rc
FORMS    += mainwindow.ui
LIBS += -Ldll -lws2_32

