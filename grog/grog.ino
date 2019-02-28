#include <Gamebuino-Meta.h>
#include "Engine.h"
#include "Colors.h"

grog::Engine engine;

void setup()
{
  gb.begin();
  // We aren't using the normal screen buffer, so initialize it to 0px × 0px.
  gb.display.init(0, 0, ColorMode::index);

  // Just to push things to the limit for this example, increase to 40fps.
  gb.setFrameRate(25);

  engine.init(10, 14, 3);
}

void loop()
{
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

  engine.setView(grog::TransformMatrix::View(0, 0, 5,
                                             0, 0, 0,
                                             0, 1, 0));

  engine.setProjection(grog::Matrix::Projection(2.f, 1, 10));

  int ii = 0;

  while(true)
  {
    while(!gb.update());

    engine.setView(grog::TransformMatrix::View(0, 0, 1,
                                               0, 0, 0,
                                               0, 1, 0));

    cube.transform.identity()
                  .scale(0.5, 0.5, 0.5)
                  .rotateX(ii/20.f)
                  .rotateY(ii/15.f)/*
                  .translate(40, 40, 40)*/;

    engine.projectScene(&cube);

    engine.render();

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());
  }
}
