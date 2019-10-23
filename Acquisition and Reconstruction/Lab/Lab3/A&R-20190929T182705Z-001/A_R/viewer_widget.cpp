#include "viewer_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include "glwidget.h"

ViewerWidget::ViewerWidget() {
  layout = new QGridLayout(this);
  load_file_button = new QPushButton("Load file");
  layout->addWidget(load_file_button, 0, 0);
  connect(load_file_button, SIGNAL(released()), this, SLOT(loadFile()));
  zsort_button = new QPushButton("Z-sorting",this);
  connect(zsort_button, SIGNAL(released()), this, SLOT(zsort()));
  layout->addWidget(zsort_button, 2, 0);
  gl_widget = new GLWidget();
  layout->addWidget(gl_widget, 1, 0);
  slider = new QSlider(Qt::Vertical, this);
  slider->setSliderPosition(50);
  slider->setValue(50);
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(value(int)));
  layout->addWidget(slider,1,1);
  QLabel *label1 = new QLabel("alpha = 1", this);
  QLabel *label2 = new QLabel("alpha = 0", this);
  layout->addWidget(label1,0,1);
  layout->addWidget(label2,2,1);
}

void ViewerWidget::loadFile() {
  QString file_name;
  // TODO: open a dialog to let the user choose a json file
  file_name = QFileDialog::getOpenFileName(this, "Open a file", "../");
   QFileInfo fi(file_name);
   QString file_ext = fi.suffix();
   if(file_ext != "json"){
       QMessageBox::warning(this, "Error", "File extension invalid!");
    }
  gl_widget->loadFaces(file_name);
}

void ViewerWidget::zsort() {
    gl_widget->zsort_wid();
    }
