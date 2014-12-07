#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#ifndef __AT_HOME__
#include "GL/glew.h"
#else
#include "glew.h"
#endif

class ResourceLoader
{
public:
    ResourceLoader();
    static GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
};

#endif // RESOURCELOADER_H
