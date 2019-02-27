#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Engine.h"
#include "Colors.h"

#include <iostream>
#include <QPainter>
#include <thread>

// Triangle ABC is large, rectangle

#define Ax 20
#define Ay 60
#define Az 10

#define Bx Ax
#define By 20
#define Bz 10

#define Cx 50
#define Cy Ay
#define Cz 50

// Triangle CBG is just next to ABC

#define Gx 70
#define Gy 30
#define Gz 50

// Triangle DEF goes through ABC

#define Dx 30
#define Dy 50
#define Dz 50

#define Ex 10
#define Ey 58
#define Ez 5

#define Fx 6
#define Fy 2
#define Fz 5

// Triangle HJI is tall and narrow

#define Hx 62
#define Hy 62
#define Hz 8

#define Jx 56
#define Jy 7
#define Jz 8

#define Ix 59
#define Iy 5
#define Iz 8

//#define ITER_COUNT 5
#define ITER_COUNT 1000000


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  //connect(ui->goButton, SIGNAL(clicked()), this, SLOT(fastInteger()));
  connect(ui->goButton, SIGNAL(clicked()), this, SLOT(animate()));

//  grog::TransformMatrix plop = grog::TransformMatrix::View(300, 0, 0,
//                                                           0, 0, 0,
//                                                           0, 1, 0);
//  std::cout << plop.data[0] << " - " << plop.data[1] << " - " << plop.data[2] <<  " - " << plop.data[3] << "\n"
//                               << plop.data[4] << " - " << plop.data[5] << " - " << plop.data[6] << " - " << plop.data[7] << "\n"
//                                  << plop.data[8] << " - " << plop.data[9] << " - " << plop.data[10] << " - " << plop.data[11] <<"\n"
//                                     << plop.data[12] <<"\n";

  //grog::Matrix pouet = grog::Matrix::Projection(10.f, 1.f, 10.f);
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
        for(unsigned int x = 0; x < grog::screenWidth; ++x, ++buffer)
        {
            painter.setPen(grog::Palette[(*buffer)&0xF]);
            painter.drawPoint(x, y);
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

void MainWindow::fastInteger() noexcept
{
/*    int vertices [] = {   Ax, Ay, Az, //0
                            Bx, By, Bz, //1
                            Cx, Cy, Cz, //2
                            Dx, Dy, Dz, //3
                            Ex, Ey, Ez, //4
                            Fx, Fy, Fz, //5
                            Gx, Gy, Gz, //6
                            Hx, Hy, Hz, //7
                            Ix, Iy, Iz, //8
                            Jx, Jy, Jz}; //9
    unsigned int triangles [] = {   0, 1, 2,    // ABC
                                    2, 1, 6,    // CBG
                                    4, 5, 6,    // EFD
                                    7, 9, 8};   // HJI

    grog::bufferType colors[] = {1, 2, 3, 4};*/


    grog::bufferType* buffer = new grog::bufferType[grog::screenWidth * grog::screenHeight];
    memset(buffer, 0x0, grog::screenWidth * grog::screenHeight * sizeof (grog::bufferType));

    unsigned int index(0);
    auto start = std::chrono::steady_clock::now();
    //for(unsigned int iter = ITER_COUNT; iter; --iter)
    {
//        index = 3 * (iter & 0x3);

      grog::Triangle triangle;
      triangle.p1x=Ax; triangle.p1y=Ay;
      triangle.p2x=Bx; triangle.p2y=By;
      triangle.p3x=Cx; triangle.p3y=Cy;
      triangle.z=Az; triangle.color=1;
        grog::rasterizeTriangle(triangle,
            buffer
            );

        triangle.p1x=Cx; triangle.p1y=Cy;
        triangle.p2x=Bx; triangle.p2y=By;
        triangle.p3x=Gx; triangle.p3y=Gy;
        triangle.z=Cz; triangle.color=2;
        grog::rasterizeTriangle(triangle,
            buffer
            );

        triangle.p1x=Dx; triangle.p1y=Dy;
        triangle.p2x=Ex; triangle.p2y=Ey;
        triangle.p3x=Fx; triangle.p3y=Fy;
        triangle.z=Dz; triangle.color=3;
        grog::rasterizeTriangle(triangle,
            buffer
            );

        triangle.p1x=Hx; triangle.p1y=Hy;
        triangle.p2x=Jx; triangle.p2y=Jy;
        triangle.p3x=Ix; triangle.p3y=Iy;
        triangle.z=Dz; triangle.color=4;
        grog::rasterizeTriangle(triangle,
            buffer
            );
    }
    auto end = std::chrono::steady_clock::now();

    QPixmap pix = drawImage(buffer);

    delete[] buffer;

    ui->label->setPixmap(pix);

    std::cout << "Speed " << (ITER_COUNT)*1000./(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()) << " triangles/second" << std::endl;

    // 106000 triangles / second
    // 160000 triangles / second avec orient2d_f ameliore
}
