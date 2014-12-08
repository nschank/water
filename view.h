#ifndef VIEW_H
#define VIEW_H

#include "common.h"

#include "sphere.h"
#include "camera.h"
#include "watersurface.h"

#include <QTime>
#include <QTimer>
#include <string>
#include <map>

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

    GLuint m_shader;
    std::map<std::string, GLint> m_uni;
    std::vector<glm::mat4x4> m_spheres_pos;
    Sphere *m_sphere;
    Camera *m_camera;

    WaterSurface *m_water;
    glm::mat4x4 m_water_transform;

    GLuint m_vao, m_vbo;
    float *m_data;
    glm::mat4x4 p, v, m;

private:
    QTime time;
    QTimer timer;

    float m_k_a;
    float m_k_d;

    glm::vec3 m_O_a;
    glm::vec3 m_O_d;
    glm::vec3 m_i_a;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);



private slots:
    void tick();
};

#endif // VIEW_H

