#include "Math.h"
#include "Engine.h"
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

  scene.m_mesh.m_vertexBuffer = nullptr;
  scene.m_mesh.m_vertexCount = 0;
  scene.m_mesh.m_faces = nullptr;
  scene.m_mesh.m_faceCount = 0;
  scene.m_mesh.m_colors = nullptr;
  scene.m_children = new grog::SceneNode*[5];
  scene.m_childCount = 5;

  car1=new demo::Car(true);
  car2=new demo::Car(false);
  scene.m_children[0] = car1;
  scene.m_children[1] = car2;
  scene.m_children[1]->m_transform.identity().translate(1500,0, 1000);

  scene.m_children[2] = new demo::Road();
  scene.m_children[3] = new demo::Road();
  scene.m_children[4] = new demo::Road();
  
  engine.setProjection(grog::Matrix::Projection(1.f, 0.01f, 100.f));

  int ii = 0;

  int32_t shift[] = {-(1<<14), 0, (1<<14)};

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

    tilt = (tilt < 50 ? 50 : tilt);
    tilt = (tilt > 200 ? 200 : tilt);
    distance = (distance < (2<<3) ? (2<<3) : distance);
    distance = (distance > (10<<3) ? (10<<3) : distance);

    gb.display.setColor(Gamebuino_Meta::ColorIndex::black);
    gb.display._fill();

    engine.setView(grog::TransformMatrix::View((distance * grog::Math::Cos(azimut)) >> 3, (distance * grog::Math::Cos(tilt)) >> 3, (distance * grog::Math::Sin(azimut)) >> 3,
                                               0, 0, 0,
                                               grog::Math::FloatToFixed(0.f), grog::Math::FloatToFixed(-1.f), 0));


  for(uint32_t roadIndex = 0; roadIndex < 3; ++roadIndex)
  {
    scene.m_children[2 + roadIndex]->m_transform.identity().translate(shift[roadIndex], 0, 0);
    shift[roadIndex] += 300;
    if(shift[roadIndex] >= (1<<15))
      shift[roadIndex] -= (1<<15)+(1<<14);
  }
  car1->setWheelRotation(ii << 6);
  car2->setWheelRotation(ii << 6);

    engine.projectScene(&scene, 0x01);
    engine.projectScene(&scene, 0x02);

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());

  }
}
