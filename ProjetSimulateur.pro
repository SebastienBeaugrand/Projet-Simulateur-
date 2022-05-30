QT -= core
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4)QT  += widgets

TEMPLATE = app

DEFINES  -= UNICODE
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++17

DEFINES += NDEBUG _CONSOLE
DEFINES += NON_MATLAB_PARSING
DEFINES += MAX_EXT_API_CONNECTIONS=255
DEFINES += DO_NOT_USE_SHARED_MEMORY
LIBS += C:\Qt\Tools\mingw1120_64\x86_64-w64-mingw32\lib\libws2_32.a
LIBS += C:\Qt\Tools\mingw1120_64\x86_64-w64-mingw32\lib\libwinmm.a


*-msvc* {
    QMAKE_CXXFLAGS += -O2
    QMAKE_CXXFLAGS += -W3
}
*-g++* {
    QMAKE_CXXFLAGS += -O3
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -Wno-empty-body
    QMAKE_CXXFLAGS += -Wno-write-strings

    QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
    QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
    QMAKE_CXXFLAGS += -Wno-narrowing

    QMAKE_CFLAGS += -O3
    QMAKE_CFLAGS += -Wall
    QMAKE_CFLAGS += -Wno-strict-aliasing
    QMAKE_CFLAGS += -Wno-unused-parameter
    QMAKE_CFLAGS += -Wno-unused-but-set-variable
    QMAKE_CFLAGS += -Wno-unused-local-typedefs
}

WIN32 {
    LIBS += -lwinmm
    LIBS += -lWs2_32
}

INCLUDEPATH += "../include"
INCLUDEPATH += "../remoteApi"


SOURCES += \
    extApi.c \
    extApiPlatform.c \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    extApi.h \
    extApiInternal.h \
    extApiPlatform.h \
    mainwindow.h \
    simConst.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
