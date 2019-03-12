#include "Engine.h"

using namespace grog;

//#include <new>          // std::nothrow
#include <string.h>
//#include <iostream>


#ifdef __linux
#include <iostream>
#define PRINT_TRIANGLE(t) std::cout << t->z << " - (" << t->p1x << "," << t->p1y <<") - ("<< t->p2x << "," << t->p2y <<") - ("<< t->p3x << "," << t->p3y <<") " << (t->next ? "+next\n" : "+end\n");
#endif

Engine::Engine() noexcept
{

}

Engine::~Engine() noexcept
{
  if(transformedVertexBuffer)
    delete[] transformedVertexBuffer;

/*  if(transformStack)
    delete[] transformStack;*/

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

/*  if(transformStack)
    delete[] transformStack;

  transformStack = new (std::nothrow) TransformMatrix[maxTransforms];
  nextTransformIndex = 0;
  transformCount = maxTransforms;*/

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

void Engine::pushTransform(const TransformMatrix &transform) noexcept
{
/*  if(GROG_UNLIKELY(nextTransformIndex == transformCount))
  {
    return;
  }

  if(GROG_UNLIKELY(nextTransformIndex == 0))
  {
    transformStack[0] = transform;
  }
  else
  {
    TransformMatrix::Product(transformStack[nextTransformIndex-1], transform, transformStack[nextTransformIndex]);
  }

  ++nextTransformIndex;*/
}

void Engine::popTransform() noexcept
{
/*  if(GROG_UNLIKELY(nextTransformIndex == 0))
  {
    return;
  }

  --nextTransformIndex;*/
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
    for(uint32_t vertexIndex = mesh.vertexCount; vertexIndex; --vertexIndex)
    {
      inX = (*inVertexBuffer++);
      inY = (*inVertexBuffer++);
      inZ = (*inVertexBuffer++);

//      std::cout << "IN FIXED "
//                << inX/1024.f << " - "
//                << inY/1024.f << " - "
//                << inZ/1024.f << "\n";

      (*outTransformedVertexBuffer++) = (((mvp.data[0] * inX
                                        +  mvp.data[1] * inY
                                        +  mvp.data[2] * inZ
                                        +  mvp.data[3] * 1024) * 40) >> 20) + 40;
      (*outTransformedVertexBuffer++) = (((mvp.data[4] * inX
                                        +  mvp.data[5] * inY
                                        +  mvp.data[6] * inZ
                                        +  mvp.data[7] * 1024) * 32) >> 20) + 32;
      (*outTransformedVertexBuffer++) = ((mvp.data[8] * inX
                                        +  mvp.data[9] * inY
                                        +  mvp.data[10] * inZ
                                        +  mvp.data[11] * 1024)*100) >> 20;

//      int32_t outX = (((mvp.data[0] * inX
//          +  mvp.data[1] * inY
//          +  mvp.data[2] * inZ
//          +  mvp.data[3] * 1024) * 40) >> 20) + 40;

//      float outXbis = ((mvp.data[0] * inX
//          +  mvp.data[1] * inY
//          +  mvp.data[2] * inZ
////          +  mvp.data[3] * 1024) * 40)/1024./1024. + 40;

//      std::cout << mvp.data[0]/1024. << "*" << inX/1024. << std::endl;
//      std::cout << mvp.data[1]/1024. << "*" << inY/1024. << std::endl;
//      std::cout << mvp.data[2]/1024. << "*" << inZ/1024. << std::endl;
//      std::cout << mvp.data[3]/1024.<< std::endl;

//      int32_t outY = ((mvp.data[4] * inX
//                    +  mvp.data[5] * inY
//                    +  mvp.data[6] * inZ
//                    +  mvp.data[7] * 1024) * 32) >> 20 + 32;

//      float outYbis = ((mvp.data[4] * inX
//                    +  mvp.data[5] * inY
//                    +  mvp.data[6] * inZ
//                    +  mvp.data[7] * 1024) * 40)/1024./1024. + 32;

//      int32_t outZ = (mvp.data[8] * inX
//                    +  mvp.data[9] * inY
//                    +  mvp.data[10] * inZ
//                    +  mvp.data[11] * 1024) >> 20;

//      std::cout << "OUT FIXED "
//                << outX << " / " << outXbis << " - "
//                << outY << " / " << outYbis << " - "
//                << outZ << "\n";
//      std::cout.flush();
//      exit(0);
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
      projection.p1y = (*vertex++);
      projection.z = (*vertex++);

      vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p2x = (*vertex++);
      projection.p2y = (*vertex++);
      int32_t z = (*vertex++);
      projection.z = max2(projection.z, z);

      vertex = transformedVertexBuffer + 3 * (*faceIter++);
      projection.p3x = (*vertex++);
      projection.p3y = (*vertex++);
      z = (*vertex++);
      projection.z = max2(projection.z, z);

      // TODO check that triangle covers some part of the screen

      projection.color = (*colorIter++);
      /*std::cout << "Pushing ";
      PRINT_TRIANGLE((&projection));*/
      pushTriangle(projection);
      /*debugTriangleStack();*/
    }
  }
  // end push triangles

  // recursive call to children
  {
    SceneNode* child = node->children;
    for(uint32_t childIndex = node->childCount;
        childIndex;
        --childIndex, ++child)
    {
      projectScene(child, mvp);
    }
  }
  // end recursive call

  popTransform();
}

void Engine::projectScene(const SceneNode *node) noexcept
{
  Matrix mvp;
  Matrix::Transform(projection, view, mvp);

  projectScene(node, mvp);
}

void Engine::render() noexcept
{
  /*const Triangle* currentTriangle = triangleStack;
  for(uint32_t triangleIndex = triangleCount; triangleIndex; --triangleIndex, ++currentTriangle)
  {
    rasterizeTriangle(*currentTriangle, display.buffer);
  }*/

  const Triangle* currentTriangle = triangleStackHead;
  while(currentTriangle)
  {
    rasterizeTriangle(*currentTriangle, display.buffer);
    currentTriangle = currentTriangle->next;
  }

  display.draw();

//std::cout << "new" << std::endl;
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
//  std::cout << "push " << in.p1x << "," << in.p1y << "  - "
//               << in.p2x << "," << in.p2y << "  - "
//                  << in.p3x << "," << in.p3y << "\n";

if(grog::orient2d(in.p1x, in.p1y, in.p2x, in.p2y, in.p3x, in.p3y) <= 0)
  return;

  if(triangleCount == maxTriangles)
  {
    // replace triangles
  }
  else
  {
    // standard insertion
    /*triangleStack[triangleCount] = in;
    ++triangleCount;*/

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
