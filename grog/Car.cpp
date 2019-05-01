#include "Car.h"
#include "asset_car.h"

using namespace demo;

Car::Car(bool mainCar):
  grog::SceneNode()
{
  renderPass = 0x02;
  mesh.vertexBuffer = demo::car_vertices;
  mesh.vertexCount = demo::car_vertexCount;
  mesh.faces = demo::car_faces;
  mesh.faceCount = demo::car_faceCount;
  if(mainCar)
  {
    mesh.colors = demo::car_colors_main;
  }
  else
  {
    mesh.colors = demo::car_colors_other;
  }

  childCount = 4;
  children = new grog::SceneNode*[childCount];

  for(uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
  {
    children[childIndex] = new grog::SceneNode;
    children[childIndex]->mesh.vertexBuffer = demo::wheel_vertices;
    children[childIndex]->mesh.vertexCount = demo::wheel_vertexCount;
    children[childIndex]->mesh.faces = demo::wheel_faces;
    children[childIndex]->mesh.faceCount = demo::wheel_faceCount;
    children[childIndex]->mesh.colors = demo::wheel_colors;
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
