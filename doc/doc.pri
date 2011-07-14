DOC = help

help.target = doc
linux | unix | mac {
	help.commands = mv $$PWD/$${DOC}.qch $$OUT_PWD/$${DOC}.qch; \
		mv $$PWD/$${DOC}.qhc $$OUT_PWD/$${DOC}.qhc
}
win32 {
	help.commands = move $$PWD/$${DOC}.qch $$OUT_PWD/$${DOC}.qch; \
		move $$PWD/$${DOC}.qhc $$OUT_PWD/$${DOC}.qhc
}
help.depends = helpcollect

helpcollect.commands = qhelpgenerator $$PWD/$${DOC}.qhp -o $$PWD/$${DOC}.qch; \
	qcollectiongenerator $$PWD/$${DOC}.qhcp -o $$PWD/$${DOC}.qhc

QMAKE_EXTRA_TARGETS += help helpcollect
