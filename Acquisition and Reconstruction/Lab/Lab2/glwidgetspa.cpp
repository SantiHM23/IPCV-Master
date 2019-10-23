#include <QFile>
#include <QString>
#include <QTransform>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

QPoint lastPos;
GLfloat rotX = 0, rotY = 0, rotZ = 0;
GLfloat movX = 0, movY = 0;
GLfloat zoomFactor = 1.0;
bool shiftFlag = false;

GLWidget::~GLWidget() {}

void GLWidget::loadFaces(const QString &path) {
  QFile json_file(path);
  if (!json_file.open(QIODevice::ReadOnly)) {
    qWarning("Failed to open file");
    exit(-1);
  }

  QByteArray json_data = json_file.readAll();
  QJsonDocument json_document(QJsonDocument::fromJson(json_data));
  face_collection.fromJson(json_document.array());

  // TODO: update the display inside the widget
}

void GLWidget::initializeGL() {
  // TODO: initialize the OpenGL to display 3d data
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::paintGL() {
  QSize viewport_size = size();
  glViewport(0, 0, viewport_size.width(), viewport_size.height());

  glMatrixMode(GL_PROJECTION);
  // TODO: define the point of view
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  //gluPerspective(atan(tan(50.0 * 3.14159 / 360.0) / zoomFactor) * 360.0 / 3.141593, 1.0, 1.0, 17.0);
  glTranslatef(movX, movY, 0);
  glScalef(zoomFactor, zoomFactor, zoomFactor);
  glRotatef(rotX, 1.0, 0.0, 0.0);
  glRotatef(rotY, 0.0, 1.0, 0.0);
  glRotatef(rotZ, 0.0, 0.0, 1.0);

//  glPushMatrix();

//      glLoadIdentity();
//     gluPerspective(atan(tan(90.0 * 3.14159 / 360.0) / 1.0) * 360.0 / 3.141593, 1.0, 1.0, 17.0);
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     gluLookAt(0.0, 0.0, 5.0,
//               0.0, 0.0, 0.0,
//               0.0, 1.0, 0.0);

//  glMatrixMode(GL_MODELVIEW);

  // TODO: draw the model

  glBegin(GL_POLYGON);
        glColor3f(1.0,0.5,0);
        glVertex3d(0.5,0.5,0);
        glColor3f(0,1.0,0.5);
        glVertex3d(-0.0,0.5,0);
        glColor3f(1,0.5,0);
        glVertex3d(-0.0,-0.0,0);
        glColor3f(1,0,0.5);
        glVertex3d(0.5,-0.0,0);
  glEnd();

  glBegin(GL_POLYGON);
        glColor3f(1.0,0.5,0);
        glVertex3d(0.0,0.0,0);
        glColor3f(0,1.0,0.5);
        glVertex3d(-0.5,0.0,0);
        glColor3f(1,0.5,0);
        glVertex3d(-0.5,-0.5,0);
        glColor3f(1,0,0.5);
        glVertex3d(0.0,-0.5,0);
  glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // TODO:
  // - Capture mouse motions
    QPoint newPos = event->pos();
  // - If left click is pressed: rotate
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        if(event->modifiers() & Qt::ShiftModifier){
            rotX -= 10 * dy;
            rotY -= 10 * dx;
        }
        else{
            rotX -= 1 * dy;
            rotY -= 1 * dx;
        }
    }

    // - If right click is pressed: translate
    else if (event->buttons() & Qt::RightButton) {
        if(event->modifiers() & Qt::ShiftModifier){
            movX -= 0.1 * dx;
            movY += 0.1 * dy;
        }
        else{
            movX -= 0.01 * dx;
            movY += 0.01 * dy;
        }
    }
    repaint();
    lastPos = newPos;
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  // TODO: zoom
    const int degrees = event->delta()  / 8;

       int steps = degrees / 15;
       double scaleFactor = 100;

       if(event->modifiers() & Qt::ShiftModifier){
        scaleFactor = 10; //How fast we zoom
       }
       else{
            scaleFactor = 100; //How fast we zoom
       }

       zoomFactor += ((double)steps)/scaleFactor;

       repaint();
}
