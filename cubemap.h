#ifndef CUBEMAP_H
#define CUBEMAP_H

#define S 20

#include <QImage>

#include "camera.h"
#include "lib/ResourceLoader.h"

#ifndef __AT_HOME__
#include "GL/glew.h"
#else
#include "glew.h"
#endif

class CubeMap
{
public:
  CubeMap(Camera* cam);
  virtual ~CubeMap();
  void draw();
  void setShaderSamplerCube(GLuint otherShader, char *propertyName);

private:
  Camera* camera;
  void loadSide(const char* filename, GLenum side);
  GLuint cubemapShader, vao, vbo, cubeTexture;
  int VertexCount;
  void GenVerts(int res);
  float *vertexData;

  static const char *topFilename,
                    *bottomFilename,
                    *leftFilename,
                    *rightFilename,
                    *frontFilename,
                    *backFilename;

  /*GLfloat mapPoints[108] = {
    // top
    S, S, S, 0.0f, 0.0f,
    -S, S, -S, 0.0f, 0.0f,
    -S, S, S, 0.0f, 0.0f,

    S, S, -S, 0.0f, 0.0f,
    S, S, S, 0.0f, 0.0f,
    -S, S, S, 0.0f, 0.0f,

    // bottom
    S, S, -S, 0.0f, 0.0f,
    -S, S, S, 0.0f, 0.0f,
    -S, S, -S, 0.0f, 0.0f,

    S, S, -S, 0.0f, 0.0f,
    S, S, S, 0.0f, 0.0f,
    -S, S, -S, 0.0f, 0.0f,

    // left


    // right


    // front


    // back

  };*/
};

#endif // CUBEMAP_H
