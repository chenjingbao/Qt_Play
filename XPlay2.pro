QT       += core gui opengl multimedia widgets openglextensions

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$/usr/local/ffmpeg/include
LIBS += -L/usr/local/ffmpeg/lib -lavcodec -lavutil -lswresample -lavformat -lswscale
-lswresample

SOURCES += \
    ivideocall.cpp \
    main.cpp \
    widget.cpp \
    xaudioplay.cpp \
    xaudiotread.cpp \
    xdecode.cpp \
    xdecodethread.cpp \
    xdemux.cpp \
    xdemuxthread.cpp \
    xresample.cpp \
    xslider.cpp \
    xvideothread.cpp \
    xvideowidget.cpp

HEADERS += \
    ivideocall.h \
    widget.h \
    xaudioplay.h \
    xaudiotread.h \
    xdecode.h \
    xdecodethread.h \
    xdemux.h \
    xdemuxthread.h \
    xresample.h \
    xslider.h \
    xvideothread.h \
    xvideowidget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
