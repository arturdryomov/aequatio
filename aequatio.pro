QT += core gui

TARGET = aequatio
TEMPLATE = app
CONFIG += help

DEFINES += "VERSION_MAJOR=0"
DEFINES += "VERSION_MINOR=3"
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
	source/main.cpp \
	source/controller.cpp \
	source/document.cpp \
	source/lexer.cpp \
	source/parser.cpp \
	source/prettyprinter.cpp \
	source/codegenerator.cpp \
	source/calculator.cpp \
	source/rpncode.cpp \
	source/number.cpp \
	source/application.cpp \
	source/logger.cpp \
	source/exceptions.cpp \
	source/incorrectinputexceptions.cpp \
	source/mainwindow.cpp \
	source/aboutwindow.cpp \
	source/helpwindow.cpp \
	source/helpbrowser.cpp \
	source/logwindow.cpp \
	source/builtin/function.cpp \
	source/builtin/basicconstants.cpp \
	source/builtin/mathutils.cpp \
	source/builtin/basicfunctions.cpp \
	source/builtin/optimization/enumerative.cpp \
	source/builtin/optimization/bisection.cpp \
	source/builtin/optimization/dichotomy.cpp \
	source/builtin/optimization/goldensection.cpp \
	source/builtin/optimization/fibonacci.cpp \
	source/builtin/optimization/powell.cpp \
	source/builtin/optimization/hookejeeves.cpp \
	source/builtin/optimization/rosenbrock.cpp \
	source/builtin/optimization/adaptiverandom.cpp \
	source/builtin/optimization/returningrandom.cpp \
	source/builtin/optimization/besttrial.cpp \
	source/builtin/optimization/neldermead.cpp \
	source/builtin/optimization/conjugategradient.cpp \
	source/builtin/equationsolving/gauss.cpp \
	source/builtin/equationsolving/cramer.cpp \
	source/builtin/matrices.cpp \
	source/builtin/equationsolving/lusolving.cpp \
	source/builtin/equationsolving/pludecomposition.cpp \
	source/builtin/equationsolving/gaussseidel.cpp \
	source/builtin/derivative.cpp \
	source/builtin/equationsolving/newton.cpp \
	source/builtin/lagrangeinterpolation.cpp

HEADERS += \
	source/controller.h \
	source/document.h \
	source/lexer.h \
	source/parser.h \
	source/prettyprinter.h \
	source/codegenerator.h \
	source/calculator.h \
	source/rpncode.h \
	source/number.h \
	source/application.h \
	source/logger.h \
	source/exceptions.h \
	source/incorrectinputexceptions.h \
	source/mainwindow.h \
	source/aboutwindow.h \
	source/helpwindow.h \
	source/helpbrowser.h \
	source/logwindow.h \
	source/builtin/function.h \
	source/builtin/constant.h \
	source/builtin/basicconstants.h \
	source/builtin/mathutils.h \
	source/builtin/basicfunctions.h \
	source/builtin/optimization/enumerative.h \
	source/builtin/optimization/bisection.h \
	source/builtin/optimization/dichotomy.h \
	source/builtin/optimization/goldensection.h \
	source/builtin/optimization/fibonacci.h \
	source/builtin/optimization/powell.h \
	source/builtin/optimization/hookejeeves.h \
	source/builtin/optimization/rosenbrock.h \
	source/builtin/optimization/adaptiverandom.h \
	source/builtin/optimization/returningrandom.h \
	source/builtin/optimization/besttrial.h \
	source/builtin/optimization/neldermead.h \
	source/builtin/optimization/conjugategradient.h \
	source/builtin/equationsolving/gauss.h \
	source/builtin/equationsolving/cramer.h \
	source/builtin/matrices.h \
	source/builtin/equationsolving/lusolving.h \
	source/builtin/equationsolving/pludecomposition.h \
	source/builtin/equationsolving/gaussseidel.h \
	source/builtin/derivative.h \
	source/builtin/equationsolving/newton.h \
	source/builtin/lagrangeinterpolation.h

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


