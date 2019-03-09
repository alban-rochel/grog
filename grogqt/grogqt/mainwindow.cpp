#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QPainter>
#include <thread>


const grog::coord vertices[] {
  -1,  -1, -1,
  -1,  -1,  1,
  -1,   1, -1,
  -1,   1,  1,
  1,  -1,  -1,
  1,  -1,  1,
  1,   1,  -1,
  1,   1,  1
};
static const uint32_t faces[] {
  0, 6, 4,
  0, 2, 6,
  0, 3, 2,
  0, 1, 3,
  2, 7, 6,
  2, 3, 7,
  4, 6, 7,
  4, 7, 5,
  0, 4, 5,
  0, 5, 1,
  1, 5, 7,
  1, 7, 3
};
static const uint8_t colors[] {
  grog::color(grog::Color::White), grog::color(grog::Color::White),
  grog::color(grog::Color::Gray), grog::color(grog::Color::Gray),
  grog::color(grog::Color::DarkGray), grog::color(grog::Color::DarkGray),
  grog::color(grog::Color::White, grog::Color::DarkBlue), grog::color(grog::Color::White, grog::Color::DarkBlue),
  grog::color(grog::Color::Gray, grog::Color::DarkBlue), grog::color(grog::Color::Gray, grog::Color::DarkBlue),
  grog::color(grog::Color::DarkGray, grog::Color::DarkBlue), grog::color(grog::Color::DarkGray, grog::Color::DarkBlue)
};



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  engine.init(10, 100, 3, &pix);

  scene.mesh.vertexBuffer = vertices;
  scene.mesh.vertexCount = 8;
  scene.mesh.faces = faces;
  scene.mesh.faceCount = 12;
  scene.mesh.colors = colors;

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

  draw();
}

MainWindow::~MainWindow()
{
  delete ui;
}

QPixmap MainWindow::drawImage(grog::bufferType *buffer) noexcept
{
    QPixmap pix(grog::screenWidth, grog::screenHeight);
    QPainter painter(&pix);

    for(unsigned int y = 0; y < grog::screenHeight; ++y)
    {
        for(unsigned int x = 0; x < grog::screenWidth; x+=2, ++buffer)
        {
            painter.setPen(grog::Palette[(*buffer)&0x0F]);
            painter.drawPoint(x, y);
            painter.setPen(grog::Palette[(*buffer) >> 4]);
            painter.drawPoint(x+1, y);
        }
    }
    painter.end();

    pix = pix.scaled(pix.width() * 4, pix.height() * 4);
    return pix;
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

  engine.setView(grog::TransformMatrix::View(eyeX, eyeZ, eyeZ,
                                             targetX, targetY, targetZ,
                                             upX, upY, upZ));

  engine.projectScene(&scene);

  engine.render();

  QPixmap coincoin = pix.scaled(320, 256);

  ui->label->setPixmap(coincoin);

  QApplication::processEvents();
}

#if 0
void MainWindow::animate()
{
  grog::Engine engine;
  QPixmap pix(80, 64);
  engine.init(10, 100, 3, &pix);


  grog::SceneNode cube;
  cube.mesh.vertexBuffer = vertices;
  cube.mesh.vertexCount = 8;
  cube.mesh.faces = faces;
  cube.mesh.faceCount = 12;
  cube.mesh.colors = colors;
//  cube.children = nullptr;
//  cube.childCount = 0;
  grog::SceneNode cube2;
  cube2.mesh.vertexBuffer = vertices;
  cube2.mesh.vertexCount = 8;
  cube2.mesh.faces = faces;
  cube2.mesh.faceCount = 12;
  cube2.mesh.colors = colors;
  cube.children = &cube2;
  cube.childCount = 1;

  grog::bufferType* buffer = new grog::bufferType[grog::screenWidth * grog::screenHeight];

  engine.setProjection(grog::Matrix::Projection(2.f, 1, 10));

  for(unsigned int ii = 0; ii < 1000; ++ii)
  {

    memset(buffer, 0x0, grog::screenWidth * grog::screenHeight * sizeof (grog::bufferType));


    engine.setView(grog::TransformMatrix::View(1, 1, 1.,
                                               0, 0, 0,
                                               0, 1, 0));

    cube.transform.identity()
        .scale(0.5, 0.5, 0.5)
        .rotateX(ii/20.f)
        .rotateY(ii/15.f)/*
                          .translate(40, 40, 40)*/;

    cube2.transform.identity()
                  .translate(1, 1, 1)
                  .scale(0.4, 0.4, 0.4)
                  ;

    engine.projectScene(&cube);
//    engine.projectScene(&cube2);

    engine.debugTriangleStack();

    engine.render();

    QPixmap coincoin = pix.scaled(320, 256);

    ui->label->setPixmap(coincoin);

    QApplication::processEvents();

    std::this_thread::sleep_for(std::chrono::milliseconds(40));
//    exit(0);
  }
}

void MainWindow::debug()
{
  grog::Engine engine;
  QPixmap pix(80, 64);
  engine.init(10, 100, 3, &pix);

  engine.pushDebugTriangles();

    engine.render();

    QPixmap coincoin = pix.scaled(320, 256);

    ui->label->setPixmap(coincoin);

    QApplication::processEvents();
}
#endif

