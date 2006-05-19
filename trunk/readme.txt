compile in linux:
	* run:
		qmake -recursive
		make
	* executable is in bin/


compile in windows:
	* open "Qt 4.1.0 Command Prompt"
	* cd to this dir
	* run :
		qmake -recursive
		make
	* if it is building debug instead of release (linking fails), set in
		C:\Qt\4.1.0\mkspecs\qconfig.pri CONFIG += ...release... instead of debug
	* executable is somewhere bin/

run in auto mode:
	* commandline arg must contain "auto"





todo:
	other daily pictures: http://members.aol.com/gca7sky/daily.htm

