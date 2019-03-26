#include <Gamebuino-Meta.h>
#include "Engine.h"
#include "Colors.h"
#include "Car.h"
#include "Road.h"

#include "trigo.h"

grog::Engine engine;
grog::Car* car1;
grog::Car* car2;


void setup()
{
  gb.begin();
  // We aren't using the normal screen buffer, so initialize it to 0px × 0px.
  //gb.display.init(0, 0, ColorMode::index);
  gb.display.init(80, 64, ColorMode::index);

  gb.setFrameRate(25);

  engine.init(39, 100, 3);
}

void loop()
{
  grog::SceneNode scene_bg;
  grog::SceneNode scene_cars;

  scene_cars.mesh.vertexBuffer = nullptr;
  scene_cars.mesh.vertexCount = 0;
  scene_cars.mesh.faces = nullptr;
  scene_cars.mesh.faceCount = 0;
  scene_cars.mesh.colors = nullptr;
  scene_cars.children = new grog::SceneNode*[2];
  scene_cars.childCount = 2;

  car1=new grog::Car(true);
  car2=new grog::Car(false);
  scene_cars.children[0] = car1;
  scene_cars.children[1] = car2;
  scene_cars.children[1]->transform.identity().translate(1500,0, 1000);

    scene_bg.mesh.vertexBuffer = nullptr;
  scene_bg.mesh.vertexCount = 0;
  scene_bg.mesh.faces = nullptr;
  scene_bg.mesh.faceCount = 0;
  scene_bg.mesh.colors = nullptr;
  scene_bg.children = new grog::SceneNode*[1];
  scene_bg.childCount = 1;

  scene_bg.children[0] = new grog::Road();
  
  engine.setProjection(grog::Matrix::Projection(1.f, 1, 100));

  int ii = 0;

  while(true)
  {
    while(!gb.update());

    gb.display.setColor(Gamebuino_Meta::ColorIndex::black);
    gb.display._fill();

    engine.setView(grog::TransformMatrix::View(grog::floatToFixed(-1.f), grog::floatToFixed(0.5f), grog::floatToFixed(-0.4f),
                                               0, 0, 0,
                                               grog::floatToFixed(0), grog::floatToFixed(-1.f), 0));

    scene_cars.transform.identity();
    scene_cars.transform.rotateY(2*ii);
    scene_bg.transform.identity();
    scene_bg.transform.rotateY(2*ii);

car1->setWheelRotation(ii << 6);
car2->setWheelRotation(ii << 6);

    engine.projectScene(&scene_bg);

    engine.render(false);

    engine.projectScene(&scene_cars);

    engine.render(true);

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());

  }
}
