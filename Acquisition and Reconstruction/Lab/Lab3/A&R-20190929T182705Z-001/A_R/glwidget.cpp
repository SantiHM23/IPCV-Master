#include <QFile>
#include <QString>
#include <QTransform>
#include <QtGui>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QtOpenGL>
#include "glwidget.h"
#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

GLWidget::~GLWidget() {}
QPoint lastPos;
GLfloat rotX = 0, rotY = 0, rotZ = 0;
GLfloat movX = 0, movY = 0, movZ = 0;
GLfloat zoomFactor = 1.0;


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
  repaint();
}

void GLWidget::initializeGL() {
  // TODO: initialize the OpenGL to display 3d data
    glClearColor(1,1,0.5,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDepthFunc(GL_NEVER);
    alpha = 50/99.;
}



void GLWidget::paintGL() {
  QSize viewport_size = size();
  glViewport(0, 0, viewport_size.width(), viewport_size.height());
  glMatrixMode(GL_PROJECTION);

  // TODO: define the point of view
  glLoadIdentity();
  gluPerspective(atan(tan(50.0 * 3.14159 / 360.0) / zoomFactor) * 360.0 / 3.141593, double(viewport_size.width())/double(viewport_size.height()), 3.0, 17.0);

    gluLookAt(0,0,5,
              0,0,0,
            0.0, 1.0, 0.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  // TODO: draw the model
  glTranslatef(movX, movY, 0);
  glScalef(zoomFactor, zoomFactor, zoomFactor);
  glRotatef(rotX, 1.0, 0.0, 0.0);
  glRotatef(rotY, 0.0, 1.0, 0.0);
  //glRotatef(rotZ, 0.0, 0.0, 1.0);

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glLightfv(GL_LIGHT0,GL_POSITION,pos);
  glDisable(GL_LIGHTING);
  if(zsorted){
      float max_x = 0, max_y = 0, max_z = 0;
             for(int i=0; i<face_collection.faces.size(); i++){
                int idx = sorted_collection[i].idx;
                                double z_vec = norm[2][0] * face_collection.faces[idx].normal[0] + norm[2][1] * face_collection.faces[idx].normal[1] + norm[2][2] * face_collection.faces[idx].normal[2];

                if(z_vec<0){
                  continue;
               }
                vector<QVector3D> vertices = face_collection.faces[idx].vertices;
                /*
               if(sorted_collection.faces[i].idx==1){
                    double z_coord = norm[2][0] * sorted_collection.faces[i].vertices[0][0] + norm[2][1] * sorted_collection.faces[i].vertices[0][1] + norm[2][2] * sorted_collection.faces[i].vertices[0][2];
                    cout << "Z coord of red square: " << z_coord << endl;
                    glBegin(GL_POLYGON);
                    glColor4f(1, 0, 0,alpha);
                    glVertex3f(float(vertices[0][0]), float(vertices[0][1]), float(vertices[0][2]));
                    glColor4f(1, 0, 0,alpha);
                    glVertex3f(float(vertices[1][0]), float(vertices[1][1]), float(vertices[1][2]));
                    glColor4f(1, 0, 0,alpha);
                    glVertex3f(float(vertices[2][0]),  float(vertices[2][1]), float(vertices[2][2]));
                    glColor4f(1,0,0,alpha);
                    glVertex3f(float(vertices[3][0]),float(vertices[3][1]), float(vertices[3][2]));
                    glEnd();
                }
                else{*/
               // cout << "I'm displayed! " << endl;
               // double z_coord = norm[2][0] * sorted_collection.faces[i].vertices[0][0] + norm[2][1] * sorted_collection.faces[i].vertices[0][1] + norm[2][2] * sorted_collection.faces[i].vertices[0][2];
               // cout << "My z coord: " << z_coord << endl;
               // cout << "My z vector: " << z << endl;
               // cout << endl;

                glBegin(GL_POLYGON);
                glColor4f(face_collection.faces[idx].c, face_collection.faces[idx].c, face_collection.faces[idx].c,alpha);
                glVertex3f(float(vertices[0][0]), float(vertices[0][1]), float(vertices[0][2]));
                glColor4f(face_collection.faces[idx].c, face_collection.faces[idx].c, face_collection.faces[idx].c,alpha);
                glVertex3f(float(vertices[1][0]), float(vertices[1][1]), float(vertices[1][2]));
                glColor4f(face_collection.faces[idx].c, face_collection.faces[idx].c, face_collection.faces[idx].c,alpha);
                glVertex3f(float(vertices[2][0]),  float(vertices[2][1]), float(vertices[2][2]));
                glColor4f(face_collection.faces[idx].c,face_collection.faces[idx].c, face_collection.faces[idx].c,alpha);
                glVertex3f(float(vertices[3][0]),float(vertices[3][1]), float(vertices[3][2]));
                glEnd();
                }


      }

  else {
  float max_x = 0, max_y = 0, max_z = 0;
  for(int i=0; i<face_collection.faces.size(); i++){
     for(int j=0; j<4; j++){
      if(abs(face_collection.faces[i].vertices[j][0])>max_x)
          max_x = abs(face_collection.faces[i].vertices[j][0]);
      if(abs(face_collection.faces[i].vertices[j][1])>max_y)
          max_y = abs(face_collection.faces[i].vertices[j][1]);
      if(abs(face_collection.faces[i].vertices[j][2])>max_z)
          max_z = abs(face_collection.faces[i].vertices[j][2]);
     }
  }
  for(int i=0; i<face_collection.faces.size(); i++){
      for(int j=0; j<4; j++){
          face_collection.faces[i].vertices[j][0] /= (max_x*1.15);
          face_collection.faces[i].vertices[j][1] /= (max_y*1.15);
          face_collection.faces[i].vertices[j][2] /= (max_z*1.15);
          }
  }

         for(int i=0; i<face_collection.faces.size(); i++){
            vector<QVector3D> vertices = face_collection.faces[i].vertices;
            glBegin(GL_POLYGON);
            glColor4f(face_collection.faces[i].c,face_collection.faces[i].c,face_collection.faces[i].c,alpha);
            glVertex3f(float(vertices[0][0]), float(vertices[0][1]), float(vertices[0][2]));
            glColor4f(face_collection.faces[i].c, face_collection.faces[i].c, face_collection.faces[i].c,alpha);
            glVertex3f(float(vertices[1][0]), float(vertices[1][1]), float(vertices[1][2]));
            glColor4f(face_collection.faces[i].c, face_collection.faces[i].c, face_collection.faces[i].c,alpha);
            glVertex3f(float(vertices[2][0]),  float(vertices[2][1]), float(vertices[2][2]));
            glColor4f(face_collection.faces[i].c,face_collection.faces[i].c,face_collection.faces[i].c,alpha);
            glVertex3f(float(vertices[3][0]),float(vertices[3][1]), float(vertices[3][2]));
            glEnd();
        }
  }
  // get matrixs and viewport:
  glGetDoublev( GL_MODELVIEW_MATRIX, mat );

  rot[0][0] = mat[0];
  rot[0][1] = mat[4];
  rot[0][2] = mat[8];
  rot[1][0] = mat[1];
  rot[1][1] = mat[5];
  rot[1][2] = mat[9];
  rot[2][0] = mat[2];
  rot[2][1] = mat[6];
  rot[2][2] = mat[10];


  double det = rot[0][0] * (rot[1][1] * rot[2][2] - rot[2][1] * rot[1][2]) -
               rot[0][1] * (rot[1][0] * rot[2][2] - rot[1][2] * rot[2][0]) +
               rot[0][2] * (rot[1][0] * rot[2][1] - rot[1][1] * rot[2][0]);

  double invdet = 1 / det;

  norm[0][0] = (rot[1][1] * rot[2][2] - rot[2][1] * rot[1][2]) * invdet;
  norm[0][1] = (rot[0][2] * rot[2][1] - rot[0][1] * rot[2][2]) * invdet;
  norm[0][2] = (rot[0][1] * rot[1][2] - rot[0][2] * rot[1][1]) * invdet;
  norm[1][0] = (rot[1][2] * rot[2][0] - rot[1][0] * rot[2][2]) * invdet;
  norm[1][1] = (rot[0][0] * rot[2][2] - rot[0][2] * rot[2][0]) * invdet;
  norm[1][2] = (rot[1][0] * rot[0][2] - rot[0][0] * rot[1][2]) * invdet;
  norm[2][0] = (rot[1][0] * rot[2][1] - rot[2][0] * rot[1][1]) * invdet;
  norm[2][1] = (rot[2][0] * rot[0][1] - rot[0][0] * rot[2][1]) * invdet;
  norm[2][2] = (rot[0][0] * rot[1][1] - rot[1][0] * rot[0][1]) * invdet;


  glFlush();
  glPopMatrix();
}



void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // TODO:
    cnt++;
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

    int N = face_collection.faces.size();
    if(zsorted == true){

        for (int i=0; i<N; i++){
            int idx = sorted_collection[i].idx;
            sorted_collection[i].z = norm[0][2] * face_collection.faces[idx].vertices[0][0] + norm[1][2] * face_collection.faces[idx].vertices[0][1] + norm[2][2] * face_collection.faces[idx].vertices[0][2];
        }

        for (int i = 1; i < N; i++)
        {
            float K = sorted_collection[i].z;
            double tmp_idx = sorted_collection[i].idx;
            int j = i - 1;
            while(j>=0 && sorted_collection[j].z > K){
                sorted_collection[j+1].z = sorted_collection[j].z;
                sorted_collection[j+1].idx = sorted_collection[j].idx;
                j--;
            }
            sorted_collection[j+1].idx = tmp_idx;
            sorted_collection[j+1].z = K;
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

void GLWidget::zsort_wid(){
    zsorted = !zsorted;
   if (zsorted){

//        glDisable(GL_DEPTH_TEST);
//        glDepthFunc(GL_NEVER);
    }
    else{
//        glEnable(GL_DEPTH_TEST);
    }

    if(zsorted && !first_sort){
        first_sort = true;
        int N = face_collection.faces.size();
        for (int i=0; i<N; i++){
            double z = (face_collection.faces[i].vertices[0][2] + face_collection.faces[i].vertices[1][2] + face_collection.faces[i].vertices[2][2] + face_collection.faces[i].vertices[3][2])/4;
            sorted_collection.push_back(Sorting_Object(z,i));
        }
        for (int i = 0; i < N-1; i++)
        {

            int jMin = i;

            for (int j = i+1; j < N; j++)
            {

                if (sorted_collection[j].z < sorted_collection[jMin].z)
                {

                    jMin = j;
                }
            }

            if (jMin != i)
            {
                double temp_z = sorted_collection[i].z;
                int temp_idx = sorted_collection[i].idx;
                sorted_collection[i].z = sorted_collection[jMin].z;
                sorted_collection[i].idx = sorted_collection[jMin].idx;
                sorted_collection[jMin].z = temp_z;
                sorted_collection[jMin].idx = temp_idx;
            }
        }
    paintGL();
    }
    else{
        paintGL();
    }
    repaint();

}

