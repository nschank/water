#include "view.h"
#include <QApplication>
#include <QKeyEvent>

#include "lib/ResourceLoader.h"
#include "sphereentity.h"
#include "settings.h"
#include <iostream>

View::View(QWidget *parent) : QGLWidget(parent)
{
  // View needs all mouse move events, not just mouse drag events
  setMouseTracking(true);

  // Hide the cursor since this is a fullscreen app
  setCursor(Qt::BlankCursor);

  // View needs keyboard focus
  setFocusPolicy(Qt::StrongFocus);


  // The game loop is implemented using a timer
  connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

  // ambient and diffuse coefficients
  m_k_a = GLOBAL_AMBIENT_COEFFICIENT;
  m_k_d = GLOBAL_DIFFUSE_COEFFICIENT;
  
  // ambient intensity
  m_i_a = GLOBAL_AMBIENT_INTENSITY;

  // object ambient color
  m_object_a = SPHERE_AMBIENT_COLOR;
  // object diffuse color
  m_object_d = SPHERE_DIFFUSE_COLOR;

  // water ambient and diffuse colors
  m_water_a = WATER_AMBIENT_COLOR;
  // water diffuse color
  m_water_d = WATER_DIFFUSE_COLOR;

  //create a World for Entities to live in
  m_world = new World();

  m_balls = 0;
}


View::~View()
{
  delete m_sphere;
  delete m_camera;
  delete cubeMap;
	delete m_world;
	delete m_water;

	for(std::vector<SphereEntity *>::iterator it = m_sphere_entities.begin(); it != m_sphere_entities.end(); it++)
		delete (*it);

  glDeleteShader(m_water_shader);
  glDeleteShader(m_object_shader);
}
//plz
void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    glGetError(); // Clear errors after call to glewInit
    if (GLEW_OK != err)
    {
      // Problem: glewInit failed, something is seriously wrong.
      fprintf(stderr, "Error initializing glew: %s\n", glewGetErrorString(err));
    }

	  m_object_shader = ResourceLoader::loadShaders(
              "shaders/default.vert",
              "shaders/default.frag");

    m_water_shader = ResourceLoader::loadShaders(
              "shaders/water.vert",
              "shaders/water.frag");


    m_uni["p"] = glGetUniformLocation(m_object_shader, "p");
    m_uni["m"] = glGetUniformLocation(m_object_shader, "m");
    m_uni["v"] = glGetUniformLocation(m_object_shader, "v");

    m_camera = new Camera(width(), height());

	m_sphere = new Sphere(m_object_shader, SPHERE_RESOLUTION);
	m_water = new WaterSurface(m_water_shader, WATER_RESOLUTION);
	m_world->addEntity(m_water);
	m_water_transform = glm::translate(glm::vec3(0.0f, WATER_PLANE_HEIGHT, 0.0f));

	  glEnable(GL_CULL_FACE);
	  if(POLYGON_MODE)
		  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	  glEnable(GL_DEPTH_TEST);

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    timer.start(1000 / 60);

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

	cubeMap = new CubeMap(m_camera);
}

