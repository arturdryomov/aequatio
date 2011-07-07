QT	+= core gui

TARGET = Aequatio
TEMPLATE = app

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
	source/lexicalanalyzer.cpp


HEADERS  += \
	source/mainwindow.h \
	source/controller.h \
	source/syntaxanalyzer.h \
	source/application.h \
	source/lexicalanalyzer.h \
	source/exprcalculator.h

FORMS	+= \
	source/mainwindow.ui
