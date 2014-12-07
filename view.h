#ifndef VIEW_H
#define VIEW_H
#include "GL/glew.h"

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "CamtransCamera.h"
#include <string>
#include <map>

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();
    CamtransCamera m_camera;
    GLuint m_shader;
    std::map<std::string, GLint> m_uni;
    GLuint m_vao, m_vbo;
    float *m_data;
    glm::mat4x4 p, v, m;

private:
    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void tick();
};

#endif // VIEW_H

