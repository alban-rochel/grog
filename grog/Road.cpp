#include "Road.h"
#include "Colors.h"
#include "asset_road.h"

using namespace grog;

Road::Road():
  SceneNode()
{
  mesh.vertexBuffer = grog::road_vertices;
  mesh.vertexCount = grog::road_vertexCount;
  mesh.faces = grog::road_faces;
  mesh.faceCount = grog::road_faceCount;
  mesh.colors = grog::road_colors;

  //children = new grog::SceneNode*[4];
  childCount = 0;
}
