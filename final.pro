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
    watersurfacemodule.cpp \
    OrbitingCamera.cpp \
    CamtransCamera.cpp

HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    watersurfacemodule.h \
    Camera.h \
    OrbitingCamera.h \
    CamtransCamera.h

FORMS += mainwindow.ui

OTHER_FILES += \
    shaders/default.frag \
    shaders/default.vert


#####SUNLAB##########
#LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
#INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
#DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include


#######WINDOWS#######
INCLUDEPATH += glew
DEPENDPATH += glew
DEFINES += GLEW_STATIC __AT_HOME__
HEADERS += glew/glew.h glew/wglew.h
SOURCES += glew/glew.c


