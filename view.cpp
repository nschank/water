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


}

View::~View()
{
    delete m_sphere;
    delete m_camera;
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


    m_shader = ResourceLoader::loadShaders(
            "shaders/default.vert",
            "shaders/default.frag");

    m_uni["p"] = glGetUniformLocation(m_shader, "p");
    m_uni["m"] = glGetUniformLocation(m_shader, "m");
    m_uni["v"] = glGetUniformLocation(m_shader, "v");


    m_camera = new Camera(width(), height());

    m_sphere = new Sphere(m_shader, 10);
    m_water = new WaterSurface(m_shader, 100);
    m_water_transform = glm::translate(glm::vec3(1.0f, 0.0f, 0.0f));


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //m_spheres_pos.push_back(glm::mat4x4(1.0));
    //m_spheres_pos.push_back(glm::translate(glm::vec3(1.0f, 0.0f, 0.0f))*glm::mat4x4(1.0));

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

    glUseProgram(m_shader);

    glUniformMatrix4fv(m_uni["p"], 1, GL_FALSE, glm::value_ptr(m_camera->P()));
    glUniformMatrix4fv(m_uni["v"], 1, GL_FALSE, glm::value_ptr(m_camera->V()));


    // draw the m_spheres in their appropriate locations
    glBindVertexArray(m_sphere->m_vao);
    for (int i=0; i<m_spheres_pos.size(); i++)
        m_sphere->Draw(m_spheres_pos.at(i), m_uni["m"]);
    glBindVertexArray(0);

    m_water->ApplyImpulses();
    m_water->UpdateHeights();
    m_water->GenVertsFromHeight();
    glBindVertexArray(m_water->m_vao);
    m_water->Draw(m_water_transform, m_uni["m"]);
    glBindVertexArray(0);

    glUseProgram(0);

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
