QT	+= core gui

TARGET = Aequatio
TEMPLATE = app

win32 {
	LIBS += -lgdi32
}

SOURCES += \
	source/mainwindow.cpp \
	source/main.cpp \
    source/controller.cpp \
    source/syntaxanalyzer.cpp \
		source/application.cpp \
		source/lexicalanalyzer.cpp

HEADERS  += \
	source/mainwindow.h \
    source/controller.h \
    source/syntaxanalyzer.h \
		source/application.h \
		source/lexicalanalyzer.h

FORMS	+= \
	source/mainwindow.ui
