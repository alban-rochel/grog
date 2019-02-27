#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Engine.h"
#include "Colors.h"

#include <iostream>
#include <QPainter>
#include <thread>


//#define ITER_COUNT 5
#define ITER_COUNT 1000000


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  //connect(ui->goButton, SIGNAL(clicked()), this, SLOT(fastInteger()));
  connect(ui->goButton, SIGNAL(clicked()), this, SLOT(animate()));

  grog::TransformMatrix pouet = grog::TransformMatrix::View(300, 0, 0,
                                                             0, 0, 0,
                                                             0, 1, 0);

  pouet.print();

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

void MainWindow::animate()
{
  grog::Engine engine;
  engine.init(10, 14, 3);

  const grog::coord vertices[] {-1,  1, -1,
                                 1,  1, -1,
                                 1, -1, -1,
                                -1, -1, -1,
                                -1,  1,  1,
                                 1,  1,  1,
                                 1, -1,  1,
                                -1, -1,  1};
  const uint32_t faces[] {0, 3, 1,
                          1, 3, 2,
                          5, 1, 2,
                          5, 2, 6,
                          4, 7, 0,
                          0, 7, 3,
                          4, 0, 1,
                          4, 1, 5,
                          3, 7, 2,
                          2, 7, 6};
  const uint8_t colors[] {grog::color(grog::Color::White), grog::color(grog::Color::White),
                          grog::color(grog::Color::Gray), grog::color(grog::Color::Gray),
                          grog::color(grog::Color::DarkGray), grog::color(grog::Color::DarkGray),
                          grog::color(grog::Color::White, grog::Color::DarkBlue), grog::color(grog::Color::White, grog::Color::DarkBlue),
                          grog::color(grog::Color::Gray, grog::Color::DarkBlue), grog::color(grog::Color::Gray, grog::Color::DarkBlue),
                          grog::color(grog::Color::DarkGray, grog::Color::DarkBlue), grog::color(grog::Color::DarkGray, grog::Color::DarkBlue)};

  grog::SceneNode cube;
  cube.mesh.vertexBuffer = vertices;
  cube.mesh.vertexCount = 8;
  cube.mesh.faces = faces;
  cube.mesh.faceCount = 10;
  cube.mesh.colors = colors;

  grog::bufferType* buffer = new grog::bufferType[grog::screenWidth * grog::screenHeight];

  engine.setView(grog::TransformMatrix::View(0, 0, 5,
                                             0, 0, 0,
                                             0, 1, 0));

/*  grog::TransformMatrix pouet;

  pouet.data[3] = 1;*/

//  engine.setView(pouet);

  engine.setProjection(grog::Matrix::Projection(2.f, 1, 10));

  for(unsigned int ii = 0; ii < 1000; ++ii)
  {

    memset(buffer, 0x0, grog::screenWidth * grog::screenHeight * sizeof (grog::bufferType));

    engine.setView(grog::TransformMatrix::View(0, 0, 1,
                                               0, 0, 0,
                                               0, 1, 0));

    cube.transform.identity()
                  .scale(0.5, 0.5, 0.5)
                  .rotateX(ii/20.f)
                  .rotateY(ii/15.f)/*
                  .translate(40, 40, 40)*/;

    engine.projectScene(&cube);

    engine.render(buffer);

    QPixmap pix = drawImage(buffer);

    ui->label->setPixmap(pix);

    QApplication::processEvents();

    std::this_thread::sleep_for(std::chrono::milliseconds(40));
  }
  delete[] buffer;
}