void View::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // TODO: Implement the demo rendering here
  // Check for errors from the last frame.
  int err;
  if ((err = glGetError()) != GL_NO_ERROR) {
      fprintf(stderr, "GL is in an error state before painting.\n");
      //fprintf(stderr, "(GL error code %s)\n", err);
  }

  m = glm::translate(glm::vec3(-1.0/100, 0.0, 0.0))*m;
  // Update the scene camera.
  glViewport(0, 0, width(), height());

  // Render the scene.
  if(USE_CUBE_MAP)
	  cubeMap->draw();

  glUseProgram(m_object_shader);

  glUniformMatrix4fv(m_uni["p"], 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMatrix()));
  glUniformMatrix4fv(m_uni["v"], 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
  
  glUniform1f(glGetUniformLocation(m_object_shader, "k_a"), m_k_a);
  glUniform1f(glGetUniformLocation(m_object_shader, "k_d"), m_k_d);
  glUniform3fv(glGetUniformLocation(m_object_shader, "object_a"), 1, glm::value_ptr(m_object_a));
  glUniform3fv(glGetUniformLocation(m_object_shader, "object_d"), 1, glm::value_ptr(m_object_d));
  glUniform3fv(glGetUniformLocation(m_object_shader, "ambient_intensity"), 1, glm::value_ptr(m_i_a));

  // draw the m_spheres in their appropriate locations
  glBindVertexArray(m_sphere->m_vao);

  for(int i=0; i < m_sphere_entities.size(); i++) {
	  SphereEntity *current = m_sphere_entities.at(i);
	  glUniformMatrix3fv(glGetUniformLocation(m_object_shader, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(current->normalMatrix()));
	  m_sphere->Draw(current->modelMatrix(), m_uni["m"]);
  }
  glBindVertexArray(0);
  glUseProgram(0);


  glUseProgram(m_water_shader);
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "p"), 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "v"), 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
  //glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "m"), 1, GL_FALSE, glm::value_ptr(m_water_transform));
  glUniform1f(glGetUniformLocation(m_water_shader, "k_a"), m_k_a);
  glUniform1f(glGetUniformLocation(m_water_shader, "k_d"), m_k_d);
  glUniform3fv(glGetUniformLocation(m_water_shader, "water_a"), 1, glm::value_ptr(m_water_a));
  glUniform3fv(glGetUniformLocation(m_water_shader, "water_d"), 1, glm::value_ptr(m_water_d));
  glUniform3fv(glGetUniformLocation(m_water_shader, "ambient_intensity"), 1, glm::value_ptr(m_i_a));
  glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3x3(glm::transpose(glm::inverse(m_water_transform)))));
  glUniform3fv(glGetUniformLocation(m_water_shader, "cameraPosition"), 1, glm::value_ptr(glm::vec3(m_camera->getEye())));
  glUniform3fv(glGetUniformLocation(m_water_shader, "object_a"), 1, glm::value_ptr(m_object_a));
  glUniform3fv(glGetUniformLocation(m_water_shader, "object_d"), 1, glm::value_ptr(m_object_d));


  int ballCount = m_sphere_entities.size();
  glUniform1f(glGetUniformLocation(m_water_shader, "ballCount"), ballCount);
  glm::mat4x4 *m_sphere_model_matrices = new glm::mat4x4[ballCount];
  glm::vec3 *m_sphere_pos = new glm::vec3[ballCount];
  glm::mat3x3 *m_sphere_normal_matrices = new glm::mat3x3[ballCount];
  for(int i=0; i<ballCount; i++) {
    SphereEntity *current = m_sphere_entities.at(i);
    m_sphere_normal_matrices[i] = current->normalMatrix();
    m_sphere_model_matrices[i] = current->modelMatrix();
    m_sphere_pos[i] = current->m_center;
  }
  if(ballCount > 0) {
  glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "ballNormalMatrices0"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(0)->normalMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "ballModels0"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(0)->modelMatrix()));
  }
  if(ballCount > 1) {
  glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "ballNormalMatrices0"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(0)->normalMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "ballModels0"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(0)->modelMatrix()));
  glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "ballNormalMatrices1"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(1)->normalMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "ballModels1"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(1)->modelMatrix()));
  }
    if(ballCount > 2) {
  glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "ballNormalMatrices0"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(0)->normalMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "ballModels0"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(0)->modelMatrix()));
  glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "ballNormalMatrices1"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(1)->normalMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "ballModels1"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(1)->modelMatrix()));
    glUniformMatrix3fv(glGetUniformLocation(m_water_shader, "ballNormalMatrices2"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(2)->normalMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "ballModels2"), 1, GL_FALSE, glm::value_ptr(m_sphere_entities.at(2)->modelMatrix()));
  }


  m_water->Draw(m_water_transform, glGetUniformLocation(m_water_shader, "m"));
  glUseProgram(0);
}


