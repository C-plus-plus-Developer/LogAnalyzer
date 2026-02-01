QT       += core gui widgets

TARGET = LogViewer
TEMPLATE = app

SOURCES += main.cpp \
           ErrorBlock.cpp \
           mainwindow.cpp

HEADERS += ErrorBlock.h \
           mainwindow.h

FORMS += mainwindow.ui

LIBS += -lboost_iostreams
