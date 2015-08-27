#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T15:34:18
#
#-------------------------------------------------

QT       += core gui
CONFIG += gnu++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11
TARGET = ComboGuiTest
TEMPLATE = app
INCLUDEPATH += /home/craig/QT/Image_SVM/

SOURCES += main.cpp\
    sobel.cpp \
    mainwindow.cpp \
    morphologyex.cpp \
    countours_rectangles.cpp \
    extractRegions.cpp \
    detectshapes.cpp \
    secondProcess.cpp \
    imagemanipulator.cpp\
    ../Image_SVM/trainerobject.cpp \
    ../Image_SVM/cbn_consoleoutput.cpp \
    ../Image_SVM/consoleOutputs.cpp \
    ../Image_SVM/file_io.cpp





HEADERS  += mainwindow.h \
    imagemanipulator.h \
    ../Image_SVM/trainerobject.h \
    ../Image_SVM/cbn_consoleoutput.h \
    ../Image_SVM/file_io.h


FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`