void View::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event)
{
    // affect the water
    if (event->button() == Qt::LeftButton) {
        glm::vec3 hit;
        if (m_camera->CastRayAtObject(&hit, m_water_transform))
        {
            glm::vec2 discreteLocation = m_water->closestDiscretePoint(hit);
            m_water->applyImpulseAt(CLICK_IMPULSE,
                                    glm::vec3(discreteLocation.x, 0.0f, discreteLocation.y));
        }
    }
    // throw a sphere in the direction you are looking
	if(event->button() == Qt::RightButton && m_balls < MAX_BALLS) {
        float force = 0.3f;
		addSphere(glm::vec3(m_camera->eye), .05, -force * m_camera->w, .5f*MAX_BUOYANCY*m_balls/float(MAX_BALLS)+1.f);
		m_balls++;
	}
}

void View::mouseMoveEvent(QMouseEvent *event)
{
  // This starter code implements mouse capture, which gives the change in
  // mouse position since the last mouse movement. The mouse needs to be
  // recentered after every movement because it might otherwise run into
  // the edge of the screen, which would stop the user from moving further
  // in that direction. Note that it is important to check that deltaX and
  // deltaY are not zero before recentering the mouse, otherwise there will
  // be an infinite loop of mouse move events.
  int deltaX = event->x() - width() / 2;
  int deltaY = event->y() - height() / 2;
  if (!deltaX && !deltaY) return;
  QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

  // TODO: Handle mouse movements here
  m_camera->MouseMoved(deltaX, deltaY);
  if (event->buttons() == Qt::LeftButton) {
    glm::vec3 hit;
	  if (m_camera->CastRayAtObject(&hit, m_water_transform))
	  {
		  glm::vec2 discreteLocation = m_water->closestDiscretePoint(hit);
		  m_water->applyImpulseAt(CLICK_IMPULSE,
								  glm::vec3(discreteLocation.x, 0.0f, discreteLocation.y));
		  m_water->applyImpulseAt(CLICK_IMPULSE/2.f,
								  glm::vec3(discreteLocation.x-1, 0.0f, discreteLocation.y-1));
		  m_water->applyImpulseAt(CLICK_IMPULSE/2.f,
								  glm::vec3(discreteLocation.x-1, 0.0f, discreteLocation.y+1));
		  m_water->applyImpulseAt(CLICK_IMPULSE/2.f,
								  glm::vec3(discreteLocation.x+1, 0.0f, discreteLocation.y-1));
		  m_water->applyImpulseAt(CLICK_IMPULSE/2.f,
								  glm::vec3(discreteLocation.x+1, 0.0f, discreteLocation.y+1));
	  }
  }
}

void View::wheelEvent(QWheelEvent *event) {
  int units = event->angleDelta().y();
  QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
  m_camera->MouseScrolled(units);
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
}

void View::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape) QApplication::quit();

	if(KEYPRESS_FOR_TICK)
		m_world->tick(DEFAULT_TICK_LENGTH);
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
  // Get the number of seconds since the last tick (variable update rate)
  float seconds = time.restart() * 0.001f;

	for(std::vector<SphereEntity *>::iterator it = m_sphere_entities.begin(); it != m_sphere_entities.end(); it++)
		(*it)->applyForceAt(GRAVITY * (*it)->m_mass, glm::vec3());

	if(!KEYPRESS_FOR_TICK)
		m_world->tick(seconds);

  // Flag this view for repainting (Qt will call paintGL() soon after)
  update();
}

void View::addSphere(glm::vec3 worldLocation, float radius, glm::vec3 velocity, float buoyancy)
{
	SphereEntity *temp = new SphereEntity(worldLocation, radius);

	temp->m_velocity = velocity;
	temp->m_buoyancy = buoyancy;
	m_sphere_entities.push_back(temp);
	m_world->addEntity(temp);
}
