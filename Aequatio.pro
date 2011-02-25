QT	+= core gui

TARGET = Aequatio
TEMPLATE = app

SOURCES += \
	source/mainwindow.cpp \
	source/main.cpp \
    source/controller.cpp \
    source/syntaxanalyzer.cpp

HEADERS  += \
	source/mainwindow.h \
    source/controller.h \
    source/syntaxanalyzer.h

FORMS	+= \
	source/mainwindow.ui
