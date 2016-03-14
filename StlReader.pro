TEMPLATE = app
TARGET = StlReader
QT += core gui opengl
CONFIG += console #debug

unix:QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += shaders.qrc
HEADERS += *.h
SOURCES += *.cpp 
