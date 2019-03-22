#include "Engine.h"

using namespace grog;

#include <string.h>

#ifdef __linux
#include <iostream>
#define PRINT_TRIANGLE(t) if(!t) std::cout << "NULL\n"; else std::cout << t->z << " - (" << t->p1x/1024. << "," << t->p1y/1024. <<") - ("<< t->p2x/1024. << "," << t->p2y/1024. <<") - ("<< t->p3x/1024. << "," << t->p3y/1024. <<") " << (t->next ? "+next\n" : "+end\n");
#endif

Engine::Engine() noexcept
{

}

Engine::~Engine() noexcept
{
  if(transformedVertexBuffer)
    delete[] transformedVertexBuffer;

  if(triangleStack)
    delete[] triangleStack;
}

void Engine::init(uint32_t maxVerticesPerMesh,
                  uint32_t maxTriangles,
                  uint32_t maxTransforms
#ifdef __linux
                  , QPixmap* pixmap
#endif
                  ) noexcept
{
  if(transformedVertexBuffer)
    delete[] transformedVertexBuffer;

  transformedVertexBuffer = new /*(std::nothrow)*/ int32_t[maxVerticesPerMesh*3];

  if(triangleStack)
    delete[] triangleStack;

  triangleStack = new /*(std::nothrow)*/ Triangle[maxTriangles];
  triangleStackHead = nullptr;
  triangleCount = 0;
  this->maxTriangles = maxTriangles;

#ifdef __linux
  display.pixmap = pixmap;
#endif
}

void Engine::projectScene(const SceneNode* node,
                          const Matrix& parentMvp)noexcept
{
  if(GROG_UNLIKELY(node == nullptr))
    return;

  Matrix mvp;
  Matrix::Transform(parentMvp, node->transform, mvp);

  const Mesh& mesh = node->mesh;

  // project all the coordinates in transformedVertexBuffer
  {
    const int32_t* inVertexBuffer = mesh.vertexBuffer;
    int32_t* outTransformedVertexBuffer = transformedVertexBuffer;
    int32_t inX(0), inY(0), inZ(0);
    int pouet(0);
    for(uint32_t vertexIndex = mesh.vertexCount; vertexIndex; --vertexIndex)
    {
      inX = (*inVertexBuffer++);
      inY = (*inVertexBuffer++);
      inZ = (*inVertexBuffer++);

      int32_t w = (mvp.data[12] * inX
          +  mvp.data[13] * inY
          +  mvp.data[14] * inZ
          +  mvp.data[15] * 1024) >> 10;
      if(GROG_UNLIKELY(w == 0))
        w = 1;

      if(w >= 0)
      {

        (*outTransformedVertexBuffer++) = (((mvp.data[0] * inX
                                          +  mvp.data[1] * inY
                                          +  mvp.data[2] * inZ
                                          +  mvp.data[3] * 1024)/w) >> 10) + 40;
        (*outTransformedVertexBuffer++) = (((mvp.data[4] * inX
                                          +  mvp.data[5] * inY
                                          +  mvp.data[6] * inZ
                                          +  mvp.data[7] * 1024)/w) >> 10) + 32;
        (*outTransformedVertexBuffer++) = (((mvp.data[8] * inX
                                          +  mvp.data[9] * inY
                                          +  mvp.data[10] * inZ
                                          +  mvp.data[11] * 1024)*1000)/w) >> 10;
      }
      else
      {
        (*outTransformedVertexBuffer++) = std::numeric_limits<int32_t>::max();
        outTransformedVertexBuffer++;
        outTransformedVertexBuffer++;

      }
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
      int32_t* vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p1x = (*vertex++);
      if(projection.p1x == std::numeric_limits<int32_t>::max())
      {
        continue;
      }
      projection.p1y = (*vertex++);
      projection.z = (*vertex++);

      vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p2x = (*vertex++);
      if(projection.p2x == std::numeric_limits<int32_t>::max())
      {
        continue;
      }
      projection.p2y = (*vertex++);
      //int32_t z = (*vertex++);
      //projection.z = max2(projection.z, z);
      projection.z += (*vertex++);

      vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p3x = (*vertex++);
      if(projection.p3x == std::numeric_limits<int32_t>::max())
      {
        continue;
      }
      projection.p3y = (*vertex++);
      /*z = (*vertex++);
      projection.z = max2(projection.z, z);*/
      projection.z += (*vertex++);

      // TODO check that triangle covers some part of the screen

      projection.color = (*colorIter++);
//      std::cout << "Pushing ";
//      PRINT_TRIANGLE((&projection));
      pushTriangle(projection);
//      debugTriangleStack();
    }
  }
  // end push triangles

  // recursive call to children
  {
    SceneNode** child = node->children;
    for(uint32_t childIndex = node->childCount;
        childIndex;
        --childIndex, ++child)
    {
      projectScene(*child, mvp);
    }
  }
  // end recursive call
}

