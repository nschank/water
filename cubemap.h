#ifndef CUBEMAP_H
#define CUBEMAP_H

#define S 20

#include <QImage>

#include "GL/glew.h"
#include "lib/ResourceLoader.h"

class CubeMap
{
public:
  CubeMap(Camera *cam);
  virtual ~CubeMap();
  void Draw();

private:
  Camera camera;
  void loadSide(const char* filename, GLenum side);
  GLuint shader, vao, vbo, cubeTexture;
  int VertexCount;
  void GenVerts(int res);
  float *vertexData;

  static const char *topFilename,
                    *bottomFilename,
                    *leftFilename,
                    *rightFilename,
                    *frontFilename,
                    *backFilename;

  int size = 108;

  GLfloat mapPoints[size] = {
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

  };
};

#endif // CUBEMAP_H
