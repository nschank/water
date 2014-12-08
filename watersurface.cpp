#include "watersurface.h"

WaterSurface::WaterSurface(GLuint shader, int subdivs)
{
    m_subdivs = subdivs;

    m_h1 = new float[(subdivs+1)*(subdivs+1)];
    m_h2 = new float[(subdivs+1)*(subdivs+1)];

    m_verts = new float[18*subdivs*subdivs];

    InitializeHeights();

    GenVertsFromHeight();


    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3*m_total_verts*sizeof(GLfloat), m_verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));

    glVertexAttribPointer(
                glGetAttribLocation(shader, "position"),
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat)*3,
                (void *)0
                );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


WaterSurface::~WaterSurface() {
    delete[] m_h1;
    delete[] m_h2;
    delete[] m_verts;
}

void WaterSurface::Draw(glm::mat4x4 mat, GLuint model) {
    //glBindVertexArray(m_vao);
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mat));
    glDrawArrays(GL_TRIANGLES, 0, m_total_verts);
    //glBindVertexArray(0);
}

void WaterSurface::InitializeHeights() {
    int ind, i, j;
    for (i=0; i<m_subdivs+1; i++) {
        for (j=0; j<m_subdivs+1; j++) {
            ind = i*m_subdivs + j;
            m_h1[ind] = 0.0f;
            m_h2[ind] = 0.0f;
        }
    }
}

void WaterSurface::UpdateHeights() {
    int ind, i, j;
    float damping = 0.98;
    // iterate over non edge components
    for (i=1; i<m_subdivs; i++) {
        for (j=1; j<m_subdivs;j++) {
            m_h1[i*(m_subdivs+1) + j] = ((m_h2[(i+1)*(m_subdivs+1) + j] +
                                          m_h2[(i-1)*(m_subdivs+1) + j] +
                                          m_h2[i*(m_subdivs+1) + (j+1)] +
                                          m_h2[i*(m_subdivs+1) + (j-1)])/2.0 - m_h1[i*(m_subdivs+1) + j]) * damping;
        }
    }
    std::swap(m_h1, m_h2);
}

/*void WaterSurface::UpdateNormals() {
    float top, left, right, bottom, center;
    float cell_dist = 1.0/m_subdivs;
    for (int i=1; i<m_subdivs; i++) {
        for (j=1; j<m_subdivs; j++) {
           center = m_h2[i*(m_subdivs+1) + j];
           top = m_h2[(i-1)*(m_subdivs+1) + j];
           bottom = m_h2[(i+1)*(m_subdivs+1) + j];
           left = m_h2[i*(m_subdivs+1) + j-1];
           right = m_h2[i*(m_subdivs+1) + j+1];

           glm::vec3(-cell_dist)

           m_normals[i*(m_subdivs+1) + j] =


        }
    }
}*/

void WaterSurface::ApplyImpulses() {
    for (int i=0; i<m_impulses.size(); i++) {
        glm::vec3 impulse = m_impulses.at(i);
        ApplyImpulseRadius(impulse, 0.2);
    }
    m_impulses.clear();
}

void WaterSurface::ApplyImpulseRadius(glm::vec3 impulse, float rad) {
    int closest_x, closest_y;

    closest_x = glm::max(0, glm::min(m_subdivs, int(((impulse.x + 0.5) / 1.0)*m_subdivs)));
    closest_y = glm::max(0, glm::min(m_subdivs, int(((impulse.z + 0.5) / 1.0)*m_subdivs)));

    int min_x = glm::max(0, glm::min(m_subdivs, int(closest_x - rad))),
        max_x = glm::max(0, glm::min(m_subdivs, int(closest_x + rad))),
        min_y = glm::max(0, glm::min(m_subdivs, int(closest_y - rad))),
        max_y = glm::max(0, glm::min(m_subdivs, int(closest_y + rad)));



    for (int i=min_y; i<=max_y; i++) {
        for (int j=min_x; j<=max_x; j++) {
            m_h2[i*(m_subdivs+1) + j] = glm::min(m_h2[i*(m_subdivs+1)+j] + 0.5, 1.0);
        }
    }
}

void WaterSurface::AddImpulse(glm::vec3 impulse) {
    m_impulses.push_back(impulse);
}


void WaterSurface::GenVertsFromHeight() {
    float ss = 1.0/m_subdivs;
    int m = 0;
    float sx = -0.5;
    float sz = -0.5;

    float MAX_HEIGHT = 0.5f;

    for (int i=0; i < m_subdivs; i++) {
        for (int j=0; j < m_subdivs; j++) {
            // triangle 1
            // v1
            m_verts[m++] = sx + (ss * j);
            m_verts[m++] = glm::min(m_h2[i*(m_subdivs+1) + j], MAX_HEIGHT);
            m_verts[m++] = sz + (ss * i);
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;

            // v2
            m_verts[m++] = sx + (ss * (j+1));
            m_verts[m++] = glm::min(m_h2[i*(m_subdivs+1) + j+1], MAX_HEIGHT);
            m_verts[m++] = sz + (ss * i);
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
            // v3
            m_verts[m++] = sx + (ss * (j+1));
            m_verts[m++] = glm::min(m_h2[(i+1)*(m_subdivs+1) + j+1], MAX_HEIGHT);
            m_verts[m++] = sz + (ss * (i+1));
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
            // triangle 2
            // v4
            m_verts[m++] = sx + (ss * j);
            m_verts[m++] = glm::min(m_h2[i*(m_subdivs+1) + j], MAX_HEIGHT);
            m_verts[m++] = sz + (ss * i);
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
            // v5
            m_verts[m++] = sx + (ss * (j+1));
            m_verts[m++] = glm::min(m_h2[(i+1)*(m_subdivs+1) + j+1], MAX_HEIGHT);
            m_verts[m++] = sz + (ss * (i+1));
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;

            //v6
            m_verts[m++] = sx + (ss * j);
            m_verts[m++] = glm::min(m_h2[(i+1)*(m_subdivs+1) + j], MAX_HEIGHT);
            m_verts[m++] = sz + (ss * (i+1));
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
        }
    }
    m_total_verts = m/3;
    glBindVertexArray(m_vao);
    //glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3*m_total_verts*sizeof(GLfloat), m_verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


