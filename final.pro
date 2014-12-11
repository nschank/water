QT += core gui opengl

TARGET = final
TEMPLATE = app

QMAKE_CXXFLAGS += -g -w

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm lib phys
DEPENDPATH += glm lib phys

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    lib/ResourceLoader.cpp \
    sphere.cpp \
    camera.cpp \
    watersurface.cpp \
    cubemap.cpp \
    phys/entity.cpp \
    phys/world.cpp \
    phys/sphereentity.cpp

HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    sphere.h \
    camera.h \
    common.h \
    watersurface.h \
    cubemap.h \
    phys/world.h \
    phys/entity.h \
    phys/sphereentity.h \
    settings.h

FORMS += mainwindow.ui

OTHER_FILES += \
    shaders/default.frag \
    shaders/default.vert \
    shaders/water.frag \
    shaders/water.vert \
    shaders/cubemap.frag \
    shaders/cubemap.vert

#####SUNLAB##########
LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include

#######WINDOWS#######
#INCLUDEPATH += glew
#DEPENDPATH += glew
#DEFINES += GLEW_STATIC __AT_HOME__
#HEADERS += glew/glew.h glew/wglew.h
#SOURCES += glew/glew.c

