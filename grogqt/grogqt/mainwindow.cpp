#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trigo.h"

#include <iostream>
#include <QPainter>
#include <thread>
#include <chrono>


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

static const uint8_t colorsX[] {
  grog::color(grog::Color::Red), grog::color(grog::Color::Red),
  grog::color(grog::Color::Red), grog::color(grog::Color::Red),
  grog::color(grog::Color::Red), grog::color(grog::Color::Red),
  grog::color(grog::Color::Red), grog::color(grog::Color::Red),
  grog::color(grog::Color::Red), grog::color(grog::Color::Red),
  grog::color(grog::Color::Red), grog::color(grog::Color::Red)
};

static const uint8_t colorsY[] {
  grog::color(grog::Color::Green), grog::color(grog::Color::Green),
  grog::color(grog::Color::Green), grog::color(grog::Color::Green),
  grog::color(grog::Color::Green), grog::color(grog::Color::Green),
  grog::color(grog::Color::Green), grog::color(grog::Color::Green),
  grog::color(grog::Color::Green), grog::color(grog::Color::Green),
  grog::color(grog::Color::Green), grog::color(grog::Color::Green)
};

static const uint8_t colorsZ[] {
  grog::color(grog::Color::Blue), grog::color(grog::Color::Blue),
  grog::color(grog::Color::Blue), grog::color(grog::Color::Blue),
  grog::color(grog::Color::Blue), grog::color(grog::Color::Blue),
  grog::color(grog::Color::Blue), grog::color(grog::Color::Blue),
  grog::color(grog::Color::Blue), grog::color(grog::Color::Blue),
  grog::color(grog::Color::Blue), grog::color(grog::Color::Blue)
};

#define FIXED(fl) ((int32_t)(fl*1024))
#define ANGLE(fl) ((int32_t)(fl * 512. / M_PI + 0.5))

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
//  generateTrigo();
  pwet();

  exit(0);
  ui->setupUi(this);

  engine.init(10, 100, 3, &pix);

  scene.mesh.vertexBuffer = vertices;
  scene.mesh.vertexCount = 8;
  scene.mesh.faces = faces;
  scene.mesh.faceCount = 12;
  scene.mesh.colors = colors;

  scene.children = new grog::SceneNode[3];
  scene.childCount = 3;

  scene.children[0].mesh.vertexBuffer = vertices;
  scene.children[0].mesh.vertexCount = 8;
  scene.children[0].mesh.faces = faces;
  scene.children[0].mesh.faceCount = 12;
  scene.children[0].mesh.colors = colorsX;

  scene.children[1].mesh.vertexBuffer = vertices;
  scene.children[1].mesh.vertexCount = 8;
  scene.children[1].mesh.faces = faces;
  scene.children[1].mesh.faceCount = 12;
  scene.children[1].mesh.colors = colorsY;

  scene.children[2].mesh.vertexBuffer = vertices;
  scene.children[2].mesh.vertexCount = 8;
  scene.children[2].mesh.faces = faces;
  scene.children[2].mesh.faceCount = 12;
  scene.children[2].mesh.colors = colorsZ;

  scene.children[0].transform.identity().translate(2, 0, 0).scale(0.5, 0.5, 0.5);
  scene.children[1].transform.identity().translate(0, 2, 0).scale(0.5, 0.5, 0.5);
  scene.children[2].transform.identity().translate(0, 0, 2).scale(0.5, 0.5, 0.5);

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
  delete[] scene.children;
}

void MainWindow::pwet()
{
  grog::TransformMatrix trans;
  fixedgrog::TransformMatrix ftrans;

#define PWET \
  std::cout << "FLOAT" << std::endl;\
  trans.print();\
  std::cout << "FIXED" << std::endl;\
  ftrans.print();

  PWET;

  trans.translate(1.234, 2.345, 4.567);
  ftrans.translate(FIXED(1.234),
                   FIXED(2.345),
                   FIXED(4.567));

  PWET;

  trans.scale(1.4f, 5.6f, 2.7f);
  ftrans.scale(FIXED(1.4),
               FIXED(5.6),
               FIXED(2.7));

  PWET;

  std::cout << "cos(0) " << cos(0) << " - " << ANGLE(0) << "/" << fixedgrog::Cos(ANGLE(0))/1024. << std::endl;
  std::cout << "cos(M_PI) " << cos(M_PI) << " - " << ANGLE(M_PI) << "/" << fixedgrog::Cos(ANGLE(M_PI))/1024. << std::endl;
  std::cout << "cos(2*M_PI) " << cos(2*M_PI) << " - " << ANGLE(2*M_PI) << "/" << fixedgrog::Cos(ANGLE(2*M_PI))/1024. << std::endl;
  std::cout << "sin(0) " << sin(0) << " - " << ANGLE(0) << "/" << fixedgrog::Sin(ANGLE(0))/1024. << std::endl;
  std::cout << "sin(M_PI) " << sin(M_PI) << " - " << ANGLE(M_PI) << "/" << fixedgrog::Sin(ANGLE(M_PI))/1024. << std::endl;
  std::cout << "sin(2*M_PI) " << sin(2*M_PI) << " - " << ANGLE(2*M_PI) << "/" << fixedgrog::Sin(ANGLE(2*M_PI))/1024. << std::endl;
  std::cout << "cos(1) " << cos(1.) << " - " << ANGLE(1.) << "/" << fixedgrog::Cos(ANGLE(1.))/1024. << std::endl;
  std::cout << "sin(1) " << sin(1.) << " - " << ANGLE(1.) << "/" << fixedgrog::Sin(ANGLE(1.))/1024. << std::endl;

  trans.rotateX(2.f);
  ftrans.rotateX(ANGLE(2.f));

  PWET;

  trans.rotateY(3.f);
  ftrans.rotateY(ANGLE(3.f));

  PWET;

  trans.rotateZ(5.f);
  ftrans.rotateZ(ANGLE(5.f));

  PWET

  std::cout.flush();

  {
    grog::TransformMatrix trans2;

    float xRot(1.), yRot(2.);

    auto start = std::chrono::steady_clock::now();

    for(unsigned int ii = 0; ii < 1000000; ++ii)
    {
      trans2.rotateX(xRot).rotateY(yRot);
    }

    auto end = std::chrono::steady_clock::now();

    std::cout << "Float duration " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
  }

  {
    fixedgrog::TransformMatrix trans2;

    int32_t xRot(ANGLE(1.)), yRot(ANGLE(2.));

    auto start = std::chrono::steady_clock::now();

    for(unsigned int ii = 0; ii < 1000000; ++ii)
    {
      trans2.rotateX(xRot).rotateY(yRot);
    }

    auto end = std::chrono::steady_clock::now();

    std::cout << "Fixed duration " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
  }

}

void MainWindow::generateTrigo()
{
  std::cout << "static constexpr int32_t cosines[] = {" << std::endl;

  uint32_t index = 0;
  for(unsigned int line = 0; line < 32; ++line)
  {
    for(unsigned int col = 0; col < 32; ++col, ++index)
    {
      std::cout << (int32_t)(cos((index * 2 * M_PI)/1024.) * 1024 + 0.5);
      if(!(col == 31 && line == 31))
      {
          std::cout << ",\t";
      }
    }
    std::cout << "\n";
  }
  std::cout << "};" << std::endl;
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

  engine.setView(grog::TransformMatrix::View(eyeX, eyeY, eyeZ,
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


