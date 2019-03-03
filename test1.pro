#-------------------------------------------------
#
# Project created by QtCreator 2018-10-08T13:06:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    zlog.cpp \
    dowork.cpp \
    zhttpserver.cpp \
    zrequest.cpp \
    zresponse.cpp \
    zhttpthreadedserver.cpp \
    zthreadedsockethandler.cpp \
    zaction.cpp \
    zactionhelper.cpp \
    zactiontask.cpp \
    zlogicexception.cpp \
    zstringhelper.cpp

HEADERS += \
        mainwindow.h \
    zlog.h \
    dowork.h \
    zhttpserver.h \
    zrequest.h \
    zresponse.h \
    zhttpthreadedserver.h \
    zthreadedsockethandler.h \
    zactiontask.h \
    zaction.h \
    zactionhelper.h \
    globals.h \
    zlogicexception.h \
    zstringhelper.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += targe

# if defined(Q_PROCESSOR_ARM) DEFINES += RPI



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android:{
contains(QMAKE_HOST.arch, arm.*):{
        DEFINES += RPI
#wiringPI a GPIO kezeléséhez
LIBS += -L/usr/local/lib -lwiringPi
LIBS += -L/usr/local/lib -lwiringPiDev
        target.path = /home/pi/$${TARGET}/bin
    }else{
        DEFINES += NRPI
    }


# definíció a feltételes fordításhoz
    # DEFINES += RPI=true
}
!isEmpty(target.path): INSTALLS += target
