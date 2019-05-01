#include "Road.h"
#include "asset_road.h"
#include "asset_tree.h"

using namespace demo;

Road::Road():
  grog::SceneNode()
{
  /** Coucou
   */
  renderPass = 0x01;
  mesh.vertexBuffer = demo::road_vertices;
  mesh.vertexCount = demo::road_vertexCount;
  mesh.faces = demo::road_faces;
  mesh.faceCount = demo::road_faceCount;
  mesh.colors = demo::road_colors;

  children = new grog::SceneNode*[8];
  childCount = 8;

  for(uint32_t ii = 0; ii < 8; ++ii)
  {
    children[ii] = new grog::SceneNode;
    children[ii]->mesh.vertexBuffer = (ii < 4 ? demo::tree1_vertices : demo::tree2_vertices);
    children[ii]->mesh.vertexCount = demo::tree_vertexCount;
    children[ii]->mesh.faces = demo::tree_faces;
    children[ii]->mesh.faceCount = demo::tree_faceCount;
    children[ii]->mesh.colors = demo::tree_colors;
    children[ii]->renderPass = 0x02;
  }
/**
  pouet
  */
  children[0]->transform.identity().translate(6000, 0, -6000);
  children[1]->transform.identity().translate(-5000, 0, 5500);
  children[2]->transform.identity().translate(3000, 0, -6000);
  children[3]->transform.identity().translate(-1000, 0, 6000);
  children[4]->transform.identity().translate(-6000, 0, -6000);
  children[5]->transform.identity().translate(5000, 0, 5500);
  children[6]->transform.identity().translate(-3000, 0, -6000);
  children[7]->transform.identity().translate(1000, 0, 6000);
}
