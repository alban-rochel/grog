#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Math.h"
#include "Car.h"
#include "Road.h"

#include <iostream>
#include <QPainter>
#include <QFileDialog>
#include <thread>
#include <chrono>

#define ANGLE(fl) ((int32_t)(fl * 512. / M_PI + 0.5))

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  engine.init(100, 100, &pix);

  scene.m_mesh.m_vertexBuffer = nullptr;
  scene.m_mesh.m_vertexCount = 0;
  scene.m_mesh.m_faces = nullptr;
  scene.m_mesh.m_faceCount = 0;
  scene.m_mesh.m_colors = nullptr;
  scene.m_children = new grog::SceneNode*[7];
  scene.m_childCount = 7;

  scene.m_children[0] = new demo::Car(true);
  scene.m_children[1] = new demo::Car(false);
  scene.m_children[1]->m_transform.identity().translate(1500,0, 1000);

  scene.m_children[2] = new demo::Road();
  scene.m_children[3] = new demo::Road();
  scene.m_children[3]->m_transform.identity().translate((1 << 14) + (1 << 15), 0, 0);
  scene.m_children[4] = new demo::Road();
  scene.m_children[4]->m_transform.identity().translate((1 << 14), 0, 5000);
  scene.m_children[5] = new demo::Road();
  scene.m_children[5]->m_transform.identity().translate(- (1 << 14), 0, -5000);
  scene.m_children[6] = new demo::Road();
  scene.m_children[6]->m_transform.identity().translate(-(1 << 14) - (1 << 15), 0, 0);


  ui->eyeXSpinBox->setValue(eyeX);
  ui->eyeYSpinBox->setValue(eyeY);
  ui->eyeZSpinBox->setValue(eyeZ);

  ui->targetXSpinBox->setValue(targetX);
  ui->targetYSpinBox->setValue(targetY);
  ui->targetZSpinBox->setValue(targetZ);

  ui->upXSpinBox->setValue(upX);
  ui->upYSpinBox->setValue(upY);
  ui->upZSpinBox->setValue(upZ);

  ui->fovSpinBox->setValue(fov);
  ui->nearSpinBox->setValue(near);
  ui->farSpinBox->setValue(far);

  connect(ui->eyeXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(eyeXChanged(double)));
  connect(ui->eyeYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(eyeYChanged(double)));
  connect(ui->eyeZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(eyeZChanged(double)));

  connect(ui->targetXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(targetXChanged(double)));
  connect(ui->targetYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(targetYChanged(double)));
  connect(ui->targetZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(targetZChanged(double)));

  connect(ui->upXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(upXChanged(double)));
  connect(ui->upYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(upYChanged(double)));
  connect(ui->upZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(upZChanged(double)));

  connect(ui->fovSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fovChanged(double)));
  connect(ui->nearSpinBox, SIGNAL(valueChanged(double)), this, SLOT(nearChanged(double)));
  connect(ui->farSpinBox, SIGNAL(valueChanged(double)), this, SLOT(farChanged(double)));

  connect(ui->actionDefault, SIGNAL(triggered(bool)), this, SLOT(defaultScene()));
  connect(ui->actionLoad, SIGNAL(triggered(bool)), this, SLOT(convertObj()));

  timer.setInterval(40);
  connect(&timer, SIGNAL(timeout()), this, SLOT(step()));

  timer.start();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::step()
{
  static int32_t shift[] = {-(1<<14)-(1<<15), -(1<<14), 0, 1<<14, (1<<14)+(1<<15)};
  for(uint32_t roadIndex = 0; roadIndex < 5; ++roadIndex)
  {
    scene.m_children[2 + roadIndex]->m_transform.identity().translate(shift[roadIndex], 0, 0);
    shift[roadIndex] += 300;
    if(shift[roadIndex] >= (1<<14)+(1<<15))
      shift[roadIndex] -= (1<<16) + (1<<14);
  }

  draw();
}

void MainWindow::eyeXChanged(double val)
{
  eyeX = val;
  draw();
}

void MainWindow::eyeYChanged(double val)
{
  eyeY = val;
  draw();
}

void MainWindow::eyeZChanged(double val)
{
  eyeZ = val;
  draw();
}

void MainWindow::targetXChanged(double val)
{
  targetX = val;
  draw();
}

