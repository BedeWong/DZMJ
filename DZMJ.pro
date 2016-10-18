QT += core
QT -= gui

CONFIG += c++11

TARGET = DZMJ
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pub\MJ_Player.cpp \
    pub\MJ_AnalyResult.cpp \
    pub\MJ_otherPlayer.cpp \
    cli/cli_main.cpp \
    cli/MJplayer.cpp

HEADERS += \
    pub\MJ_Player.h \
    pub\MJ_AnalyResult.h \
    pub\MJ_otherPlayer.h \
    cli/cli_main.h \
    cli/MJplayer.h
