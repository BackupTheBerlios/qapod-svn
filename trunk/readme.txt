run in auto mode:
	* commandline arg must contain "auto"


compile in windows:
	* open "Qt 4.1.0 Command Prompt"
	* cd to this dir
	* run :
		qmake -recursive
		make
	* if it is building debug instead of release (linking fails), set in
		C:\Qt\4.1.0\mkspecs\qconfig.pri CONFIG += ...release... instead of debug
