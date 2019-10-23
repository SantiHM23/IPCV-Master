#pragma once

#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>
#include <QLabel>
#include "glwidget.h"

class ViewerWidget : public QWidget {
public:
  Q_OBJECT
public:
  ViewerWidget();
  QGridLayout *layout;
  QPushButton *load_file_button;
  QPushButton *zsort_button;
  QFileDialog *dialog;
  QSlider *slider;
  QLabel *label1, label2;
  GLWidget *gl_widget;
public slots:
  void loadFile();
  void zsort();
  void value(int k){
      gl_widget->alpha = k/99.;
      gl_widget->repaint();
  }
};
