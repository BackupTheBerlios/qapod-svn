# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/qapod

FORMS += MainWindow.ui 
IDLS += MainWindow.ui 
HEADERS += MainWindow.h \
           getterapod.h \
           getter.h \
           getterepod.h \
           web.h 
SOURCES += qapod.cpp \
           MainWindow.cpp \
           getterapod.cpp \
           getter.cpp \
           getterepod.cpp \
           web.cpp 
CONFIG += designer thread
QT += network
TARGET=../bin/qapod
win32-msvc{
  LIBS+=user32.lib shell32.lib
}
