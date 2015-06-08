#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T15:26:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    syntaxlighter.cpp \
    codeeditor.cpp \
    options.cpp

HEADERS  += mainwindow.h \
    syntaxlighter.h \
    codeeditor.h \
    options.h

FORMS    += mainwindow.ui \
    options.ui

RESOURCES += \
    appresources.qrc \
    completer.qrc 


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QScintilla-gpl-2.9/Qt4Qt5/release/ -lqscintilla2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QScintilla-gpl-2.9/Qt4Qt5/debug/ -lqscintilla2
else:unix: LIBS += -L$$PWD/QScintilla-gpl-2.9/Qt4Qt5/ -lqscintilla2

INCLUDEPATH += $$PWD/QScintilla-gpl-2.9/Qt4Qt5/Qsci
DEPENDPATH += $$PWD/QScintilla-gpl-2.9/Qt4Qt5/Qsci

DEFINES += QSCINTILLA_DLL SCI_LEXER
