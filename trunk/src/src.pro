# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/qapod

FORMS += MainWindow.ui 
IDLS += MainWindow.ui 
HEADERS += MainWindow.h \
           getterapod.h \
           getter.h 
SOURCES += qapod.cpp \
           MainWindow.cpp \
           getterapod.cpp \
           getter.cpp 
CONFIG += designer
QT += network
TARGET=../bin/qapod
