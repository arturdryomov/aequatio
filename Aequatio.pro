QT	+= core gui

TARGET = Aequatio
TEMPLATE = app
CONFIG += help

mac {
	ICON = appicon/Aequatio.icns
}

win32 {
	RC_FILE = appicon/Aequatio.rc
	LIBS += -lGdi32 \
		-lUser32
}

SOURCES += \
	source/mainwindow.cpp \
	source/main.cpp \
	source/controller.cpp \
	source/exprcalculator.cpp \
	source/syntaxanalyzer.cpp \
	source/application.cpp \
	source/lexicalanalyzer.cpp \
	source/logger.cpp \
	source/exceptions.cpp \
	source/parsingexceptions.cpp \
	source/calculatingexceptions.cpp \
	source/aboutwindow.cpp \
    source/helpwindow.cpp \
    source/helpbrowser.cpp


HEADERS  += \
	source/mainwindow.h \
	source/controller.h \
	source/syntaxanalyzer.h \
	source/application.h \
	source/lexicalanalyzer.h \
	source/logger.h \
	source/exprcalculator.h \
	source/exceptions.h \
	source/parsingexceptions.h \
	source/calculatingexceptions.h \
	source/aboutwindow.h \
    source/helpwindow.h \
    source/helpbrowser.h

FORMS	+= \
	source/mainwindow.ui \
	source/aboutwindow.ui

RESOURCES += \
	aequatio.qrc

CONFIG(release, debug|release) {
	DESTDIR = release
}

CONFIG(debug, debug|release) {
	DESTDIR = debug
}

OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/rcc
UI_DIR = $$DESTDIR/ui

include(doc/doc.pri)
