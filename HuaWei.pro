TEMPLATE = app
CONFIG += console
CONFIG -= qt

CONFIG += c++11
LIBS += -lpthread

SOURCES += main.cpp \
    protocol.cpp \
    strategy.cpp \
    player.cpp \
    enviroment.cpp

HEADERS += \
    protocol.h \
    strategy.h \
    player.h \
    enviroment.h

