QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

RC_FILE = app.rc

SOURCES += \
    FilesContents.cpp \
    main.cpp \
    Widget.cpp

HEADERS += \
    FilesContents.h \
    Widget.h

FORMS += \
    Widget.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
