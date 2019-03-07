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

  /*grog::TransformMatrix pouet = grog::TransformMatrix::View(300, 40, 0,
                                                            0, 12, 5,
                                                            0, 1, 0.2);*/
  grog::Matrix pouet = grog::Matrix::Projection(10.f, 1, 20);

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
  QPixmap pix(80, 64);
  engine.init(10, 100, 3, &pix);

  const grog::coord vertices[] {-1,  -1, -1,
                                -1,  -1,  1,
                                -1,   1, -1,
                                -1,   1,  1,
                                 1,  -1,  -1,
                                 1,  -1,  1,
                                 1,   1,  -1,
                                 1,   1,  1};
  const uint32_t faces[] {0, 6, 4,
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
                          1, 7, 3};
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

    engine.render();

    QPixmap coincoin = pix.scaled(320, 256);

    ui->label->setPixmap(coincoin);

    QApplication::processEvents();

    std::this_thread::sleep_for(std::chrono::milliseconds(40));
//    exit(0);
  }
}


