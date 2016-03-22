#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T15:52:47
#
#-------------------------------------------------

QT       += core gui svg

CONFIG += c++11 console
QMAKE_CXXFLAGS += -std=c++11 -O0 -g

include(src/xlsx/qtxlsx.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interface
TEMPLATE = app

copydata.commands = $(COPY_DIR) $$PWD/src/help $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

SOURCES += src/about.cpp \
    src/Brandes.cpp \
    src/Dijkstra.cpp \
    src/FileWriter.cpp \
    src/Graph.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/Measure.cpp \
    src/Node.cpp \
    src/Plane.cpp \
    src/Suurballe.cpp \
    src/DrawGraph.cpp \
    src/svgviewer.cpp \
    src/svgview.cpp \
    src/GraphEditor.cpp \
    src/graphicsview.cpp \
    src/editorpathitem.cpp \
    src/pngview.cpp \
    src/pngviewer.cpp

HEADERS  += src/about.h \
    src/Brandes.hpp \
    src/Dijkstra.hpp \
    src/FileWriter.hpp \
    src/Graph.hpp \
    src/mainwindow.h \
    src/Measure.hpp \
    src/Node.hpp \
    src/Plane.hpp \
    src/Suurballe.hpp \
    src/tree.hh \
    src/tree_util.hh \
    src/DrawGraph.hpp \
    src/svgviewer.h \
    src/svgview.h \
    src/GraphEditor.hpp \
    src/graphicsview.h \
    src/editorpathitem.h \
    src/pngview.h \
    src/pngviewer.h

FORMS    += src/mainwindow.ui \
    src/about.ui \
    src/svgviewer.ui \
    src/pngviewer.ui

INCLUDEPATH += $$PWD/OGDF/include
DEPENDPATH += $$PWD/OGDF/include

contains(QT_ARCH, i386) {
    unix: LIBS += -L$$PWD/OGDF/_release_32/ -lOGDF
    unix: PRE_TARGETDEPS += $$PWD/OGDF/_release_32/libOGDF.a
    unix: LIBS += -L$$PWD/OGDF/_release_32/ -lCOIN
    unix: PRE_TARGETDEPS += $$PWD/OGDF/_release_32/libCOIN.a
} else {
    unix: LIBS += -L$$PWD/OGDF/_release_64/ -lOGDF
    unix: PRE_TARGETDEPS += $$PWD/OGDF/_release_64/libOGDF.a
    unix: LIBS += -L$$PWD/OGDF/_release_64/ -lCOIN
    unix: PRE_TARGETDEPS += $$PWD/OGDF/_release_64/libCOIN.a
}
