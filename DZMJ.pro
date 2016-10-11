QT += core
QT -= gui

CONFIG += c++11

TARGET = DZMJ
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pub\MJC_CardSet.cpp \
    pub\MJ_AnalyResult.cpp

HEADERS += \
    pub\MJC_CardSet.h \
    pub\MJ_AnalyResult.h
