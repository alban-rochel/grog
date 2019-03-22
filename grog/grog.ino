#include <Gamebuino-Meta.h>
#include "Engine.h"
#include "Colors.h"
#include "Car.h"

#include "trigo.h"

grog::Engine engine;
grog::Car* car1;
grog::Car* car2;


void setup()
{
  gb.begin();
  // We aren't using the normal screen buffer, so initialize it to 0px × 0px.
  gb.display.init(0, 0, ColorMode::index);

  gb.setFrameRate(25);

  engine.init(100, 100, 3);
}

void loop()
{
  grog::SceneNode scene;

  scene.mesh.vertexBuffer = nullptr;
  scene.mesh.vertexCount = 0;
  scene.mesh.faces = nullptr;
  scene.mesh.faceCount = 0;
  scene.mesh.colors = nullptr;
  scene.children = new grog::SceneNode*[2];
  scene.childCount = 2;

  car1=new grog::Car(true);
  car2=new grog::Car(false);
  scene.children[0] = car1;
  scene.children[1] = car2;
  scene.children[1]->transform.identity().translate(1500,0, 1000);
  
  engine.setProjection(grog::Matrix::Projection(1.f, 1, 100));

  int ii = 0;

  while(true)
  {
    while(!gb.update());

    engine.setView(grog::TransformMatrix::View(grog::floatToFixed(-1.f), grog::floatToFixed(0.5f), grog::floatToFixed(-0.4f),
                                               0, 0, 0,
                                               grog::floatToFixed(0), grog::floatToFixed(-1.f), 0));

    scene.transform.identity();
          //.scale(1 << 10, 1 << 10, 1 << 10)
          /*.rotateX(ii << 2)
          .rotateY(ii << 1);*/
scene.transform.rotateY(2*ii);

car1->setWheelRotation(ii << 6);
car2->setWheelRotation(ii << 6);

    engine.projectScene(&scene);

    engine.render();

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());

  }
}
