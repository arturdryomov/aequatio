QT += core gui

TARGET = aequatio
TEMPLATE = app
CONFIG += help

DEFINES += "VERSION_MAJOR=0"
DEFINES += "VERSION_MINOR=2"
DEFINES += "VERSION_BUILD=0"
DEFINES += "VERSION_REVISION=1000"

# VERSION_REVISION has following meaning:
#   1xxx - dev build
#   2axx - pre-release a
#   3axx - alpha a
#   4axx - beta a
#   5axx - RC a
#   9xxx - release
#   6xxx-8xxx - reserved

mac {
	ICON = resource/icons/mac/aequatio.icns
}

win32 {
	RC_FILE = resource/icons/windows/aequatio.rc
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
	source/helpbrowser.cpp \
	source/logwindow.cpp \
	source/rpncode.cpp \
	source/builtinfunctions/mathutils.cpp \
	source/builtinfunctions/basicfunctions.cpp \
	source/builtinfunctions/optimization/enumerative.cpp \
	source/builtinfunctions/optimization/bisection.cpp \
	source/builtinfunctions/optimization/dichotomy.cpp \
	source/builtinfunctions/optimization/goldensection.cpp \
	source/builtinfunctions/optimization/fibonacci.cpp \
	source/builtinfunctions/optimization/powell.cpp \
	source/builtinfunctions/optimization/hookejeeves.cpp \
	source/builtinfunctions/optimization/rosenbrock.cpp \
	source/builtinfunctions/optimization/adaptiverandom.cpp \
	source/builtinfunctions/optimization/returningrandom.cpp \
	source/builtinfunctions/optimization/besttrial.cpp \
	source/builtinfunctions/optimization/neldermead.cpp \
	source/builtinfunctions/optimization/conjugategradient.cpp \
	source/builtinfunctions/equationsolving/gauss.cpp \
	source/builtinfunctions/equationsolving/cramer.cpp \
	source/builtinfunctions/matrices.cpp \
	source/builtinfunctions/equationsolving/ludecomposition.cpp \
	source/builtinfunctions/equationsolving/lusolving.cpp

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
	source/helpbrowser.h \
	source/logwindow.h \
	source/rpncode.h \
	source/builtinfunctions/mathutils.h \
	source/builtinfunctions/builtinfunction.h \
	source/builtinfunctions/basicfunctions.h \
	source/builtinfunctions/optimization/enumerative.h \
	source/builtinfunctions/optimization/bisection.h \
	source/builtinfunctions/optimization/dichotomy.h \
	source/builtinfunctions/optimization/goldensection.h \
	source/builtinfunctions/optimization/fibonacci.h \
	source/builtinfunctions/optimization/powell.h \
	source/builtinfunctions/optimization/hookejeeves.h \
	source/builtinfunctions/optimization/rosenbrock.h \
	source/builtinfunctions/optimization/adaptiverandom.h \
	source/builtinfunctions/optimization/returningrandom.h \
	source/builtinfunctions/optimization/besttrial.h \
	source/builtinfunctions/optimization/neldermead.h \
	source/builtinfunctions/optimization/conjugategradient.h \
	source/builtinfunctions/equationsolving/gauss.h \
	source/builtinfunctions/equationsolving/cramer.h \
	source/builtinfunctions/matrices.h \
	source/builtinfunctions/equationsolving/ludecomposition.h \
	source/builtinfunctions/equationsolving/lusolving.h

FORMS	+= \
	source/mainwindow.ui \
	source/aboutwindow.ui

RESOURCES += \
	resource/icons/aequatio.qrc

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