void Engine::projectScene(const SceneNode *node) noexcept
{
  Matrix mvp;
  Matrix::Transform(projection, view, mvp);

  projectScene(node, mvp);
}

void Engine::render() noexcept
{
  const Triangle* currentTriangle = triangleStackHead;
  while(currentTriangle)
  {
    rasterizeTriangle(*currentTriangle, display.buffer);
    currentTriangle = currentTriangle->next;
  }

  display.draw();

//  debugTriangleStack();
  newFrame();
//  exit(0);
}

void Engine::setProjection(const Matrix& projection) noexcept
{
  this->projection = projection;
}

void Engine::setView(const TransformMatrix& view) noexcept
{
  this->view = view;
}

void Engine::debugTriangleStack()
{
#ifdef __linux
  std::cout << "Triangle count: " << triangleCount << std::endl;
  std::cout << "Head ";
  PRINT_TRIANGLE(triangleStackHead);
  std::cout.flush();
  Triangle* current = triangleStackHead;
  uint32_t count(0);
  while(current)
  {
    std::cout << "#" <<count << " ";
    PRINT_TRIANGLE(current);
    current = current->next;
    ++count;
  }
#endif
}

#include "Colors.h"
#include <cstdio>
void Engine::pushDebugTriangles()
{
  Triangle t1;
  t1.p1x = 0;
  t1.p1y = 0;
  t1.p2x = 5;
  t1.p2y = 0;
  t1.p3x = 5;
  t1.p3y = 5;
  t1.z = 10;
  t1.color = grog::color(grog::Color::Green);

  std::printf("color %x\n", t1.color);

//  Triangle t2;
//  t2.p1x = 30;
//  t2.p1y = 20;
//  t2.p2x = 70;
//  t2.p2y = 20;
//  t2.p3x = 70;
//  t2.p3y = 60;
//  t2.z = 20;
//  t2.color = grog::color(grog::Color::Blue);

  pushTriangle(t1);
//  pushTriangle(t2);

  debugTriangleStack();
}

void Engine::pushTriangle(Triangle& in) noexcept
{
if(grog::orient2d(in.p1x, in.p1y, in.p2x, in.p2y, in.p3x, in.p3y) <= 0)
{
  return;
}

  if(triangleCount == maxTriangles)
  {
    // replace triangles
  }
  else
  {
    // standard insertion

    Triangle* insertedTriangle = triangleStack + triangleCount;
    *insertedTriangle = in;

    if(GROG_UNLIKELY(triangleCount == 0))
    {
      insertedTriangle->next = nullptr;
      triangleStackHead = insertedTriangle;
    }
    else
    {
      Triangle* current = triangleStackHead;

      if(GROG_UNLIKELY(current->z <= insertedTriangle->z))
      {
        insertedTriangle->next = current;
        triangleStackHead = insertedTriangle;
      }
      else
      {
        while(current->next && current->next->z > insertedTriangle->z)
        {
          current = current->next;
        }

        if(GROG_UNLIKELY(current->next == nullptr))
        {
          insertedTriangle->next = nullptr;
          current->next = insertedTriangle;
        }
        else // current->z > insertedTriangle->z && current->next->z <= insertedTriangle->z
        {
          insertedTriangle->next= current->next;
          current->next = insertedTriangle;
        }
      }

    }

    ++triangleCount;
  }
}

void Engine::newFrame()
{
  triangleCount = 0;
  triangleStackHead = nullptr;
  //katenextTransformIndex = 0;
}
