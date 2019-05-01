#include "Road.h"
#include "asset_road.h"
#include "asset_tree.h"

using namespace demo;

Road::Road():
  grog::SceneNode()
{
  // The road segments are rendered in first pass.
  // Objects rendered in subsequent passes will be rasterized over the road, no matter the actual visibility.
  // But this saves memory.
  m_renderPass = 0x01;

  // The main node is the road itself.
  m_mesh.m_vertexBuffer = demo::road_vertices;
  m_mesh.m_vertexCount  = demo::road_vertexCount;
  m_mesh.m_faces        = demo::road_faces;
  m_mesh.m_faceCount    = demo::road_faceCount;
  m_mesh.m_colors       = demo::road_colors;

  // The children are tree on the side
  m_children = new grog::SceneNode*[8];
  m_childCount = 8;

  for(uint32_t ii = 0; ii < 8; ++ii)
  {
    m_children[ii] = new grog::SceneNode;
    m_children[ii]->m_mesh.m_vertexBuffer = (ii < 4 ? demo::tree1_vertices : demo::tree2_vertices);
    m_children[ii]->m_mesh.m_vertexCount  = demo::tree_vertexCount;
    m_children[ii]->m_mesh.m_faces        = demo::tree_faces;
    m_children[ii]->m_mesh.m_faceCount    = demo::tree_faceCount;
    m_children[ii]->m_mesh.m_colors       = demo::tree_colors;
    m_children[ii]->m_renderPass          = 0x02; // Trees are rendered in pass 2
  }

  // Positioning trees.
  m_children[0]->m_transform.identity().translate(6000, 0, -6000);
  m_children[1]->m_transform.identity().translate(-5000, 0, 5500);
  m_children[2]->m_transform.identity().translate(3000, 0, -6000);
  m_children[3]->m_transform.identity().translate(-1000, 0, 6000);
  m_children[4]->m_transform.identity().translate(-6000, 0, -6000);
  m_children[5]->m_transform.identity().translate(5000, 0, 5500);
  m_children[6]->m_transform.identity().translate(-3000, 0, -6000);
  m_children[7]->m_transform.identity().translate(1000, 0, 6000);
}
