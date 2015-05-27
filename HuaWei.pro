TEMPLATE = app
CONFIG += console
CONFIG -= qt
TARGET = ~/game/works/target/game

CONFIG += c++11
LIBS += -lpthread

SOURCES += main.cpp \
    protocol.cpp \
    strategy.cpp \
    player.cpp \
    enviroment.cpp \
    bl_mutex.cpp

HEADERS += \
    protocol.h \
    strategy.h \
    player.h \
    enviroment.h \
    bl_mutex.h

