#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trigo.h"
#include "asset_car.h"

#include <iostream>
#include <QPainter>
#include <QFileDialog>
#include <thread>
#include <chrono>


const int32_t vertices[] {
  (-1) << 10,  (-1) << 10, (-1) << 10,
  (-1) << 10,  (-1) << 10, ( 1) << 10,
  (-1) << 10,  ( 1) << 10, (-1) << 10,
  (-1) << 10,  ( 1) << 10, ( 1) << 10,
  ( 1) << 10,  (-1) << 10, (-1) << 10,
  ( 1) << 10,  (-1) << 10, ( 1) << 10,
  ( 1) << 10,  ( 1) << 10, (-1) << 10,
  ( 1) << 10,  ( 1) << 10, ( 1) << 10
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
  grog::color(grog::Color::White), grog::color(grog::Color::White),
  grog::color(grog::Color::Gray), grog::color(grog::Color::Gray),
  grog::color(grog::Color::DarkGray), grog::color(grog::Color::DarkGray)
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

#define ANGLE(fl) ((int32_t)(fl * 512. / M_PI + 0.5))

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
//  generateTrigo();
//  pwet();

//  exit(0);
  ui->setupUi(this);

  engine.init(100, 100, 3, &pix);

  /*scene.mesh.vertexBuffer = vertices;
  scene.mesh.vertexCount = 8;
  scene.mesh.faces = faces;
  scene.mesh.faceCount = 12;
  scene.mesh.colors = colors;*/

  scene.mesh.vertexBuffer = grog::car_vertices;
  scene.mesh.vertexCount = grog::car_vertexCount;
  scene.mesh.faces = grog::car_faces;
  scene.mesh.faceCount = grog::car_faceCount;
  scene.mesh.colors = grog::car_colors;

  scene.children = new grog::SceneNode[3];
  scene.childCount = 0;

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

  scene.children[0].transform.identity().translate(2 << 10, 0, 0).scale(grog::floatToFixed(0.5f), grog::floatToFixed(0.5f), grog::floatToFixed(0.5f));
  scene.children[1].transform.identity().translate(0, 2 << 10, 0).scale(grog::floatToFixed(0.5f), grog::floatToFixed(0.5f), grog::floatToFixed(0.5f));
  scene.children[2].transform.identity().translate(0, 0, 2 << 10).scale(grog::floatToFixed(0.5f), grog::floatToFixed(0.5f), grog::floatToFixed(0.5f));

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

  draw();
}

MainWindow::~MainWindow()
{
  delete ui;
  delete[] scene.children;
}

