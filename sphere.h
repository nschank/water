#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"

class Sphere
{
public:
    Sphere(GLuint shader, int res);
    virtual ~Sphere();

    void Draw(glm::mat4x4 mat, GLuint model);

    GLuint m_shader, m_vao, m_vbo;

    float *m_verts;

private:

    int m_total_verts;
    void GenVerts(int res);


};

#endif // SPHERE_H
