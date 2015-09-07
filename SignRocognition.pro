#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T15:34:18
#
#-------------------------------------------------

QT       += core gui
CONFIG += gnu++11
CONFIG += link_pkgconfig
PKGCONFIG += x11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11
TARGET = ComboGuiTest
TEMPLATE = app
INCLUDEPATH += /home/craig/git_repos/QT_Projects/Image_SVM/

SOURCES += main.cpp\
    sobel.cpp \
    mainwindow.cpp \
    morphologyex.cpp \
    countours_rectangles.cpp \
    extractRegions.cpp \
    detectshapes.cpp \
    secondProcess.cpp \
    imagemanipulator.cpp\
    ../QT_Projects/Image_SVM/trainerobject.cpp \
    ../QT_Projects/Image_SVM/cbn_consoleoutput.cpp \
    ../QT_Projects/Image_SVM/consoleOutputs.cpp \
    ../QT_Projects/Image_SVM/file_io.cpp\
    ../dlib/dlib/all/source.cpp





HEADERS  += mainwindow.h \
    imagemanipulator.h \
    ../QT_Projects/Image_SVM/trainerobject.h \
    ../QT_Projects/Image_SVM/cbn_consoleoutput.h \
    ../QT_Projects/Image_SVM/file_io.h


FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`

INCLUDEPATH+= /home/craig/git_repos/dlib/





