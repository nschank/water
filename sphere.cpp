#include "sphere.h"

Sphere::Sphere(GLuint shader, int res)
{
    // generate the vertex data
    GenVerts(res);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 6*m_total_verts*sizeof(GLfloat), m_verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));
    glEnableVertexAttribArray(glGetAttribLocation(shader, "normal"));

    glVertexAttribPointer(
                glGetAttribLocation(shader, "position"),
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat)*6,
                (void *)0
                );

    glVertexAttribPointer(
                glGetAttribLocation(shader, "normal"),
                3,
                GL_FLOAT,
                GL_TRUE,
                sizeof(GLfloat)*6,
                (void *)(sizeof(GLfloat)*3)
                );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



}

Sphere::~Sphere() {
    delete[] m_verts;
}

void Sphere::Draw(glm::mat4x4 mat, GLuint model) {
    //glBindVertexArray(m_vao);
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mat));
    glDrawArrays(GL_TRIANGLES, 0, m_total_verts);
    //glBindVertexArray(0);
}

void Sphere::GenVerts(int res) {

    m_verts = new float[36*res*res];

    int m = 0;
    for (int i=0; i < res; i++) {
        for (int j=0; j < res; j++) {
            float phi1 = M_PI*((float)i)/res;
            float phi2 = M_PI*(((float)i)+1)/res;
            float theta1 = 2*M_PI*((float)j)/res;
            float theta2 = 2*M_PI*(((float)j)+1)/res;
            //triangle 1
            //v1
            m_verts[m++] = 0.5*sin(phi2)*cos(theta2);
            m_verts[m++] = 0.5*cos(phi2);
            m_verts[m++] = 0.5*sin(phi2)*sin(theta2);
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            //v2
            m_verts[m++] = 0.5*sin(phi2)*cos(theta1);
            m_verts[m++] = 0.5*cos(phi2);
            m_verts[m++] = 0.5*sin(phi2)*sin(theta1);
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            //v3
            m_verts[m++] = 0.5*sin(phi1)*cos(theta1);
            m_verts[m++] = 0.5*cos(phi1);
            m_verts[m++] = 0.5*sin(phi1)*sin(theta1);
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            //triangle 2
            //v1
            m_verts[m++] = 0.5*sin(phi1)*cos(theta2);
            m_verts[m++] = 0.5*cos(phi1);
            m_verts[m++] = 0.5*sin(phi1)*sin(theta2);
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            //v2
            m_verts[m++] = 0.5*sin(phi2)*cos(theta2);
            m_verts[m++] = 0.5*cos(phi2);
            m_verts[m++] = 0.5*sin(phi2)*sin(theta2);
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            //v3
            m_verts[m++] = 0.5*sin(phi1)*cos(theta1);
            m_verts[m++] = 0.5*cos(phi1);
            m_verts[m++] = 0.5*sin(phi1)*sin(theta1);
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
            m_verts[m++] = m_verts[m-3]/0.5;
        }
    }
    m_total_verts = m/6;
}

