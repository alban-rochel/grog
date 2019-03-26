#include "Car.h"
#include "asset_car.h"

using namespace grog;

Car::Car(bool mainCar):
  SceneNode()
{
  renderPass = 0x02;
  mesh.vertexBuffer = grog::car_vertices;
  mesh.vertexCount = grog::car_vertexCount;
  mesh.faces = grog::car_faces;
  mesh.faceCount = grog::car_faceCount;
  if(mainCar)
  {
    mesh.colors = grog::car_colors_main;
  }
  else
  {
    mesh.colors = grog::car_colors_other;
  }

  childCount = 4;
  children = new grog::SceneNode*[childCount];

  for(uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
  {
    children[childIndex] = new grog::SceneNode;
    children[childIndex]->mesh.vertexBuffer = grog::wheel_vertices;
    children[childIndex]->mesh.vertexCount = grog::wheel_vertexCount;
    children[childIndex]->mesh.faces = grog::wheel_faces;
    children[childIndex]->mesh.faceCount = grog::wheel_faceCount;
    children[childIndex]->mesh.colors = grog::wheel_colors;
    children[childIndex]->renderPass = 0x02;
  }

  setWheelRotation(0);
}

void Car::setWheelRotation(int32_t rotation) noexcept
{
    children[0]->transform.identity().rotateZ(rotation).translate(-300, 50, 200);
    children[1]->transform.identity().rotateZ(rotation).translate(-300, 50, -200);
    children[2]->transform.identity().rotateZ(rotation).translate(600, 50, 200);
    children[3]->transform.identity().rotateZ(rotation).translate(600, 50, -200);
}
