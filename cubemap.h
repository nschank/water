#ifndef CUBEMAP_H
#define CUBEMAP_H

#define S 20

#include <QImage>

#include "camera.h"
#include "GL/glew.h"
#include "lib/ResourceLoader.h"

class CubeMap
{
public:
  CubeMap(Camera* cam);
  virtual ~CubeMap();
  void draw();

private:
  Camera* camera;
  void loadSide(uchar* bits, int width, int startX, int endX, int startY, int endY, GLenum side);
  GLuint cubemapShader, vao, vbo, cubeTexture;
  int VertexCount;
  void GenVerts(int res);
  float *vertexData;
  static const char *filename;
};

#endif // CUBEMAP_H
