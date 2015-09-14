TEMPLATE = app

QT += qml quick widgets

# CONFIG += c++14
# QMAKE_CXXFLAGS += -std=c++14 -Wall -Wextra
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra

SOURCES += main.cpp \
    sequencer.cpp \
    sequence.cpp \
    sequencepoint.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sequencer.h \
    sequence.h \
    sequencepoint.h \
    utils.h