void MainWindow::targetYChanged(double val)
{
  targetY = val;
  draw();
}

void MainWindow::targetZChanged(double val)
{
  targetZ = val;
  draw();
}

void MainWindow::upXChanged(double val)
{
  upX = val;
  draw();
}

void MainWindow::upYChanged(double val)
{
  upY = val;
  draw();
}

void MainWindow::upZChanged(double val)
{
  upZ = val;
  draw();
}

void MainWindow::fovChanged(double val)
{
  fov = val;
  draw();
}

void MainWindow::nearChanged(double val)
{
  near = val;
  draw();
}

void MainWindow::farChanged(double val)
{
  far = val;
  draw();
}

void MainWindow::draw()
{
  engine.setProjection(grog::Matrix::Projection(fov, near, far));

  engine.setView(grog::TransformMatrix::View(grog::Math::FloatToFixed(eyeX),    grog::Math::FloatToFixed(eyeY),     grog::Math::FloatToFixed(eyeZ),
                                             grog::Math::FloatToFixed(targetX), grog::Math::FloatToFixed(targetY),  grog::Math::FloatToFixed(targetZ),
                                             grog::Math::FloatToFixed(upX),     grog::Math::FloatToFixed(upY),      grog::Math::FloatToFixed(upZ)));

  engine.projectScene(&scene, 0x01);
  engine.projectScene(&scene, 0x02);

  engine.display.draw();

  QPixmap coincoin = pix.scaled(320, 256);

  ui->label->setPixmap(coincoin);

  QApplication::processEvents();
}

void MainWindow::defaultScene()
{}

void MainWindow::convertObj()
{
  QString fileName = QFileDialog::getOpenFileName(this, "obj to convert", QString(), "*.obj");

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    std::cerr << "Failed opening " << fileName.toStdString() << std::endl;
    return;
  }

  std::cout << "Success opening " << fileName.toStdString() << std::endl;

  std::vector<int32_t> vertices;
  std::vector<uint32_t> faces;

  while(!file.atEnd())
  {
    QString line = QString(file.readLine().data());

    QStringList sl = line.split(" ");

    if(sl.size() == 4 && sl[0] == "v")
    {
      float vertexCoord = atof(sl[1].toStdString().data());
      vertices.push_back(grog::Math::FloatToFixed(vertexCoord));
      vertexCoord = atof(sl[2].toStdString().data());
      vertices.push_back(grog::Math::FloatToFixed(vertexCoord));
      vertexCoord = atof(sl[3].toStdString().data());
      vertices.push_back(grog::Math::FloatToFixed(vertexCoord));
    }
    else if(sl.size() == 4 && sl[0] == "f")
    {
      QStringList sl2 = sl[1].split("/");
      uint32_t faceIndex = atoi(sl2[0].toStdString().data())-1;
      faces.push_back(faceIndex);
      sl2 = sl[2].split("/");
      faceIndex = atoi(sl2[0].toStdString().data())-1;
      faces.push_back(faceIndex);
      sl2 = sl[3].split("/");
      faceIndex = atoi(sl2[0].toStdString().data())-1;
      faces.push_back(faceIndex);
    }
  }

  file.close();

  std::cout << "Success parsing " << fileName.toStdString() << std::endl;

  std::cout << "static const int32_t vertices[] {\n";
  for(unsigned int ii = 0; ii < vertices.size(); ii+=3)
  {
    std::cout << "\t" << vertices[ii] << ",\t" << vertices[ii+1] << ",\t" << vertices[ii+2] << ",\n";
  }
  std::cout << "};\n\n";

  std::cout << "static const uint32_t faces[] {\n";
  for(unsigned int ii = 0; ii < faces.size(); ii+=3)
  {
    std::cout << "\t" << faces[ii] << ",\t" << faces[ii+1] << ",\t" << faces[ii+2] << ",\n";
  }
  std::cout << "};\n\n";

  std::cout << "static const uint8_t colors[] {\n";
  for(unsigned int ii = 0; ii < faces.size(); ii+=3)
  {
    std::cout << "grog::color(grog::Color::White),\n";
  }
  std::cout << "};\n\n";

  std::cout << "static const uint32_t vertexCount {" << vertices.size()/3 << "};\n";
  std::cout << "static const uint32_t faceCount {" << faces.size()/3 << "};\n";

  std::cout.flush();
}


