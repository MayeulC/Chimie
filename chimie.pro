######################################################################
# Automatically generated by qmake (2.01a) lun. 29. avr. 01:50:02 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . release
INCLUDEPATH += .

# Input
HEADERS += classes.h fenapropos.h mainwindow.h reglagescourbes.h tools.h vue.h
FORMS += fenapropos.ui mainwindow.ui reglagescourbes.ui
SOURCES += classes.cpp \
           fenapropos.cpp \
           main.cpp \
           mainwindow.cpp \
           reglagescourbes.cpp \
           tools.cpp \
           vue.cpp
RESOURCES += ressources.qrc

RC_FILE = ressources.rc

#LIBS += -LE:\\Qt\\SDL-1.2.15\\lib -lSDL.dll

#win32:LIBS += -Wl,-Bstatic -LE:/Qt/SDL-1.2.15/lib  -lmingw32 -lSDLmain -lSDL.dll  -mwindows  -lm -luser32 -lgdi32 -lwinmm -ldxguid  -Wl,-Bdynamic
LIBS += -lSDL

