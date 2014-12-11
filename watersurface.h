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
	void post();

    //void UpdateNormals();

	void setPoints(bool clear);
	void ApplyImpulses(float secondsSinceLastTick);

	void setPoint(glm::vec2 discretePoint, float height);

	glm::vec2 closestDiscretePoint(glm::vec3 continuousPoint);

    void Draw(glm::mat4x4 mat, GLuint model);

    GLuint m_vao, m_vbo;

    std::vector<glm::vec3> m_impulses;
	std::vector<glm::vec3> m_setPoints;

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void tick(float secondsSinceLastTick);

protected:
	void collideWithSphere(SphereEntity *other);
	void collideWithSurface(WaterSurface *other);

	float& heightAt(glm::vec2 discretePoint);
	float& velocityAt(glm::vec2 discretePoint);
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
