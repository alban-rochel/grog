#include <Gamebuino-Meta.h>
#include "Engine.h"
#include "Colors.h"

grog::Engine engine;

const grog::coord vertices[] {
  -1,  -1, -1,
  -1,  -1,  1,
  -1,   1, -1,
  -1,   1,  1,
  1,  -1,  -1,
  1,  -1,  1,
  1,   1,  -1,
  1,   1,  1
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
  grog::color(grog::Color::White, grog::Color::DarkBlue), grog::color(grog::Color::White, grog::Color::DarkBlue),
  grog::color(grog::Color::Gray, grog::Color::DarkBlue), grog::color(grog::Color::Gray, grog::Color::DarkBlue),
  grog::color(grog::Color::DarkGray, grog::Color::DarkBlue), grog::color(grog::Color::DarkGray, grog::Color::DarkBlue)
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

void setup()
{
  gb.begin();
  // We aren't using the normal screen buffer, so initialize it to 0px × 0px.
  gb.display.init(0, 0, ColorMode::index);

  gb.setFrameRate(25);

  engine.init(10, 100, 3);
}

void loop()
{
  const grog::coord vertices[] {-1,  -1, -1,
                                -1,  -1,  1,
                                -1,   1, -1,
                                -1,   1,  1,
                                 1,  -1,  -1,
                                 1,  -1,  1,
                                 1,   1,  -1,
                                 1,   1,  1};
  const uint32_t faces[] {0, 6, 4,
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
                          1, 7, 3};
  const uint8_t colors[] {grog::color(grog::Color::White), grog::color(grog::Color::White),
                          grog::color(grog::Color::Gray), grog::color(grog::Color::Gray),
                          grog::color(grog::Color::DarkGray), grog::color(grog::Color::DarkGray),
                          grog::color(grog::Color::White, grog::Color::DarkBlue), grog::color(grog::Color::White, grog::Color::DarkBlue),
                          grog::color(grog::Color::Gray, grog::Color::DarkBlue), grog::color(grog::Color::Gray, grog::Color::DarkBlue),
                          grog::color(grog::Color::DarkGray, grog::Color::DarkBlue), grog::color(grog::Color::DarkGray, grog::Color::DarkBlue)};

  grog::SceneNode scene;

  scene.mesh.vertexBuffer = vertices;
  scene.mesh.vertexCount = 8;
  scene.mesh.faces = faces;
  scene.mesh.faceCount = 12;
  scene.mesh.colors = colors;

  scene.children = new grog::SceneNode[3];
  scene.childCount = 3;

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

  scene.children[0].transform.identity().translate(2, 0, 0).scale(0.5, 0.5, 0.5);
  scene.children[1].transform.identity().translate(0, 2, 0).scale(0.5, 0.5, 0.5);
  scene.children[2].transform.identity().translate(0, 0, 2).scale(0.5, 0.5, 0.5);

  engine.setProjection(grog::Matrix::Projection(2.f, 1, 10));

  int ii = 0;

  while(true)
  {
    while(!gb.update());

    engine.setView(grog::TransformMatrix::View(1, 1, 1.,
                                               0, 0, 0,
                                               0, 1, 0));

    scene.transform.identity()
          .scale(0.5, 0.5, 0.5)
          .rotateX(ii/20.f)
          .rotateY(ii/15.f)/*
                          .translate(40, 40, 40)*/;

    engine.projectScene(&scene);

    engine.render();

    ++ii;

    SerialUSB.printf("%d %d\n", gb.getCpuLoad(), gb.getFreeRam());
  }
}
