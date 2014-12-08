#ifndef WATERSURFACE_H
#define WATERSURFACE_H

#include "common.h"

class WaterSurface
{
public:
    WaterSurface(GLuint shader, int subdivs);

    virtual ~WaterSurface();

    void GenVertsFromHeight();

    void InitializeHeights();

    void UpdateHeights();

    void ApplyImpulses();

    void ApplyImpulseRadius(glm::vec3 impulse, float rad);

    void AddImpulse(glm::vec3 impulse);

    void Draw(glm::mat4x4 mat, GLuint model);

    GLuint m_vao, m_vbo;

    std::vector<glm::vec3> m_impulses;


private:
    float *m_verts;
    int m_total_verts, m_subdivs;

    float *m_h1, *m_h2;



};

#endif // WATERSURFACE_H
