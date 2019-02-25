#define DISPLAY_CONSTRUCTOR Image(0, 0, ColorMode::rgb565)
#include <Gamebuino-Meta.h>
#include "Engine.h"

void setup()
{
  gb.begin();
  //gb.display.init(80, 64, ColorMode::index);
  //ii = 0;
}

void loop()
{

  gb.display.clear();
  //gb.display._buffer[0] = 1;
  //gb.display._buffer[ii] = 0xFFFF;
  //SerialUSB.printf("%i %d\n", gb.display.bufferSize, millis());
  //++ii;


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
  const uint8_t colors[] {1, 1,
                          2, 2,
                          3, 3,
                          4, 4,
                          5, 5,
                          6, 6};

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
    gb.display.clear();


    engine.setView(grog::TransformMatrix::View(0, 0, 1,
                                               0, 0, 0,
                                               0, 1, 0));

    cube.transform.identity()
                  .scale(0.5, 0.5, 0.5)
                  .rotateX(ii/20.f)
                  .rotateY(ii/15.f)/*
                  .translate(40, 40, 40)*/;

    engine.projectScene(&cube);

    engine.render(gb.display._buffer);

    ++ii;

  uint8_t load = gb.getCpuLoad();
  gb.display.print("CPU:");
  gb.display.print(load);
  gb.display.println("%");
  }
}
