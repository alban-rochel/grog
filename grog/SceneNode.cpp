#include "SceneNode.h"

using namespace grog;

SceneNode::SceneNode() noexcept:
  mesh(),
  transform(),
  children(nullptr),
  childCount(0)
{

}

SceneNode::~SceneNode() noexcept
{
  if(childCount && children)
  {
    for(uint32_t childIndex = childCount;
        childIndex;
        --childIndex)
    {
      delete children[childIndex-1];
    }
    delete[] children;
  }
}
