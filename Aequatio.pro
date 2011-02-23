QT	+= core gui

TARGET = Aequatio
TEMPLATE = app

SOURCES += \
	source/mainwindow.cpp \
	source/main.cpp \
    source/controller.cpp

HEADERS  += \
	source/mainwindow.h \
    source/controller.h

FORMS	+= \
	source/mainwindow.ui
