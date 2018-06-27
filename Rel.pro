#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T14:48:45
#
#-------------------------------------------------

QT += core gui
QT += xml
#CONFIG += qwt qwtpolar

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport svg

CONFIG -= std
CONFIG += c++11

TARGET = Rel
TEMPLATE = app


#LIBS += C:\Qt\Qt5.3.2\Projects\Rel\LIBs\visa32.lib
LIBS += D:\Sergey\Projects_on_Qt\Rel\LIBs\visa32.lib
#LIBS += D:\ProgProjects\QtProjects\Rel\LIBs\visa32.lib

#INCLUDEPATH += C:\Qt\Qt5.3.2\Projects\Rel\LIBs
INCLUDEPATH += D:\Sergey\Projects_on_Qt\Rel\LIBs
#INCLUDEPATH += D:\ProgProjects\QtProjects\Rel\LIBs

SOURCES += main.cpp\
        mainwindow.cpp \
        agb220xa.cpp \
        agb1500.c \
    eq_control.cpp \
    HC.cpp \
    EMIsoT.cpp \
    NBTI.cpp \
    VRDB.cpp \
    TBS.cpp \
    XML.cpp \
    Params.cpp \
    SMU.cpp \
    qcustomplot.cpp \
    utils.cpp \
    Prober.cpp \
    plot.cpp \
    viewdata.cpp \
    Test.cpp \
    GOI.cpp

HEADERS  += mainwindow.h \
        agb220xa.h \
        agb1500.h \
    measclasses.h \
    utils.h \
    eq_control.h \
    XML.h \
    qcustomplot.h \
    Prober.h \
    viewdata.h


FORMS    += mainwindow.ui \
    viewdata.ui

RESOURCES += \
    resources.qrc
