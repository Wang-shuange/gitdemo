######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 15 18:28:30 2018
######################################################################
QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QT += axcontainer
CONFIG +=qaxcontainer
HEADERS += mainwindow.h \
           mycombobox.h \
           qextserialbase.h \
           qextserialport.h \
           win_qextserialport.h \
           libmodbus/config.h \
           libmodbus/modbus-private.h \
           libmodbus/modbus-rtu-private.h \
           libmodbus/modbus-rtu.h \
           libmodbus/modbus-tcp-private.h \
           libmodbus/modbus-tcp.h \
           libmodbus/modbus-version.h \
           libmodbus/modbus.h
FORMS += mainwindow.ui
SOURCES += main.cpp \
           mainwindow.cpp \
           mycombobox.cpp \
           qextserialbase.cpp \
           qextserialport.cpp \
           win_qextserialport.cpp \
           libmodbus/modbus-data.c \
           libmodbus/modbus-rtu.c \
           libmodbus/modbus-tcp.c \
           libmodbus/modbus.c
LIBS += -Ldll -lws2_32
