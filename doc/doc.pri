DOC = help

help.target = doc

linux | unix | mac {
	help.commands = \
		mv $$PWD/$${DOC}.qch $$DESTDIR/$${DOC}.qch && \
		mv $$PWD/$${DOC}.qhc $$DESTDIR/$${DOC}.qhc
}

win32 {
	SOURCEDIR_WIN = $$PWD
	SOURCEDIR_WIN ~= s,/,\\,g
	DESTDIR_WIN = $$DESTDIR
	DESTDIR_WIN ~= s,/,\\,g

	help.commands = \
		cmd /C move /Y $${SOURCEDIR_WIN}\\$${DOC}.qch $${DESTDIR_WIN}\\$${DOC}.qch && \
		cmd /C move /Y $${SOURCEDIR_WIN}\\$${DOC}.qhc $${DESTDIR_WIN}\\$${DOC}.qhc
}

help.depends = helpcollect

helpcollect.commands = \
	qhelpgenerator $$PWD/$${DOC}.qhp -o $$PWD/$${DOC}.qch &&  \
	qcollectiongenerator $$PWD/$${DOC}.qhcp -o $$PWD/$${DOC}.qhc

QMAKE_EXTRA_TARGETS += help helpcollect
