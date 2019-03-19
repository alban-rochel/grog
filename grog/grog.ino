#include <Gamebuino-Meta.h>
#include "Engine.h"
#include "Colors.h"
#include "asset_car.h"

#include "trigo.h"

grog::Engine engine;


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

  scene.mesh.vertexBuffer = grog::car_vertices;
  scene.mesh.vertexCount = grog::car_vertexCount;
  scene.mesh.faces = grog::car_faces;
  scene.mesh.faceCount = grog::car_faceCount;
  scene.mesh.colors = grog::car_colors;

  scene.children = new grog::SceneNode[4];
  scene.childCount = 4;

  scene.children[0].mesh.vertexBuffer = grog::wheel_vertices;
  scene.children[0].mesh.vertexCount = grog::wheel_vertexCount;
  scene.children[0].mesh.faces = grog::wheel_faces;
  scene.children[0].mesh.faceCount = grog::wheel_faceCount;
  scene.children[0].mesh.colors = grog::wheel_colors;

  scene.children[1].mesh.vertexBuffer = grog::wheel_vertices;
  scene.children[1].mesh.vertexCount = grog::wheel_vertexCount;
  scene.children[1].mesh.faces = grog::wheel_faces;
  scene.children[1].mesh.faceCount = grog::wheel_faceCount;
  scene.children[1].mesh.colors = grog::wheel_colors;

  scene.children[2].mesh.vertexBuffer = grog::wheel_vertices;
  scene.children[2].mesh.vertexCount = grog::wheel_vertexCount;
  scene.children[2].mesh.faces = grog::wheel_faces;
  scene.children[2].mesh.faceCount = grog::wheel_faceCount;
  scene.children[2].mesh.colors = grog::wheel_colors;

  scene.children[3].mesh.vertexBuffer = grog::wheel_vertices;
  scene.children[3].mesh.vertexCount = grog::wheel_vertexCount;
  scene.children[3].mesh.faces = grog::wheel_faces;
  scene.children[3].mesh.faceCount = grog::wheel_faceCount;
  scene.children[3].mesh.colors = grog::wheel_colors;

  scene.children[0].transform.identity().translate(-300, 50, 200);
  scene.children[1].transform.identity().translate(-300, 50, -200);
  scene.children[2].transform.identity().translate(600, 50, 200);
  scene.children[3].transform.identity().translate(600, 50, -200);

  engine.setProjection(grog::Matrix::Projection(1.f, 1, 10));

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
scene.transform.rotateY(ii);
    scene.children[0].transform.identity().rotateZ(ii*50).translate(-300, 50, 200);
    scene.children[1].transform.identity().rotateZ(ii*50).translate(-300, 50, -200);
    scene.children[2].transform.identity().rotateZ(ii*50).translate(600, 50, 200);
    scene.children[3].transform.identity().rotateZ(ii*50).translate(600, 50, -200);

    engine.projectScene(&scene);

    engine.render();

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());

  }
}
