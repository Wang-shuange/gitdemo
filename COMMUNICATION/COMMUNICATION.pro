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

TARGET = COMMUNICATION
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
    mycombobox.cpp

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
    mycombobox.h
RC_FILE = comm.rc
FORMS    += mainwindow.ui
LIBS += -Ldll -lws2_32

