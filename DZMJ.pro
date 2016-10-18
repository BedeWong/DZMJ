QT += core gui widgets

CONFIG += c++11

TARGET = DZMJ
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pub\MJ_Player.cpp \
    pub\MJ_AnalyResult.cpp \
    pub\MJ_otherPlayer.cpp \
    cli\MJplayer.cpp \
    pub/MJ_Base.cpp \
    pub/MJ_Request.cpp \
    cli/MJ_RequestLocal.cpp \
    pub/MJ_response.cpp \
    cli/MJ_LocalServer.cpp \
    cli/MJ_Cli.cpp \
    cli/MJ_Desktop.cpp \
    pub/MJ_RequestData.cpp

HEADERS += \
    pub\MJ_Player.h \
    pub\MJ_AnalyResult.h \
    pub\MJ_otherPlayer.h \
    cli\MJplayer.h \
    pub/MJ_Base.h \
    pub/MJ_Request.h \
    cli/MJ_RequestLocal.h \
    pub/MJ_response.h \
    cli/MJ_LocalServer.h \
    cli/MJ_Cli.h \
    cli/MJ_Desktop.h \
    pub/MJ_RequestData.h

FORMS += \
    cli/MJ_Cli.ui \
    cli/MJ_Desktop.ui
