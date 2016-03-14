TEMPLATE = app
TARGET = DicomReader
QT += core gui opengl
CONFIG += console release
DEFINES += HAVE_CONFIG_H

unix:QMAKE_CXXFLAGS += -std=c++11

RESOURCES += shaders.qrc
HEADERS += *.h
SOURCES += *.cpp
LIBS += -ldcmdata -ldcmimage -ldcmimgle -lofstd
