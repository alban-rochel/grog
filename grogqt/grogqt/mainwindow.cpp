#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trigo.h"
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

  //debug();

  engine.init(100, 100, &pix);

#ifdef DEBUG_ALBAN
  static const int32_t vertexBuffer [] =
  {
    grog::floatToFixed(0), grog::floatToFixed(0), grog::floatToFixed(0),
    grog::floatToFixed(20), grog::floatToFixed(-5), grog::floatToFixed(0),
    grog::floatToFixed(5), grog::floatToFixed(5), grog::floatToFixed(0),
  };

  static const uint32_t faces [] =
  {
    0, 1, 2
  };

  static const Gamebuino_Meta::ColorIndex colors [] =
  {
    Gamebuino_Meta::ColorIndex::white
  };

  scene.mesh.vertexBuffer = vertexBuffer;
  scene.mesh.vertexCount = 3;
  scene.mesh.faces = faces;
  scene.mesh.faceCount = 1;
  scene.mesh.colors = colors;
  scene.children = nullptr;
  scene.childCount = 0;
#else
  scene.mesh.vertexBuffer = nullptr;
  scene.mesh.vertexCount = 0;
  scene.mesh.faces = nullptr;
  scene.mesh.faceCount = 0;
  scene.mesh.colors = nullptr;
  scene.children = new grog::SceneNode*[7];
  scene.childCount = 7;

  scene.children[0] = new grog::Car(true);
  scene.children[1] = new grog::Car(false);
  scene.children[1]->transform.identity().translate(1500,0, 1000);

  scene.children[2] = new grog::Road();
  scene.children[3] = new grog::Road();
  scene.children[3]->transform.identity().translate(48000, 0, 0);
  scene.children[4] = new grog::Road();
  scene.children[4]->transform.identity().translate(16000, 0, 0);
  scene.children[5] = new grog::Road();
  scene.children[5]->transform.identity().translate(-16000, 0, 0);
  scene.children[6] = new grog::Road();
  scene.children[6]->transform.identity().translate(-48000, 0, 0);
#endif


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
//  draw();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::step()
{
  static int32_t shift[] = {-32000, -16000, 0, 16000, 32000};
  for(uint32_t roadIndex = 0; roadIndex < 5; ++roadIndex)
  {
    scene.children[2 + roadIndex]->transform.identity().translate(shift[roadIndex], 0, 0);
    shift[roadIndex] += 300;
    if(shift[roadIndex] >= 48000)
      shift[roadIndex] -= 80000;
  }

  draw();
}

