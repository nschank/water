#include "cubemap.h"
#include <stdio.h>

const char *CubeMap::filename = "clouds.jpg";

CubeMap::CubeMap(Camera* cam) {
  camera = cam;


  cubemapShader = ResourceLoader::loadShaders(
          "shaders/cubemap.vert",
          "shaders/cubemap.frag");
  
  glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &cubeTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
  
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  QImage textureFile = QImage(QString(filename));
  if(textureFile.isNull()) {
    return;
  }
  
  uchar* bits = textureFile.bits();
  int w = textureFile.width(),
      h = textureFile.height();

  /*loadSide(bits, w, w/4, w/2, 0, h/3, GL_TEXTURE_CUBE_MAP_POSITIVE_Y); // top
  loadSide(bits, w, 0, w/4, h/3, 2*h/3, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y); // bottom
  loadSide(bits, w, w/4, w/2, h/3, 2*h/3, GL_TEXTURE_CUBE_MAP_NEGATIVE_X); // left
  loadSide(bits, w, w/2, 3*w/4, h/3, 2*h/3, GL_TEXTURE_CUBE_MAP_POSITIVE_X); // right
  loadSide(bits, w, 3*w/4, w, h/3, 2*h/3, GL_TEXTURE_CUBE_MAP_POSITIVE_Z); // back
  loadSide(bits, w, w/4, w/2, 2*h/3, h, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z); // front*/
  loadSide(bits, w, 0, w, 0, h, GL_TEXTURE_CUBE_MAP_POSITIVE_Y); // top
  loadSide(bits, w, 0, w, 0, h, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y); // bottom
  loadSide(bits, w, 0, w, 0, h, GL_TEXTURE_CUBE_MAP_NEGATIVE_X); // left
  loadSide(bits, w, 0, w, 0, h, GL_TEXTURE_CUBE_MAP_POSITIVE_X); // right
  loadSide(bits, w, 0, w, 0, h, GL_TEXTURE_CUBE_MAP_POSITIVE_Z); // back
  loadSide(bits, w, 0, w, 0, h, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z); // front

  GLint vertex = glGetAttribLocation(cubeTexture, "vertex");
  glEnableVertexAttribArray(vertex);
  glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);



  GLfloat skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
  };
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glBindVertexArray(0);
}

CubeMap::~CubeMap() {
  //glDeleteTextures(1, &cubeTexture);
}

void CubeMap::loadSide(uchar* bits, int width, int startX, int endX, int startY, int endY, GLenum side) {
  int sideWidth = endX - startX,
      sideHeight = endY - startY;
  uchar *sideBits = new uchar[sideWidth*sideHeight];
  for(int y=startY; y<endY; y++) {
    for(int x=startX; x<endX; x++) {
      int bitsIndex = x + y*width;
      int sideIndex = x - startX + sideWidth*(y - startY);
     // printf("bitsIndex %d, sideIndex %d, sideWidth*sideHeight %d\n", bitsIndex, sideIndex, sideWidth*sideHeight);
      sideBits[sideIndex] = bits[bitsIndex];
    }
  }
  printf("W %d, h %d\n", sideWidth, sideHeight);
  glTexImage2D(side, 0, GL_RGBA, sideWidth, sideHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sideBits);
  //delete[] sideBits;
}

void CubeMap::draw() {
  glDepthMask(GL_FALSE);
  glUseProgram(cubemapShader);
  glUniformMatrix4fv(glGetUniformLocation(cubemapShader, "view"), 1, GL_FALSE, glm::value_ptr(camera->V()));
  glUniformMatrix4fv(glGetUniformLocation(cubemapShader, "projection"), 1, GL_FALSE, glm::value_ptr(camera->P()));
  glBindVertexArray(vao);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(cubemapShader, "skybox"), 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthMask(GL_TRUE);
}
