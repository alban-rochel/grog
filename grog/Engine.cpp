#include "Engine.h"

using namespace grog;

#include <new>          // std::nothrow
#include <string.h>
#include <iostream>

Engine::Engine() noexcept
{

}

Engine::~Engine() noexcept
{
  if(transformedVertexBuffer)
    delete[] transformedVertexBuffer;

  if(transformStack)
    delete[] transformStack;

  if(triangleStack)
    delete[] triangleStack;
}

void Engine::init(uint32_t maxVerticesPerMesh,
                  uint32_t maxTriangles,
                  uint32_t maxTransforms) noexcept
{
  if(transformedVertexBuffer)
    delete[] transformedVertexBuffer;

  transformedVertexBuffer = new (std::nothrow) coord[maxVerticesPerMesh*3];

  if(transformStack)
    delete[] transformStack;

  transformStack = new (std::nothrow) TransformMatrix[maxTransforms];
  nextTransformIndex = 0;
  transformCount = maxTransforms;

  if(triangleStack)
    delete[] triangleStack;

  triangleStack = new (std::nothrow) Triangle[maxTriangles];
  triangleStackHead = nullptr;
  triangleCount = 0;
  this->maxTriangles = maxTriangles;
}

void Engine::pushTransform(const TransformMatrix &transform) noexcept
{
  if(GROG_UNLIKELY(nextTransformIndex == transformCount))
  {
    return;
  }

  if(GROG_UNLIKELY(nextTransformIndex == 0))
  {
    memcpy(transformStack, transform, sizeof(transform));
  }
  else
  {
    TMatrixProduct(transformStack[nextTransformIndex-1], transform, transformStack[nextTransformIndex]);
  }

  ++nextTransformIndex;
}

void Engine::popTransform() noexcept
{
  if(GROG_UNLIKELY(nextTransformIndex == 0))
  {
    return;
  }

  --nextTransformIndex;
}

void Engine::projectScene(const SceneNode *node) noexcept
{
  if(GROG_UNLIKELY(node == nullptr))
    return;

  pushTransform(node->transform);

  const TransformMatrix& transform = transformStack[nextTransformIndex-1]; // TODO we should use MVP later on, rather than MV

  const Mesh& mesh = node->mesh;

  // project all the coordinates in transformedVertexBuffer
  {
    const coord* inVertexBuffer = mesh.vertexBuffer;
    coord* outTransformedVertexBuffer = transformedVertexBuffer;
    coord inX(0), inY(0), inZ(0);
    for(uint32_t vertexIndex = mesh.vertexCount; vertexIndex; --vertexIndex)
    {
      inX = (*inVertexBuffer++);
      inY = (*inVertexBuffer++);
      inZ = (*inVertexBuffer++);

      (*outTransformedVertexBuffer++) = transform[0] * inX
                                        +  transform[1] * inY
                                        +  transform[2] * inZ
                                        +  transform[3];
      (*outTransformedVertexBuffer++) = transform[4] * inX
                                        +  transform[5] * inY
                                        +  transform[6] * inZ
                                        +  transform[7];
      (*outTransformedVertexBuffer++) = transform[8] * inX
                                        +  transform[9] * inY
                                        +  transform[10] * inZ
                                        +  transform[11];
    }
  }
  // end project all the coordinates in transformedVertexBuffer

  // push triangles
  {
    Triangle projection;
    const uint32_t* faceIter = mesh.faces;
    const uint8_t* colorIter = mesh.colors;
    for(uint32_t faceIndex = mesh.faceCount; faceIndex; --faceIndex)
    {
      coord* vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p1x = (*vertex++);
      projection.p1y = (*vertex++);
      projection.z = (*vertex++);

      vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p2x = (*vertex++);
      projection.p2y = (*vertex++);
      coord z = (*vertex++);
      projection.z = max2(projection.z, z);

      vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p3x = (*vertex++);
      projection.p3y = (*vertex++);
      z = (*vertex++);
      projection.z = max2(projection.z, z);

      // TODO check that triangle covers some part of the screen

      projection.color = (*colorIter++);
      pushTriangle(projection);
    }
  }
  // end push triangles

  // recursive call to children
  {
    SceneNode* child;
    for(uint32_t childIndex = node->childCount;
        childIndex;
        --childIndex, ++child)
    {
      projectScene(child);
    }
  }
  // end recursive call

  popTransform();
}

void Engine::render(bufferType* frameBuffer) noexcept
{
  const Triangle* currentTriangle = triangleStack;
  for(uint32_t triangleIndex = triangleCount; triangleIndex; --triangleIndex, ++currentTriangle)
  {
    rasterizeTriangle(*currentTriangle, frameBuffer);
  }
std::cout << "new" << std::endl;
  newFrame();
}

void Engine::pushTriangle(Triangle& in) noexcept
{
  std::cout << "push " << in.p1x << "," << in.p1y << "  - "
               << in.p2x << "," << in.p2y << "  - "
                  << in.p3x << "," << in.p3y << "\n";
  if(triangleCount == maxTriangles)
  {
    // replace triangles
  }
  else
  {
    // standard insertion
    triangleStack[triangleCount] = in;
    ++triangleCount;
  }
}

void Engine::newFrame()
{
  triangleCount = 0;
  triangleStackHead = nullptr;
  nextTransformIndex = 0;
}
