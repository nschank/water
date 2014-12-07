#ifndef WATERSURFACEMODULE_H
#define WATERSURFACEMODULE_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

class WaterSurfaceModule
{   
public:
    WaterSurfaceModule();

    ~WaterSurfaceModule();

    void RecomputeSurface();

    void AddImpulse(glm::vec2 point, glm::vec3 dir);

    void RenderSurface();

private:
    GLuint m_vao, m_vbo;
    GLfloat *m_verts;

    float *height_map;
    int m_width;
    std::vector<std::tuple<glm::vec2, glm::vec3>> m_impulses;
};

#endif // WATERSURFACEMODULE_H
