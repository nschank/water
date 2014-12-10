#include "view.h"
#include <QApplication>
#include <QKeyEvent>

#include "lib/ResourceLoader.h"

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
    m_k_a = 0.2f;
    m_k_d = 0.8f;
    // ambient intensity
    m_i_a = glm::vec3(0.25, 0.25, 0.25);

    // object ambient color
    m_object_a = glm::vec3(1.0, 0.6, 0.6);
    // object diffuse color
    m_object_d = glm::vec3(1.0, 0.2, 0.2);

    // water ambient and diffuse colors
    m_water_a = glm::vec3(0.0, 0.8, 1.0);
    // object diffuse color
    m_water_d = glm::vec3(0.0, 0.6, 1.0);
}

View::~View()
{
    delete m_sphere;
    delete m_camera;
    delete cubeMap;
}

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

    m_sphere = new Sphere(m_object_shader, 30);
    m_water = new WaterSurface(m_water_shader, 100);
    m_water_transform = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_spheres_pos.push_back(glm::mat4x4(1.0));
    m_spheres_pos.push_back(glm::translate(glm::vec3(0.5f, 0.0f, 0.0f))*glm::mat4x4(1.0));

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
        fprintf(stderr, "(GL error code %d)\n", err);
    }

    m = glm::translate(glm::vec3(-1.0/100, 0.0, 0.0))*m;
    // Update the scene camera.
    glViewport(0, 0, width(), height());

    // Render the scene.

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
    for (int i=0; i<m_spheres_pos.size(); i++) {
      glm::mat3 normal_matrix = glm::mat3x3(glm::transpose(glm::inverse(m_spheres_pos.at(i))));
      glUniformMatrix3fv(glGetUniformLocation(m_object_shader, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(normal_matrix));
      m_sphere->Draw(m_spheres_pos.at(i), m_uni["m"]);
    }
    glBindVertexArray(0);
    //glUseProgram(0);


    /*glUseProgram(m_water_shader);
    glUniform1f(glGetUniformLocation(m_water_shader, "k_a"), m_k_a);
    glUniform1f(glGetUniformLocation(m_water_shader, "k_d"), m_k_d);
    glUniform3fv(glGetUniformLocation(m_water_shader, "water_a"), 1, glm::value_ptr(m_water_a));
    glUniform3fv(glGetUniformLocation(m_water_shader, "water_d"), 1, glm::value_ptr(m_water_d));
    glUniform3fv(glGetUniformLocation(m_water_shader, "ambient_intensity"), 1, glm::value_ptr(m_i_a));*/
    glUseProgram(0);
    
    
    /*glUseProgram(m_water_shader);
    glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "p"), 1, GL_FALSE, glm::value_ptr(m_camera->P()));
    glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "v"), 1, GL_FALSE, glm::value_ptr(m_camera->V()));
    glUniformMatrix4fv(glGetUniformLocation(m_water_shader, "m"), 1, GL_FALSE, glm::value_ptr(m_water_transform));
    m_water->ApplyImpulses();
    m_water->UpdateHeights();
    m_water->GenVertsFromHeight();
    glBindVertexArray(m_water->m_vao);
    m_water->Draw(m_water_transform, glGetUniformLocation(m_water_shader, "m"));
    glBindVertexArray(0);
    glUseProgram(0);*/
}


void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event)
{
    glm::vec3 hit;
    if (m_camera->CastRayAtObject(&hit, m_water_transform))
        m_water->AddImpulse(hit);
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
            m_water->AddImpulse(hit);
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

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
