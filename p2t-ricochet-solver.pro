QT += core network
QT -= gui

CONFIG += c++11

TARGET = p2t-ricochet-solver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    p2tparser.cpp

HEADERS += \
    p2tparser.h
