#include "grog.h"
#include "Car.h"
#include "Road.h"

grog::Engine engine;
demo::Car* car1;
demo::Car* car2;


void setup()
{
  gb.begin();
  // We aren't using the normal screen buffer, so initialize it to 0px × 0px.
  //gb.display.init(0, 0, ColorMode::index);
  gb.display.init(80, 64, ColorMode::index);

  gb.setFrameRate(25);

  engine.init(39, 100);
}

void loop()
{
  grog::SceneNode scene;

  scene.mesh.vertexBuffer = nullptr;
  scene.mesh.vertexCount = 0;
  scene.mesh.faces = nullptr;
  scene.mesh.faceCount = 0;
  scene.mesh.colors = nullptr;
  scene.children = new grog::SceneNode*[7];
  scene.childCount = 5;

  car1=new demo::Car(true);
  car2=new demo::Car(false);
  scene.children[0] = car1;
  scene.children[1] = car2;
  scene.children[1]->transform.identity().translate(1500,0, 1000);

  scene.children[2] = new demo::Road();
  scene.children[3] = new demo::Road();
  scene.children[4] = new demo::Road();
  
  engine.setProjection(1.f, 0.01, 100);

  int ii = 0;

  int32_t shift[] = {-16000, 0, 16000};

  int32_t azimut = 60;
  int32_t tilt = 50;
  int32_t distance = 2 << 3;

  while(true)
  {
    while(!gb.update());
    if(gb.buttons.repeat(BUTTON_UP, 0))
    {
      tilt -= 5;
    }
    else if(gb.buttons.repeat(BUTTON_DOWN, 0))
    {
      tilt += 5;
    }

    if(gb.buttons.repeat(BUTTON_LEFT, 0))
    {
      azimut -= 5;
    }
    else if(gb.buttons.repeat(BUTTON_RIGHT, 0))
    {
      azimut += 5;
    }

    if(gb.buttons.repeat(BUTTON_A, 0))
    {
      distance -= 1;
    }
    else if(gb.buttons.repeat(BUTTON_B, 0))
    {
      distance += 1;
    }

    tilt = grog::Math::Min2(grog::Math::Max2(50, tilt), 200);
    distance = grog::Math::Min2(grog::Math::Max2(2 << 3, distance), 10 << 3);

    gb.display.setColor(Gamebuino_Meta::ColorIndex::black);
    gb.display._fill();

    engine.setView(grog::TransformMatrix::View((distance * grog::Math::Cos(azimut)) >> 3, (distance * grog::Cos(tilt)) >> 3, (distance * grog::Sin(azimut)) >> 3,
                                               0, 0, 0,
                                               grog::Math::FloatToFixed(0), grog::Math::floatToFixed(-1.f), 0));


  for(uint32_t roadIndex = 0; roadIndex < 5; ++roadIndex)
  {
    scene.children[2 + roadIndex]->transform.identity().translate(shift[roadIndex], 0, 0);
    shift[roadIndex] += 300;
    if(shift[roadIndex] >= 32000)
      shift[roadIndex] -= 48000;
  }
  car1->setWheelRotation(ii << 6);
  car2->setWheelRotation(ii << 6);

    engine.projectScene(&scene, 0x01);
    engine.projectScene(&scene, 0x02);

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());

  }
}
