#include "SceneNode.h"

using namespace grog;

SceneNode::SceneNode() noexcept:
  mesh(),
  transform {1.f, 0.f, 0.f, 0.f,
             0.f, 1.f, 0.f, 0.f,
             0.f, 0.f, 1.f, 0.f,
             0.f},
  children(nullptr),
  childCount(0)
{

}

SceneNode::~SceneNode() noexcept
{
  if(children)
    delete[] children;
}
