#include "cubemap.h"
#include <stdio.h>

const char *CubeMap::topFilename = "clouds.jpg";
const char *CubeMap::bottomFilename = "clouds.jpg";
const char *CubeMap::leftFilename = "clouds.jpg";
const char *CubeMap::rightFilename = "clouds.jpg";
const char *CubeMap::frontFilename = "clouds.jpg";
const char *CubeMap::backFilename = "clouds.jpg";

CubeMap::CubeMap(Camera *cam) {
  camera = cam;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), mapPoints, GL_STATIC_DRAW);

  glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));
  glEnableVertexAttribArray(glGetAttribLocation(shader, "texCoord"));

  glVertexAttribPointer(
              glGetAttribLocation(shader, "position"),
              3,
              GL_FLOAT,
              GL_FALSE,
              sizeof(GLfloat)*5,
              (void *)0
              );
  glVertexAttribPointer(
          glGetAttribLocation(m_shader, "texCoord"),
          2,
          GL_FLOAT,
          GL_FALSE,
          sizeof(GLfloat) * 5,
          (void*) (sizeof(GLfloat) * 3)
      );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  shader = ResourceLoader::loadShaders(
      "shaders/cubemap.vert",
      "shaders/cubemap.frag");



  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &cubeTexture);
  loadSide(topFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
  loadSide(bottomFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
  loadSide(leftFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
  loadSide(rightFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
  loadSide(frontFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
  loadSide(backFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}

CubeMap::~CubeMap() {

}

void CubeMap::loadSide(const char* filename, GLenum side) {
  QImage textureFile = QImage(QString(filename));
  if(!textureFile.isNull()) {
    printf("side not null\n");
    uchar* bits = textureFile.bits();
    int w = textureFile.width(),
        h = textureFile.height();
    glTexImage2D(side, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
  }
}

void CubeMap::Draw() {
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
  glBindVertexArray(vao);
  glDepthMask(false);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(true);
  glUseProgram(0);
}
