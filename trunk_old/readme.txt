compile in windows:
	* open "Qt 4.1.0 Command Prompt"
	* cd to this dir
	* run :
		qmake -recursive
		make
	* if it is building debug instead of release (linking fails), set in
		C:\Qt\4.1.0\mkspecs\qconfig.pri CONFIG += ...release... instead of debug
		OR (should work...)
		in src\src.pro: CONFIG += release
	* to build static, it results in a approx. 6MB exe, if 48MB debug is enabled ;-)

run in auto mode:
	* commandline arg must contain "auto"

static qt in windows built with:
# images do not work: configure -plugin-sql-sqlite -plugin-sql-odbc -qt-libpng -qt-libjpeg -static -fast 
configure -plugin-sql-sqlite -plugin-sql-odbc -qt-libpng -system-libjpeg -static -fast 



todo:
	other daily pictures: http://members.aol.com/gca7sky/daily.htm

