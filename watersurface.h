#ifndef WATERSURFACE_H
#define WATERSURFACE_H

#include "common.h"
#include "entity.h"

class WaterSurface : public Entity
{
friend class SphereEntity;

public:
    WaterSurface(GLuint shader, int subdivs);

    virtual ~WaterSurface();

    void GenVertsFromHeight();

    void InitializeHeights();

    void UpdateHeights();

    //void UpdateNormals();

    void ApplyImpulses();

    void ApplyImpulseRadius(glm::vec3 impulse, float rad);

    void AddImpulse(glm::vec3 impulse);

    void Draw(glm::mat4x4 mat, GLuint model);

    GLuint m_vao, m_vbo;

    std::vector<glm::vec3> m_impulses;

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

protected:
	void collideWithSphere(SphereEntity *other);
	void collideWithSurface(WaterSurface *other);

	float heightAt(float x, float y);
	float getXResolution();
	float getYResolution();

	float getMaxHeight();

private:
    float *m_verts;
    int m_total_verts, m_subdivs;

    float *m_vel, *m_height;

    glm::vec3 *m_normals;



};

#endif // WATERSURFACE_H
