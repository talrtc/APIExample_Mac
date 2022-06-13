#-------------------------------------------------
#
# Project created by QtCreator 2022-05-24T10:39:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RtcDemo
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


win32: {
    INCLUDEPATH += \
        $$PWD/../libDepend/rtc_sdk/include


    LIBS += \
        $$PWD/../libDepend/rtc_sdk/lib/rtc_sdk.lib



    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/../bin/Win32/Debug

    } else {
        DESTDIR = $$PWD/../bin/Win32/Release

        QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
    }


}


macx: {
DESTDIR = $$PWD/../bin/macx
INCLUDEPATH += \
    $$PWD/../libDepend/rtc_sdk.framework/Headers \
    $$PWD/../libDepend/OmniRtc.framework/Headers


LIBS += -F$$PWD/../libDepend/ -framework rtc_sdk
LIBS += -F$$PWD/../libDepend/ -framework OmniRtc

 QMAKE_INFO_PLIST = ./Info.plist

QMAKE_CFLAGS_RELEASE   = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE   = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

}

SOURCES += \
        main.cpp \
        mainwindow.cpp\
        Context.cpp\
        GlYuvWidget.cpp\
        onevideo.cpp

HEADERS += \
        mainwindow.h\
        Context.h\
        GlYuvWidget.h\
        onevideo.h\
        xes_rtc_engine.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