void MainWindow::debug()
{
  grog::Matrix projection = grog::Matrix::Projection(1.f, 1.f, 10.f);

  std::cout << "Projection\n";
  projection.print();

  grog::TransformMatrix view = grog::TransformMatrix::View(0, 0, 0,
                                                           0, 0, 1024,
                                                           0, 1024, 0);

  std::cout << "View\n";
  view.print();

  int32_t p_in_10[] = {grog::floatToFixed(1.f),
                    grog::floatToFixed(0.2f),
                   grog::floatToFixed(8.f),
                   grog::floatToFixed(1.f)};

  int32_t p_out_10[] = {grog::floatToFixed(12.f),
                    grog::floatToFixed(0.8f),
                   grog::floatToFixed(-3.f),
                   grog::floatToFixed(1.f)};

#define PVECT(v) std::cout << "[" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] <<"] = [" << \
                            v[0]/1024.f << ", " << v[1]/1024.f << ", " << v[2]/1024.f << ", " << v[3]/1024.f << "]\n";
#define PVECT2(v) std::cout << "[" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] <<"] = [" << \
                            v[0]/40.f/1024.f/1024.f << ", " << v[1]/32.f/1024.f/1024.f << ", " << v[2]/1024.f/1024.f << ", " << v[3]/1024.f/1024.f << "]\n";

  std::cout << "p_in\n";
  PVECT(p_in_10);
  std::cout << "p_out\n";
  PVECT(p_out_10);

  grog::Matrix mvp;
  grog::Matrix::Transform(projection, view, mvp);

  int32_t p_in_trans_20_shift[4];
  int32_t inX = p_in_10[0];
  int32_t inY = p_in_10[1];
  int32_t inZ = p_in_10[2];

  p_in_trans_20_shift[0] = mvp.data[0] * inX
      +  mvp.data[1] * inY
      +  mvp.data[2] * inZ
      +  mvp.data[3] * 1024;

  p_in_trans_20_shift[1] = mvp.data[4] * inX
      +  mvp.data[5] * inY
      +  mvp.data[6] * inZ
      +  mvp.data[7] * 1024;

  p_in_trans_20_shift[2] = mvp.data[8] * inX
      +  mvp.data[9] * inY
      +  mvp.data[10] * inZ
      +  mvp.data[11] * 1024;

  p_in_trans_20_shift[3] = mvp.data[12] * inX
      +  mvp.data[13] * inY
      +  mvp.data[14] * inZ
      +  mvp.data[15] * 1024;

  std::cout << "p_in_trans ";
  PVECT2(p_in_trans_20_shift);

  int32_t p_out_trans_20_shift[4];
  inX = p_out_10[0];
  inY = p_out_10[1];
  inZ = p_out_10[2];

  p_out_trans_20_shift[0] = mvp.data[0] * inX
      +  mvp.data[1] * inY
      +  mvp.data[2] * inZ
      +  mvp.data[3] * 1024;

  p_out_trans_20_shift[1] = mvp.data[4] * inX
      +  mvp.data[5] * inY
      +  mvp.data[6] * inZ
      +  mvp.data[7] * 1024;

  p_out_trans_20_shift[2] = mvp.data[8] * inX
      +  mvp.data[9] * inY
      +  mvp.data[10] * inZ
      +  mvp.data[11] * 1024;

  p_out_trans_20_shift[3] = mvp.data[12] * inX
      +  mvp.data[13] * inY
      +  mvp.data[14] * inZ
      +  mvp.data[15] * 1024;

  std::cout << "p_out_trans ";
PVECT2(p_out_trans_20_shift);

  // p_out_trans < -1024


  int32_t gamma_num = - p_out_trans_20_shift[3] - p_out_trans_20_shift[2];
  int32_t gamma_den = p_in_trans_20_shift[2] - p_out_trans_20_shift[2] + p_in_trans_20_shift[3] - p_out_trans_20_shift[3];

  float gamma =  gamma_num/(float)gamma_den;
  std::cout << "Gamma " << gamma << " = " << gamma_num << "/" << gamma_den <<std::endl;
  int32_t p_res_20_shift[4];

  p_res_20_shift[0] = gamma_num * ((p_in_trans_20_shift[0] - p_out_trans_20_shift[0]) / gamma_den) + p_out_trans_20_shift[0];
  p_res_20_shift[1] = gamma_num * ((p_in_trans_20_shift[1] - p_out_trans_20_shift[1]) / gamma_den) + p_out_trans_20_shift[1];
  p_res_20_shift[2] = gamma_num * ((p_in_trans_20_shift[2] - p_out_trans_20_shift[2]) / gamma_den) + p_out_trans_20_shift[2];
  p_res_20_shift[3] = -p_res_20_shift[2];
  std::cout << p_in_trans_20_shift[0] << "/40 = " << (p_in_trans_20_shift[0])/40. << "/1024/1024=" << (p_in_trans_20_shift[0])/40./1024./1024. << std::endl;
  std::cout << p_out_trans_20_shift[0] << "/40 = " << (p_out_trans_20_shift[0])/40. << "/1024/1024=" << (p_out_trans_20_shift[0])/40./1024./1024. << std::endl;

  std::cout << "Res\n";
  PVECT2(p_res_20_shift);
  /*std::cout << "{" << p_res[0]/(float)p_res[3] << ", "
                   << p_res[1]/(float)p_res[3] << ", "
                   << p_res[2]/(float)p_res[3] << "}\n";*/

//  int32_t p_in_proj[3];
//  p_in_proj[0] = (p_in_trans[0]/p_in_trans[3])/* >> 10*/;
//  p_in_proj[1] = (p_in_trans[1]/p_in_trans[3])/* >> 10*/;
//  p_in_proj[2] = (p_in_trans[2]/p_in_trans[3])/* >> 10*/;

//  std::cout << "In proj (" << p_in_proj[0] << ", " << p_in_proj[1] << ", " << p_in_proj[2] << ")\n";

//  int32_t p_res_proj[3];
//  p_res_proj[0] = (p_res[0]/p_res[3])/1024.;
//  p_res_proj[1] = (p_res[1]/p_res[3])/1024.;
//  p_res_proj[2] = (p_res[2]/p_res[3])/1024.;
//  std::cout << "Res proj (" << p_res_proj[0] << ", " << p_res_proj[1] << ", " << p_res_proj[2] << ")\n";

  exit(0);
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

void plop()
{
  grog::Matrix proj = grog::Matrix::Projection(1., 0.5f, 10.f);
  std::cout << "Projection\n";
  proj.print();

  grog::TransformMatrix view = grog::TransformMatrix::View(0, 0, 0,
                                                           0, 0, 1024,
                                                           0, 1024, 0);
  std::cout << "View\n";
  view.print();

  grog::Matrix mvp;
  grog::Matrix::Transform(proj, view, mvp);

  ///// NEAR
  int32_t inX(0), inY(0), inZ(512);
  int32_t w = (mvp.data[12] * inX
      +  mvp.data[13] * inY
      +  mvp.data[14] * inZ
      +  mvp.data[15] * 1024) >> 10;

//    int32_t z = (((mvp.data[8] * inX
//                  +  mvp.data[9] * inY
//                 +  mvp.data[10] * inZ
//+  mvp.data[11] * 1024)*1000)/w) >> 10;
  int32_t z = (mvp.data[8] * inX
                +  mvp.data[9] * inY
               +  mvp.data[10] * inZ
+  mvp.data[11] * 1024)/w;
  std::cout << "w near " << w << " - z " << z << std::endl;

  ///// FAR
  inZ = 10*1024;
  w = (mvp.data[12] * inX
      +  mvp.data[13] * inY
      +  mvp.data[14] * inZ
      +  mvp.data[15] * 1024) >> 10;

//    z = (((mvp.data[8] * inX
//                  +  mvp.data[9] * inY
//                 +  mvp.data[10] * inZ
//+  mvp.data[11] * 1024)*1000)/w) >> 10;
  z = (mvp.data[8] * inX
                +  mvp.data[9] * inY
               +  mvp.data[10] * inZ
+  mvp.data[11] * 1024)/w;
  std::cout << "w far " << w << " - z " << z << std::endl;

  exit(0);
}

void MainWindow::draw()
{
  engine.setProjection(fov, near, far);

  engine.setView(grog::TransformMatrix::View(grog::floatToFixed(eyeX), grog::floatToFixed(eyeY), grog::floatToFixed(eyeZ),
                                             grog::floatToFixed(targetX), grog::floatToFixed(targetY), grog::floatToFixed(targetZ),
                                             grog::floatToFixed(upX), grog::floatToFixed(upY), grog::floatToFixed(upZ)));

  engine.projectScene(&scene, 0x01);
  engine.projectScene(&scene, 0x02);

  engine.display.draw();

  QPixmap coincoin = pix.scaled(320, 256);

  ui->label->setPixmap(coincoin);

  QApplication::processEvents();
}

//void MainWindow::draw()
//{
//  engine.setProjection(grog::Matrix::Projection(1.f, 1, 10));

////  static int ii = 228;

//  //while(true)
//  {
//    std::this_thread::sleep_for(std::chrono::milliseconds(40));

//    engine.setView(grog::TransformMatrix::View(grog::floatToFixed(-1.f), grog::floatToFixed(0.5f), grog::floatToFixed(-0.4f),
//                                               0, 0, 0,
//                                               grog::floatToFixed(0), grog::floatToFixed(-1.f), 0));

//    scene.transform.identity();
//          //.scale(1 << 10, 1 << 10, 1 << 10)
//          /*.rotateX(ii << 2)
//          .rotateY(ii << 1);*/
//    scene.transform.rotateY(ii/100.*512);

//    scene.children[0].transform.identity().rotateZ(ii*50).translate(-300, 50, 200);
//    scene.children[1].transform.identity().rotateZ(ii*50).translate(-300, 50, -200);
//    scene.children[2].transform.identity().rotateZ(ii*50).translate(600, 50, 200);
//    scene.children[3].transform.identity().rotateZ(ii*50).translate(600, 50, -200);

//    engine.projectScene(&scene);

////    engine.debugTriangleStack();

//    engine.render();

//    ++ii;
//    QPixmap coincoin = pix.scaled(320, 256);

//    ui->label->setPixmap(coincoin);

//    QApplication::processEvents();

//  }

//}

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


