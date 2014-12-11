#include "watersurface.h"
#include "sphereentity.h"

WaterSurface::WaterSurface(GLuint shader, int subdivs)
{
    m_subdivs = subdivs;

    m_vel = new float[(subdivs+1)*(subdivs+1)];
    m_height = new float[(subdivs+1)*(subdivs+1)];

    m_verts = new float[18*subdivs*subdivs];

    InitializeHeights();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

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

	GenVertsFromHeight();
}


WaterSurface::~WaterSurface() {
    delete[] m_vel;
    delete[] m_height;
    delete[] m_verts;
}

void WaterSurface::Draw(glm::mat4x4 mat, GLuint model) {
	GenVertsFromHeight();

	glBindVertexArray(m_vao);
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(mat));
    glDrawArrays(GL_TRIANGLES, 0, m_total_verts);
	glBindVertexArray(0);
}

void WaterSurface::InitializeHeights() {
    int ind, i, j;
    for (i=0; i<m_subdivs+1; i++) {
        for (j=0; j<m_subdivs+1; j++) {
            ind = i*(m_subdivs+1) + j;
            m_vel[ind] = 0.0f;
            m_height[ind] = 0.0f;
        }
    }
}


// really update velocities
void WaterSurface::UpdateHeights() {
	// iterate over non edge components
	for(int i=1; i<m_subdivs; i++) {
		for(int j=1; j<m_subdivs;j++) {
			m_vel[i*(m_subdivs+1) + j] = glm::clamp(((m_height[(i+1)*(m_subdivs+1) + j] + m_height[(i-1)*(m_subdivs+1) + j] +
					m_height[i*(m_subdivs+1) + (j+1)] + m_height[i*(m_subdivs+1) + (j-1)])/VELOCITY_AVERAGE_FACTOR
					- m_vel[i*(m_subdivs+1) + j]) * VELOCITY_DAMPING_FACTOR, MINIMUM_VELOCITY, MAXIMUM_VELOCITY);
			//m_vel[i*(m_subdivs+1) + j] = glm::min(0.5f, m_h1[i*(m_subdivs+1) + j]);
		}
	}
	std::swap(m_vel, m_height);
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

void WaterSurface::ApplyImpulses(float secondsSinceLastTick)
{
	for (int i=0; i<m_impulses.size(); i++)
	{
		glm::vec3 impulse = m_impulses.at(i);
		velocityAt(glm::vec2(impulse.x, impulse.z)) += impulse.y;//*secondsSinceLastTick;
		std::cout << impulse.y << std::endl;
	}
	m_impulses.clear();
}

void WaterSurface::setPoints(bool clear)
{
	for(int i = 0; i < m_setPoints.size(); i++)
	{
		glm::vec3 point = m_setPoints.at(i);
		m_setPoints.at(i).y = heightAt(glm::vec2(point.x,point.z));
		heightAt(glm::vec2(point.x,point.z)) = point.y;
	}
	if(clear)
		m_setPoints.clear();
}

void WaterSurface::setPoint(glm::vec2 discretePoint, float height)
{
	m_setPoints.push_back(glm::vec3(discretePoint.x,height,discretePoint.y));
}

glm::vec2 WaterSurface::closestDiscretePoint(glm::vec3 continuousPoint)
{
	int closest_x = glm::max(1, glm::min(m_subdivs-1, (int)glm::round((((continuousPoint.x + 0.5) / 1.0)*m_subdivs))));
	int closest_y = glm::max(1, glm::min(m_subdivs-1, (int)glm::round((((continuousPoint.z + 0.5) / 1.0)*m_subdivs))));

	return glm::vec2(closest_x, closest_y);
}



void WaterSurface::GenVertsFromHeight() {
    float ss = 1.0/m_subdivs;
    int m = 0;
    float sx = -0.5;
    float sz = -0.5;

    for (int i=0; i < m_subdivs; i++) {
        for (int j=0; j < m_subdivs; j++) {
            // triangle 1
            // v1
            m_verts[m++] = sx + (ss * j);
			m_verts[m++] = m_height[i*(m_subdivs+1) + j];
            m_verts[m++] = sz + (ss * i);
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;

            // v2
            m_verts[m++] = sx + (ss * (j+1));
			m_verts[m++] = m_height[i*(m_subdivs+1) + j+1];
            m_verts[m++] = sz + (ss * i);
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
            // v3
            m_verts[m++] = sx + (ss * (j+1));
			m_verts[m++] = m_height[(i+1)*(m_subdivs+1) + j+1];
            m_verts[m++] = sz + (ss * (i+1));
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
            // triangle 2
            // v4
            m_verts[m++] = sx + (ss * j);
			m_verts[m++] = m_height[i*(m_subdivs+1) + j];
            m_verts[m++] = sz + (ss * i);
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
            // v5
            m_verts[m++] = sx + (ss * (j+1));
			m_verts[m++] = m_height[(i+1)*(m_subdivs+1) + j+1];
            m_verts[m++] = sz + (ss * (i+1));
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;

            //v6
            m_verts[m++] = sx + (ss * j);
			m_verts[m++] = m_height[(i+1)*(m_subdivs+1) + j];
            m_verts[m++] = sz + (ss * (i+1));
            //m_verts[m++] = nx;
            //m_verts[m++] = ny;
            //m_verts[m++] = nz;
        }
    }
    m_total_verts = m/3;
	glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3*m_total_verts*sizeof(GLfloat), m_verts, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void WaterSurface::applyTranslationAt(glm::vec3 translation, glm::vec3 location)
{

}

void WaterSurface::applyImpulseAt(glm::vec3 impulse, glm::vec3 location)
{
    m_impulses.push_back(impulse + location);
}

void WaterSurface::applyForceAt(glm::vec3 force, glm::vec3 location)
{
//Unused
}

void WaterSurface::tick(float secondsSinceLastTick)
{
	setPoints(true);
	ApplyImpulses(secondsSinceLastTick);
	UpdateHeights();
}

void WaterSurface::collideWith(Entity *other)
{
	other->collideWithSurface(this);
}


void WaterSurface::collideWithSphere(SphereEntity *other)
{
	other->collideWithSurface(this);
}

void WaterSurface::collideWithSurface(WaterSurface *other)
{
	//does nothing
}

float& WaterSurface::heightAt(glm::vec2 discretePoint)
{
	return m_height[(int)glm::round(discretePoint.y)*(m_subdivs+1) + (int)glm::round(discretePoint.x)];
}

float& WaterSurface::velocityAt(glm::vec2 discretePoint)
{
	return m_vel[(int)glm::round(discretePoint.y)*(m_subdivs+1) + (int)glm::round(discretePoint.x)];
}

float WaterSurface::getResolution()
{
	return 1/float(m_subdivs);
}

void WaterSurface::post()
{
	setPoints(false);
}
