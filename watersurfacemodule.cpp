/*#include "watersurfacemodule.h"

WaterSurfaceModule::WaterSurfaceModule()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    m_width = 10;
    m_verts = new GLfloat[m_width * m_width * 6 * 6];
    height_map = new float[m_width * m_width];
}

WaterSurfaceModule::~WaterSurfaceModule() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_vao);
    delete[] m_verts;
    delete[] m_height_map;
}


void WaterSurfaceModule::AddImpulse(glm::vec2 point, glm::vec3 dir) {
    m_impulses.push_back(new tuple<glm::vec2, glm::vec3>(point, dir));
}

void WaterSurfaceModule::RecomputeSurface() {
    // compute the
    int m = 0;
    float sx = 0.0, sy = 0.0, sz = 0.0;
    float ss = 1.0/m_width;
    for (int i=0; i<m_width; i++) {
        for (int j=0; j<m_width; j++) {
            m_verts[m++] = sx + (ss * j);
            m_verts[m++] = sy;
            m_verts[m++] = sz + (ss * i);
            m_verts[m++] = 0.0;
            m_verts[m++] = 1.0;
            m_verts[m++] = 0.0;
            // v2
            m_verts[m++] = sx + (ss * (j+1));
            m_verts[m++] = sy;
            m_verts[m++] = sz + (ss * i);
            m_verts[m++] = 0.0;
            m_verts[m++] = 1.0;
            m_verts[m++] = 0.0;
            // v3
            m_verts[m++] = sx + (ss * (j+1));
            m_verts[m++] = sy;
            m_verts[m++] = sz + (ss * (i+1));
            m_verts[m++] = 0.0;
            m_verts[m++] = 1.0;
            m_verts[m++] = 0.0;
            // v4
            m_verts[m++] = sx + (ss * j);
            m_verts[m++] = sy;
            m_verts[m++] = sz + (ss * i);
            m_verts[m++] = 0.0;
            m_verts[m++] = 1.0;
            m_verts[m++] = 0.0;
            // v5
            m_verts[m++] = sx + (ss * (j+1));
            m_verts[m++] = sy;
            m_verts[m++] = sz + (ss * (i+1));
            m_verts[m++] = 0.0;
            m_verts[m++] = 1.0;
            m_verts[m++] = 0.0;
            // v6
            m_verts[m++] = sx + (ss * j);
            m_verts[m++] = sy;
            m_verts[m++] = sz + (ss * (i+1));
            m_verts[m++] = 0.0;
            m_verts[m++] = 1.0;
            m_verts[m++] = 0.0;
        }
    }

    // clear the impulses at the end of the recomputation
    m_impulses.clear();
}

void WaterSurfaceModule::RenderSurface() {
     glBindVertexArray(m_vao);
     glDrawArrays(GL_TRIANGLES, 0, m_width * m_height * 6);
     glBindVertexArray(0);
}*/
