#include "Car.h"
#include "asset_car.h"

using namespace demo;

Car::Car(bool mainCar):
  grog::SceneNode()
{
  // Cars are rendered in second pass.
  // This makes them appear on top of everything rasterized in first pass, but saves memory.
  m_renderPass = 0x02;

  // The main node is the car body itself
  m_mesh.m_vertexBuffer = demo::car_vertices;
  m_mesh.m_vertexCount  = demo::car_vertexCount;
  m_mesh.m_faces        = demo::car_faces;
  m_mesh.m_faceCount    = demo::car_faceCount;
  if(mainCar)
  {
    m_mesh.m_colors     = demo::car_colors_main;
  }
  else
  {
    m_mesh.m_colors     = demo::car_colors_other;
  }

  // The children are the wheels
  m_childCount = 4;
  m_children = new grog::SceneNode*[m_childCount];

  for(uint32_t childIndex = 0; childIndex < m_childCount; ++childIndex)
  {
    m_children[childIndex] = new grog::SceneNode;
    m_children[childIndex]->m_mesh.m_vertexBuffer = demo::wheel_vertices;
    m_children[childIndex]->m_mesh.m_vertexCount  = demo::wheel_vertexCount;
    m_children[childIndex]->m_mesh.m_faces        = demo::wheel_faces;
    m_children[childIndex]->m_mesh.m_faceCount    = demo::wheel_faceCount;
    m_children[childIndex]->m_mesh.m_colors       = demo::wheel_colors;
    m_children[childIndex]->m_renderPass        = 0x02;
  }

  setWheelRotation(0);
}

void Car::setWheelRotation(int32_t rotation) noexcept
{
    m_children[0]->m_transform.identity().rotateZ(rotation).translate(-300, 50, 200);
    m_children[1]->m_transform.identity().rotateZ(rotation).translate(-300, 50, -200);
    m_children[2]->m_transform.identity().rotateZ(rotation).translate(600, 50, 200);
    m_children[3]->m_transform.identity().rotateZ(rotation).translate(600, 50, -200);
}
