#-------------------------------------------------
#
# Project created by QtCreator 2018-09-09T01:18:16
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = dummylauncher
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
        dialog.cpp \
    protocol/protocol.cpp \
    protocol/updaterprotocol.cpp \
    state/state.cpp \
    state/updater_state/updaterreceivefilesstate.cpp \
    state/updater_state/updaterreceivefilescountstate.cpp \
    state/updater_state/updaterdownloadfilesstate.cpp \
    state/updater_state/updaterdownloadsinglefilestate.cpp \
    state/masterinitialstate.cpp \
    state/updater_state/updaterinitalstate.cpp

HEADERS += \
        dialog.h \
    protocol/protocol.h \
    protocol/updaterprotocol.h \
    state/state.h \
    state/updater_state/updaterreceivefilesstate.h \
    state/updater_state/updaterreceivefilescountstate.h \
    state/updater_state/updaterdownloadfilesstate.h \
    state/updater_state/updaterdownloadsinglefilestate.h \
    state/masterinitialstate.h \
    state/updater_state/updaterinitalstate.h

FORMS += \
        dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
