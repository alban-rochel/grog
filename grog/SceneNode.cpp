#include "SceneNode.h"

using namespace grog;


SceneNode::~SceneNode() noexcept
{
  if(m_childCount && m_children)
  {
    for(uint32_t childIndex = m_childCount;
        childIndex;
        --childIndex)
    {
      delete m_children[childIndex-1];
    }
    delete[] m_children;
  }
}
