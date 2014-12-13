#include "cubemap.h"
#include <stdio.h>

const char *CubeMap::topFilename = TOP_IMG;
const char *CubeMap::bottomFilename = BOTTOM_IMG;
const char *CubeMap::leftFilename = LEFT_IMG;
const char *CubeMap::rightFilename = RIGHT_IMG;
const char *CubeMap::frontFilename = FRONT_IMG;
const char *CubeMap::backFilename = BACK_IMG;

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
  
  loadSide(topFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
  loadSide(bottomFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
  loadSide(leftFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
  loadSide(rightFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
  loadSide(frontFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
  loadSide(backFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
  
  GLint vertex = glGetAttribLocation(cubeTexture, "vertex");
  glEnableVertexAttribArray(vertex);
  glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

  float size = 0.5f;

  GLfloat skyboxVertices[] = {
    // Positions          
    -size,  size, -size,
    -size, -size, -size,
     size, -size, -size,
     size, -size, -size,
     size,  size, -size,
    -size,  size, -size,

    -size, -size,  size,
    -size, -size, -size,
    -size,  size, -size,
    -size,  size, -size,
    -size,  size,  size,
    -size, -size,  size,

     size, -size, -size,
     size, -size,  size,
     size,  size,  size,
     size,  size,  size,
     size,  size, -size,
     size, -size, -size,

    -size, -size,  size,
    -size,  size,  size,
     size,  size,  size,
     size,  size,  size,
     size, -size,  size,
    -size, -size,  size,

    -size,  size, -size,
     size,  size, -size,
     size,  size,  size,
     size,  size,  size,
    -size,  size,  size,
    -size,  size, -size,

    -size, -size, -size,
    -size, -size,  size,
     size, -size, -size,
     size, -size, -size,
    -size, -size,  size,
     size, -size,  size
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
  glDeleteTextures(1, &cubeTexture);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void CubeMap::loadSide(const char* filename, GLenum side) {
  QImage textureImageFile = QImage(QString(filename));
  if(!textureImageFile.isNull()) {
    QImage textureFile = QGLWidget::convertToGLFormat(textureImageFile);
    uchar* bits = textureFile.bits();
    int w = textureFile.width(),
        h = textureFile.height();
    glTexImage2D(side, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
  }
  else std::cout << "null image" << std::endl;
}

void CubeMap::draw() {
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glUseProgram(cubemapShader);
  glUniformMatrix4fv(glGetUniformLocation(cubemapShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4x4(1.0)));
  glUniformMatrix4fv(glGetUniformLocation(cubemapShader, "view"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(cubemapShader, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
  glBindVertexArray(vao);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(cubemapShader, "skybox"), 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
}

void CubeMap::setShaderSamplerCube(GLuint otherShader, char *propertyName) {
  glUniform1i(glGetUniformLocation(otherShader, propertyName), 0);
}
