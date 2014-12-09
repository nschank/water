QT += core gui opengl

TARGET = final
TEMPLATE = app

QMAKE_CXXFLAGS += -g

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm lib
DEPENDPATH += glm lib

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    lib/ResourceLoader.cpp \
    sphere.cpp \
    camera.cpp \
    watersurface.cpp \
    cubemap.cpp

HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    sphere.h \
    camera.h \
    common.h \
    watersurface.h \
    cubemap.h

FORMS += mainwindow.ui

LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include

OTHER_FILES += \
    shaders/default.frag \
    shaders/default.vert \
    shaders/water.frag \
    shaders/water.vert \
    shaders/cubemap.frag \
    shaders/cubemap.vert
