DOC = help

help.target = doc

linux {
	help.commands = \
		mkdir -p $$OUT_PWD/share/doc/aequatio/ && \
		mv $$PWD/$${DOC}.qch $$OUT_PWD/share/doc/aequatio/$${DOC}.qch && \
		mv $$PWD/$${DOC}.qhc $$OUT_PWD/share/doc/aequatio/$${DOC}.qhc
}

mac {
	help.commands = \
		mkdir -p $$DESTDIR/$${TARGET}.app/Contents/Resources/doc && \
		mv $$PWD/$${DOC}.qch $$DESTDIR/$${TARGET}.app/Contents/Resources/doc/$${DOC}.qch && \
		mv $$PWD/$${DOC}.qhc $$DESTDIR/$${TARGET}.app/Contents/Resources/doc/$${DOC}.qhc
}

win32 {
	SOURCEDIR_WIN = $$PWD
	SOURCEDIR_WIN ~= s,/,\\,g
	DESTDIR_WIN = $$DESTDIR/help
	DESTDIR_WIN ~= s,/,\\,g

	help.commands = \
		cmd /C if not exist $${DESTDIR_WIN} (mkdir $${DESTDIR_WIN}) && \
		cmd /C move /Y $${SOURCEDIR_WIN}\\$${DOC}.qch $${DESTDIR_WIN}\\$${DOC}.qch && \
		cmd /C move /Y $${SOURCEDIR_WIN}\\$${DOC}.qhc $${DESTDIR_WIN}\\$${DOC}.qhc
}

help.depends = helpcollect

helpcollect.commands = \
	qhelpgenerator $$PWD/$${DOC}.qhp -o $$PWD/$${DOC}.qch &&  \
	qcollectiongenerator $$PWD/$${DOC}.qhcp -o $$PWD/$${DOC}.qhc

QMAKE_EXTRA_TARGETS += help helpcollect
