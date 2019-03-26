#include "Car.h"
#include "asset_car.h"

using namespace grog;

Car::Car(bool mainCar):
  SceneNode()
{
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

  children = new grog::SceneNode*[4];
  childCount = 4;

  children[0] = new grog::SceneNode;
  children[0]->mesh.vertexBuffer = grog::wheel_vertices;
  children[0]->mesh.vertexCount = grog::wheel_vertexCount;
  children[0]->mesh.faces = grog::wheel_faces;
  children[0]->mesh.faceCount = grog::wheel_faceCount;
  children[0]->mesh.colors = grog::wheel_colors;

  children[1] = new grog::SceneNode;
  children[1]->mesh.vertexBuffer = grog::wheel_vertices;
  children[1]->mesh.vertexCount = grog::wheel_vertexCount;
  children[1]->mesh.faces = grog::wheel_faces;
  children[1]->mesh.faceCount = grog::wheel_faceCount;
  children[1]->mesh.colors = grog::wheel_colors;

  children[2] = new grog::SceneNode;
  children[2]->mesh.vertexBuffer = grog::wheel_vertices;
  children[2]->mesh.vertexCount = grog::wheel_vertexCount;
  children[2]->mesh.faces = grog::wheel_faces;
  children[2]->mesh.faceCount = grog::wheel_faceCount;
  children[2]->mesh.colors = grog::wheel_colors;

  children[3] = new grog::SceneNode;
  children[3]->mesh.vertexBuffer = grog::wheel_vertices;
  children[3]->mesh.vertexCount = grog::wheel_vertexCount;
  children[3]->mesh.faces = grog::wheel_faces;
  children[3]->mesh.faceCount = grog::wheel_faceCount;
  children[3]->mesh.colors = grog::wheel_colors;

  setWheelRotation(0);
}

void Car::setWheelRotation(int32_t rotation) noexcept
{
    children[0]->transform.identity().rotateZ(rotation).translate(-300, 50, 200);
    children[1]->transform.identity().rotateZ(rotation).translate(-300, 50, -200);
    children[2]->transform.identity().rotateZ(rotation).translate(600, 50, 200);
    children[3]->transform.identity().rotateZ(rotation).translate(600, 50, -200);
}
