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
  if(children)
    delete[] children;
}
