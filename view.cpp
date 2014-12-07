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

    p = glm::perspective(55.0f, 1.0f, 0.01f, 10.0f);
    v = glm::lookAt(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    m = glm::mat4(1.0f);
}

View::~View()
{
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    glGetError(); // Clear errors after call to glewInit
	if(GLEW_OK != err)
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


    m_data = new float[3*3];

    int inv = 1;

    m_data[0 + 6*inv] = 0; m_data[1 + 6*inv] = 0; m_data[2 + 6*inv] = 0;
    m_data[3] = 1; m_data[4] = 0; m_data[5] = 0;
    m_data[6 - 6*inv] = 1; m_data[7 - 6*inv] = 0; m_data[8 - 6*inv] = 1;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), m_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(glGetAttribLocation(m_shader, "position"));

    glVertexAttribPointer(
                glGetAttribLocation(m_shader, "position"),
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat)*3,
                (void *)0
                );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
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

    glUniformMatrix4fv(m_uni["p"], 1, GL_FALSE, glm::value_ptr(p));
    glUniformMatrix4fv(m_uni["v"], 1, GL_FALSE, glm::value_ptr(v));
    glUniformMatrix4fv(m_uni["m"], 1, GL_FALSE, glm::value_ptr(m));

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);


    glUseProgram(0);

}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event)
{
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
