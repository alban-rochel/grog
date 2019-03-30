#include "Engine.h"
#ifdef __linux
#include "Gamebuino-Meta-stuff.h"
#else
#include <Gamebuino-Meta.h>
#endif

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
                  uint32_t maxTriangles
#ifdef __linux
                  , QPixmap* pixmap
#endif
                  ) noexcept
{
  if(transformedVertexBuffer)
    delete[] transformedVertexBuffer;

  transformedVertexBuffer = new /*(std::nothrow)*/ int32_t[maxVerticesPerMesh*4];

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
                          const Matrix& parentMvp,
                          uint32_t pass) noexcept
{
  if(GROG_UNLIKELY(node == nullptr))
    return;

  Matrix mvp;
  Matrix::Transform(parentMvp, node->transform, mvp);

  if(pass & node->renderPass)
  {
    const Mesh& mesh = node->mesh;

    // project all the coordinates in transformedVertexBuffer
    {
      const int32_t* inVertexBuffer = mesh.vertexBuffer;
      int32_t* outTransformedVertexBuffer = transformedVertexBuffer;
      int32_t inX(0), inY(0), inZ(0);
      //    int pouet(0);
      for(uint32_t vertexIndex = mesh.vertexCount; vertexIndex; --vertexIndex)
      {
        inX = (*inVertexBuffer++);
        inY = (*inVertexBuffer++);
        inZ = (*inVertexBuffer++);

//        int32_t w = (mvp.data[12] * inX
//            +  mvp.data[13] * inY
//            +  mvp.data[14] * inZ
//            +  mvp.data[15] * 1024) >> 10;
//        if(GROG_UNLIKELY(w == 0))
//          w = 1;
//        if(w<0)
//          w = -w;
        //      if(w >= 0)
        {
#ifdef BEFORE
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
              +  mvp.data[11] * 1024))/w);
#else
          // X_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.data[0] * inX
                                            +  mvp.data[1] * inY
                                            +  mvp.data[2] * inZ
                                            +  mvp.data[3] * 1024;

          // Y_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.data[4] * inX
                                            +  mvp.data[5] * inY
                                            +  mvp.data[6] * inZ
                                            +  mvp.data[7] * 1024;

          // Z_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.data[8] * inX
                                            +  mvp.data[9] * inY
                                            +  mvp.data[10] * inZ
                                            +  mvp.data[11] * 1024;

          // W_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.data[12] * inX
                                            +  mvp.data[13] * inY
                                            +  mvp.data[14] * inZ
                                            +  mvp.data[15] * 1024;
#endif
        }
      }
    }
    // end project all the coordinates in transformedVertexBuffer

    // push triangles
    {
//      bool isInNDC(true);
      Triangle projection;
      const uint32_t* faceIter = mesh.faces;
      const Gamebuino_Meta::ColorIndex* colorIter = mesh.colors;
      for(uint32_t faceIndex = mesh.faceCount; faceIndex; --faceIndex)
      {
        uint32_t ndcFlag(0);
        int32_t* vertex = transformedVertexBuffer + 4 * (*faceIter++);
        projection.p1x = (*vertex++);
        projection.p1y = (*vertex++);
        int32_t z = (*vertex++);
        projection.z = z;
        int32_t w = (*vertex++) >> 10;
        if(GROG_UNLIKELY(w == 0))
          w = 1;
        projection.p1x = ((projection.p1x/w) >> 10) + 40;
        projection.p1y = ((projection.p1y/w) >> 10) + 32;
        if(z > -1024)
          ndcFlag = 0b001;

        vertex = transformedVertexBuffer + 4 * (*faceIter++);
        projection.p2x = (*vertex++);
        projection.p2y = (*vertex++);
        z = (*vertex++);
        projection.z += z;
        w = (*vertex++) >> 10;
        if(GROG_UNLIKELY(w == 0))
          w = 1;
        projection.p2x = ((projection.p2x/w) >> 10) + 40;
        projection.p2y = ((projection.p2y/w) >> 10) + 32;
        if(z > -1024)
          ndcFlag |= 0b010;

        vertex = transformedVertexBuffer + 4 * (*faceIter++);
        projection.p3x = (*vertex++);
        projection.p3y = (*vertex++);
        z = (*vertex++);
        projection.z += z;
        w = (*vertex++) >> 10;
        if(GROG_UNLIKELY(w == 0))
          w = 1;
        projection.p3x = ((projection.p3x/w) >> 10) + 40;
        projection.p3y = ((projection.p3y/w) >> 10) + 32;
        if(z > -1024)
          ndcFlag |= 0b100;

        projection.color = (*colorIter++);
        //      std::cout << "Pushing ";
        //      PRINT_TRIANGLE((&projection));
        if(ndcFlag == 0b111)
          pushTriangle(projection);
        //      debugTriangleStack();
      }
    }
    // end push triangles
  } // end if(pass & node->renderPass)

  // recursive call to children
  {
    SceneNode** child = node->children;
    for(uint32_t childIndex = node->childCount;
        childIndex;
        --childIndex, ++child)
    {
      projectScene(*child, mvp, pass);
    }
  }
  // end recursive call
}

void Engine::projectScene(const SceneNode *node, uint32_t pass) noexcept
{
  Matrix mvp;
  Matrix::Transform(projection, view, mvp);

  projectScene(node, mvp, pass);

  render();
}

void Engine::render() noexcept
{
  const Triangle* currentTriangle = triangleStackHead;
  while(currentTriangle)
  {
#ifdef __linux
    rasterizeTriangle(*currentTriangle, display.buffer);
#else
    rasterizeTriangle(*currentTriangle, (uint8_t*)gb.display._buffer);
#endif
    currentTriangle = currentTriangle->next;
  }



//  debugTriangleStack();
  passDone();
}

/*void Engine::setProjection(const Matrix& projection) noexcept
{
  this->projection = projection;
}*/

void Engine::setProjection(float fov, float near, float far) noexcept
{
  this->projection = Matrix::Projection(fov, near, far);
  this->near = grog::floatToFixed(near);
}

void Engine::setView(const TransformMatrix& view) noexcept
{
  this->view = view;
}

#ifdef __linux
void Engine::debugTriangleStack()
{
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
}

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
  t1.color = Gamebuino_Meta::ColorIndex::green;

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
#endif

void Engine::pushTriangle(Triangle& in) noexcept
{

//  std::cout << "Pushing\n";

if(grog::orient2d(in.p1x, in.p1y, in.p2x, in.p2y, in.p3x, in.p3y) <= 0)
{
//  std::cout << "Discarding\n";
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

void Engine::passDone() noexcept
{
  triangleCount = 0;
  triangleStackHead = nullptr;
}