void MainWindow::pwet()
{
  oldgrog::TransformMatrix trans;
  grog::TransformMatrix ftrans;

#define PWET \
  std::cout << "FLOAT" << std::endl;\
  trans.print();\
  std::cout << "FIXED" << std::endl;\
  ftrans.print();

  PWET;

  trans.translate(1.234, 2.345, 4.567);
  ftrans.translate(grog::floatToFixed(1.234),
                   grog::floatToFixed(2.345),
                   grog::floatToFixed(4.567));

  PWET;

  trans.scale(1.4f, 5.6f, 2.7f);
  ftrans.scale(grog::floatToFixed(1.4),
               grog::floatToFixed(5.6),
               grog::floatToFixed(2.7));

  PWET;

  std::cout << "cos(0) " << cos(0) << " - " << ANGLE(0) << "/" << grog::Cos(ANGLE(0))/1024. << std::endl;
  std::cout << "cos(M_PI) " << cos(M_PI) << " - " << ANGLE(M_PI) << "/" << grog::Cos(ANGLE(M_PI))/1024. << std::endl;
  std::cout << "cos(2*M_PI) " << cos(2*M_PI) << " - " << ANGLE(2*M_PI) << "/" << grog::Cos(ANGLE(2*M_PI))/1024. << std::endl;
  std::cout << "sin(0) " << sin(0) << " - " << ANGLE(0) << "/" << grog::Sin(ANGLE(0))/1024. << std::endl;
  std::cout << "sin(M_PI) " << sin(M_PI) << " - " << ANGLE(M_PI) << "/" << grog::Sin(ANGLE(M_PI))/1024. << std::endl;
  std::cout << "sin(2*M_PI) " << sin(2*M_PI) << " - " << ANGLE(2*M_PI) << "/" << grog::Sin(ANGLE(2*M_PI))/1024. << std::endl;
  std::cout << "cos(1) " << cos(1.) << " - " << ANGLE(1.) << "/" << grog::Cos(ANGLE(1.))/1024. << std::endl;
  std::cout << "sin(1) " << sin(1.) << " - " << ANGLE(1.) << "/" << grog::Sin(ANGLE(1.))/1024. << std::endl;

  trans.rotateX(2.f);
  ftrans.rotateX(ANGLE(2.f));

  PWET;

  trans.rotateY(3.f);
  ftrans.rotateY(ANGLE(3.f));

  PWET;

  trans.rotateZ(5.f);
  ftrans.rotateZ(ANGLE(5.f));

  PWET;

  std::cout << "View FLOAT" << std::endl;
  oldgrog::TransformMatrix::View(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 0.f, 1.f, 2.f).print();
  std::cout << "View FIXED" << std::endl;
  grog::TransformMatrix::View(grog::floatToFixed(1.f), grog::floatToFixed(2.f), grog::floatToFixed(3.f),
                              grog::floatToFixed(4.f), grog::floatToFixed(5.f), grog::floatToFixed(6.f),
                              grog::floatToFixed(0.f), grog::floatToFixed(1.f), grog::floatToFixed(2.f)).print();

  oldgrog::Matrix proj;
  grog::Matrix fproj;

  proj = oldgrog::Matrix::Projection(2.f, 1.f, 10.f);
  fproj = grog::Matrix::Projection(2.f, 1.f, 10.f);

  std::cout << "Projection FLOAT" << std::endl;
  proj.print();
  std::cout << "Projection FIXED" << std::endl;
  fproj.print();

  oldgrog::Matrix res;
  grog::Matrix fres;

  std::cout << "Composition FLOAT" << std::endl;
  oldgrog::Matrix::Transform(proj, trans, res);
  res.print();
  std::cout << "Composition FIXED" << std::endl;
  grog::Matrix::Transform(fproj, ftrans, fres);
  fres.print();


//  std::cout.flush();

//  {
//    oldgrog::TransformMatrix trans2;

//    float xRot(1.), yRot(2.);

//    auto start = std::chrono::steady_clock::now();

//    for(unsigned int ii = 0; ii < 1000000; ++ii)
//    {
//      trans2.rotateX(xRot).rotateY(yRot);
//    }

//    auto end = std::chrono::steady_clock::now();

//    std::cout << "Float duration " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
//  }

//  {
//    grog::TransformMatrix trans2;

//    int32_t xRot(ANGLE(1.)), yRot(ANGLE(2.));

//    auto start = std::chrono::steady_clock::now();

//    for(unsigned int ii = 0; ii < 1000000; ++ii)
//    {
//      trans2.rotateX(xRot).rotateY(yRot);
//    }

//    auto end = std::chrono::steady_clock::now();

//    std::cout << "Fixed duration " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
//  }

//  int32_t testNorm[] {grog::floatToFixed(40.f), 0, 0};
//  std::cout << "Before:     [" << testNorm[0]/1024.f << "," << testNorm[1]/1024. << "," << testNorm[2]/1024. << "]" << std::endl;
//  grog::normalize(testNorm);
//  std::cout << "Normalized: [" << testNorm[0]/1024.f << "," << testNorm[1]/1024. << "," << testNorm[2]/1024. << "]" << std::endl;

  std::cout << "---- TRANS\n";
  float plop[] = {1.f, -2.5f, 6.f};
  int32_t fplop[] = {grog::floatToFixed(1.f),
                     grog::floatToFixed(-2.5f),
                     grog::floatToFixed(6.f)};

  {
    float inX = plop[0];
    float inY = plop[1];
    float inZ = plop[2];

    std::cout << "float [";
    std::cout <<  res.data[0] * inX +
                  res.data[1] * inY +
                  res.data[2] * inZ +
                  res.data[3] << ", ";
    std::cout <<  res.data[4] * inX +
                  res.data[5] * inY +
                  res.data[6] * inZ +
                  res.data[7] << ", ";
    std::cout <<  res.data[8] * inX +
                  res.data[9] * inY +
                  res.data[10] * inZ +
                  res.data[11] << "\n";
  }

  {
    int32_t inX = fplop[0];
    int32_t inY = fplop[1];
    int32_t inZ = fplop[2];

    std::cout << "fixed [";
    std::cout <<  ((fres.data[0] * inX +
                  fres.data[1] * inY +
                  fres.data[2] * inZ +
                  fres.data[3] * 1024) >> 10) / 1024.f << ", ";
    std::cout <<  ((fres.data[4] * inX +
                  fres.data[5] * inY +
                  fres.data[6] * inZ +
                  fres.data[7] * 1024) >> 10) / 1024.f << ", ";
    std::cout <<  ((fres.data[8] * inX +
                  fres.data[9] * inY +
                  fres.data[10] * inZ +
                  fres.data[11] * 1024) >> 10) / 1024.f << "\n";
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
//grog::Matrix::Projection(fov, near, far).print();
  engine.setProjection(grog::Matrix::Projection(fov, near, far));

//  grog::TransformMatrix::View(grog::floatToFixed(eyeX), grog::floatToFixed(eyeY), grog::floatToFixed(eyeZ),
//                                               grog::floatToFixed(targetX), grog::floatToFixed(targetY), grog::floatToFixed(targetZ),
//                                               grog::floatToFixed(upX), grog::floatToFixed(upY), grog::floatToFixed(upZ)).print();

  engine.setView(grog::TransformMatrix::View(grog::floatToFixed(eyeX), grog::floatToFixed(eyeY), grog::floatToFixed(eyeZ),
                                             grog::floatToFixed(targetX), grog::floatToFixed(targetY), grog::floatToFixed(targetZ),
                                             grog::floatToFixed(upX), grog::floatToFixed(upY), grog::floatToFixed(upZ)));

  engine.projectScene(&scene);

  engine.render();

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

  std::cerr << "Success opening " << fileName.toStdString() << std::endl;

  std::vector<int32_t> vertices;
  std::vector<uint32_t> faces;

  while(!file.atEnd())
  {
    QString line = QString(file.readLine().data());

    QStringList sl = line.split(" ");

    if(sl.size() == 4 && sl[0] == "v")
    {
      float vertexCoord = atof(sl[1].toStdString().data());
      vertices.push_back(grog::floatToFixed(vertexCoord));
      vertexCoord = atof(sl[2].toStdString().data());
      vertices.push_back(grog::floatToFixed(vertexCoord));
      vertexCoord = atof(sl[3].toStdString().data());
      vertices.push_back(grog::floatToFixed(vertexCoord));
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

  std::cerr << "Success parsing " << fileName.toStdString() << std::endl;

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


