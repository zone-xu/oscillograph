#-------------------------------------------------
#
# Project created by QtCreator 2022-09-27T08:45:12
#
#-------------------------------------------------
QT += core
QT += xml
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

################# 动态库制作 ##################
TEMPLATE = lib
DEFINES += OSCILLOGRAPH_LIBRARY

TARGET = oscillograph
#TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    qcustomplot.h \
    customplot.h \
    datatype.h \
    tracerplot.h \
    tracerline.h \
    track.h \
    setentry.h \
    setinfowidget.h \
    saveplcdata.h \
    filecontrol.h \
    slidingwindow.h \
    oscillograph_global.h

SOURCES += \
    qcustomplot.cpp \
    customplot.cpp \
    tracerplot.cpp \
    tracerline.cpp \
    track.cpp \
    setentry.cpp \
    setinfowidget.cpp \
    saveplcdata.cpp \
    filecontrol.cpp \
    slidingwindow.cpp \
#    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target1


include (segmentcheckedit/segmentcheckedit.pri)

RESOURCES += \
    resource.qrc

TRANSLATIONS += trans_osci.ts

CONFIG += translate

######## 静态/动态库其他环境配置 ##########
unix
{
    target.path = /usr/lib
    INSTALLS += target
}
