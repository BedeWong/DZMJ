QT += core gui widgets

CONFIG += c++11

TARGET = DZMJ
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pub/MJ_Player.cpp \
    pub/MJ_AnalyResult.cpp \
    pub/MJ_otherPlayer.cpp \
    pub/MJ_Base.cpp \
    pub/MJ_Request.cpp \
    cli/MJ_RequestLocal.cpp \
    pub/MJ_response.cpp \
    cli/MJ_LocalServer.cpp \
    cli/MJ_Cli.cpp \
    cli/MJ_Desktop.cpp \
    pub/MJ_RequestData.cpp \
    AI/MJ_Computer.cpp \
    cli/widget/MJ_HGPCWidget.cpp \
    cli/widget/MJ_CLockWidget.cpp \
    cli/widget/MJ_DiscardWidget.cpp \
    cli/widget/MJ_selfWidget.cpp \
    cli/widget/MJ_widgetBase.cpp \
    cli/widget/MJ_XiaJiaWidget.cpp \
    cli/widget/MJ_DuiMenWidget.cpp \
    cli/widget/MJ_ShangJiaWidget.cpp

HEADERS += \
    pub/MJ_Player.h \
    pub/MJ_AnalyResult.h \
    pub/MJ_otherPlayer.h \
    pub/MJ_Base.h \
    pub/MJ_Request.h \
    cli/MJ_RequestLocal.h \
    pub/MJ_response.h \
    cli/MJ_LocalServer.h \
    cli/MJ_Cli.h \
    cli/MJ_Desktop.h \
    pub/MJ_RequestData.h \
    AI/MJ_Computer.h \
    cli/widget/MJ_HGPCWidget.h \
    cli/widget/MJ_CLockWidget.h \
    cli/widget/MJ_DiscardWidget.h \
    cli/widget/MJ_selfWidget.h \
    cli/widget/MJ_widgetBase.h \
    cli/widget/MJ_XiaJiaWidget.h \
    cli/widget/MJ_DuiMenWidget.h \
    cli/widget/MJ_ShangJiaWidget.h

FORMS += \
    cli/MJ_Cli.ui \
    cli/MJ_Desktop.ui

RESOURCES += \
    cli/img/img.qrc
