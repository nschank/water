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

    void Draw(glm::mat4x4 mat, GLuint model);

	void collideWith(Entity *other);

	void applyTranslationAt(glm::vec3 translation, glm::vec3 location);
	void applyImpulseAt(glm::vec3 impulse, glm::vec3 location);
	void applyForceAt(glm::vec3 force, glm::vec3 location);
	void setPoint(glm::vec2 discretePoint, float height);

	void tick(float secondsSinceLastTick);
	void post();

	glm::vec2 closestDiscretePoint(glm::vec3 continuousPoint);
	float& heightAt(glm::vec2 discretePoint);
	float& velocityAt(glm::vec2 discretePoint);

protected:
	void collideWithSphere(SphereEntity *other);
	void collideWithSurface(WaterSurface *other);

	float getResolution();

	void ApplyImpulses(float secondsSinceLastTick);
	void setPoints(bool clear);
	void UpdateHeights();

	//void UpdateNormals();

	void GenVertsFromHeight();
	void InitializeHeights();

private:
    float *m_verts;
    int m_total_verts, m_subdivs;

	glm::vec3 *m_normals;
	std::vector<glm::vec3> m_impulses;
	std::vector<glm::vec3> m_setPoints;

	float *m_vel, *m_height;
	GLuint m_vao, m_vbo;
};

#endif // WATERSURFACE_H
