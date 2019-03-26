#include "Road.h"
#include "asset_road.h"
#include "asset_tree.h"

using namespace grog;

Road::Road():
  SceneNode()
{
  mesh.vertexBuffer = grog::road_vertices;
  mesh.vertexCount = grog::road_vertexCount;
  mesh.faces = grog::road_faces;
  mesh.faceCount = grog::road_faceCount;
  mesh.colors = grog::road_colors;

  children = new grog::SceneNode*[8];
  childCount = 8;

  for(uint32_t ii = 0; ii < 8; ++ii)
  {
    children[ii] = new grog::SceneNode;
    children[ii]->mesh.vertexBuffer = (ii < 4 ? grog::tree1_vertices : grog::tree2_vertices);
    children[ii]->mesh.vertexCount = grog::tree_vertexCount;
    children[ii]->mesh.faces = grog::tree_faces;
    children[ii]->mesh.faceCount = grog::tree_faceCount;
    children[ii]->mesh.colors = grog::tree_colors;
  }

  children[0]->transform.identity().translate(6000, 0, -6000);
  children[1]->transform.identity().translate(-5000, 0, 5500);
  children[2]->transform.identity().translate(3000, 0, -6000);
  children[3]->transform.identity().translate(-1000, 0, 6000);
  children[4]->transform.identity().translate(-6000, 0, -6000);
  children[5]->transform.identity().translate(5000, 0, 5500);
  children[6]->transform.identity().translate(-3000, 0, -6000);
  children[7]->transform.identity().translate(1000, 0, 6000);
}
