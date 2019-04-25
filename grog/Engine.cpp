#include "Engine.h"
#ifdef __linux
#include "Gamebuino-Meta-stuff.h"
#else
#include <Gamebuino-Meta.h>
#endif

using namespace grog;

#define INVALID_I32 (0x7FFFFFFF)

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

GROG_INLINE void pushClippedTriangle1(grog::Engine* engine,
                                      int32_t* v1 /* inside */,
                                      int32_t* v2 /* outside */,
                                      int32_t* v3 /* outside */,
                                      const Gamebuino_Meta::ColorIndex& color)
{
//  std::cout << "*** Clipped 1\n";
  Triangle projection;

  int32_t w = v1[3] >> 10;
  if(GROG_UNLIKELY(w==0))
    w = 1;
  projection.p1x = ((v1[0]/w) >> 10) + 40;
  projection.p1y = ((v1[1]/w) >> 10) + 32;
  projection.z = 3 * v1[2];

  int32_t gamma_num;
  int32_t gamma_den;
//  int32_t z;

  {
    // Process v1 (in) vs v2 (out)
#define Z1 v1[2]
#define W1 v1[3]
#define Z2 v2[2]
#define W2 v2[3]
     gamma_num = - W2 - Z2;
     gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
     w = -(gamma_num * ((v1[2] - v2[2]) / gamma_den) + v2[2]) >> 10;
     if(GROG_UNLIKELY(w==0))
       w = 1;

     projection.p2x = gamma_num * ((v1[0] - v2[0]) / gamma_den) + v2[0];
     projection.p2x = ((projection.p2x/w) >> 10) + 40;
     projection.p2y = gamma_num * ((v1[1] - v2[1]) / gamma_den) + v2[1];
     projection.p2y = ((projection.p2y/w) >> 10) + 32;
  }

  {
    // Process v1 (in) vs v3 (out)
#define Z1 v1[2]
#define W1 v1[3]
#define Z2 v3[2]
#define W2 v3[3]
     gamma_num = - W2 - Z2;
     gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
     w = -(gamma_num * ((v1[2] - v3[2]) / gamma_den) + v3[2]) >> 10;
     if(GROG_UNLIKELY(w==0))
       w = 1;

     projection.p3x = gamma_num * ((v1[0] - v3[0]) / gamma_den) + v3[0];
     projection.p3x = ((projection.p3x/w) >> 10) + 40;
     projection.p3y = gamma_num * ((v1[1] - v3[1]) / gamma_den) + v3[1];
     projection.p3y = ((projection.p3y/w) >> 10) + 32;
  }

  projection.color = color;

  engine->pushTriangle(projection);
}

GROG_INLINE void pushClippedTriangle2(grog::Engine* engine,
                                      int32_t* v1 /* inside */,
                                      int32_t* v2 /* inside */,
                                      int32_t* v3 /* outside */,
                                      const Gamebuino_Meta::ColorIndex& color)
{
//  std::cout << "*** Clipped 2\n";
  Triangle projection1 /*(v1, v2, vn)*/, projection2 /*(v1, vn, vm)*/;

  int32_t w = v1[3] >> 10;
  if(GROG_UNLIKELY(w==0))
    w = 1;
  projection1.p1x = projection2.p1x = ((v1[0]/w) >> 10) + 40;
  projection1.p1y = projection2.p1y = ((v1[1]/w) >> 10) + 32;
  projection1.z = projection2.z = v1[2] / w;

  w = v2[3] >> 10;
  if(GROG_UNLIKELY(w==0))
    w = 1;
  projection1.p2x = ((v2[0]/w) >> 10) + 40;
  projection1.p2y = ((v2[1]/w) >> 10) + 32;
  projection1.z = 3 * v2[2];

  int32_t gamma_num;
  int32_t gamma_den;
//  int32_t z;

  {
    // Process v2 (in) vs v3 (out)
#define Z1 v2[2]
#define W1 v2[3]
#define Z2 v3[2]
#define W2 v3[3]
     gamma_num = - W2 - Z2;
     gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
     w = -(gamma_num * ((v2[2] - v3[2]) / gamma_den) + v3[2]) >> 10;
     if(GROG_UNLIKELY(w==0))
       w = 1;

     projection1.p3x = gamma_num * ((v2[0] - v3[0]) / gamma_den) + v3[0];
     projection1.p3x = projection2.p2x = ((projection1.p3x/w) >> 10) + 40;
     projection1.p3y = gamma_num * ((v2[1] - v3[1]) / gamma_den) + v3[1];
     projection1.p3y = projection2.p2y = ((projection1.p3y/w) >> 10) + 32;
  }


  {
    // Process v1 (in) vs v3 (out)
#define Z1 v1[2]
#define W1 v1[3]
#define Z2 v3[2]
#define W2 v3[3]
     gamma_num = - W2 - Z2;
     gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
     w = -(gamma_num * ((v1[2] - v3[2]) / gamma_den) + v3[2]) >> 10;
     if(GROG_UNLIKELY(w==0))
       w = 1;

     projection2.p3x = gamma_num * ((v1[0] - v3[0]) / gamma_den) + v3[0];
     projection2.p3x = ((projection2.p3x/w) >> 10) + 40;
     projection2.p3y = gamma_num * ((v1[1] - v3[1]) / gamma_den) + v3[1];
     projection2.p3y = ((projection2.p3y/w) >> 10) + 32;
  }

  projection1.color = projection2.color = color;

  engine->pushTriangle(projection1);
  engine->pushTriangle(projection2);
}


GROG_INLINE void pushUnclippedTriangle(grog::Engine* engine,
                                       int32_t* v1,
                                       int32_t* v2,
                                       int32_t* v3,
                                       const Gamebuino_Meta::ColorIndex& color)
{
//  std::cout << "*** unclipped\n";
  Triangle projection;

  projection.p1x = (*v1++);
  projection.p1y = (*v1++);
  projection.z = (*v1++);
  int32_t w = (*v1++) >> 10;
  if(GROG_UNLIKELY(w == 0))
    w = 1;
  projection.p1x = ((projection.p1x/w) >> 10) + 40;
  projection.p1y = ((projection.p1y/w) >> 10) + 32;

  projection.p2x = (*v2++);
  projection.p2y = (*v2++);
  projection.z += (*v2++);
  w = (*v2++) >> 10;
  if(GROG_UNLIKELY(w == 0))
    w = 1;
  projection.p2x = ((projection.p2x/w) >> 10) + 40;
  projection.p2y = ((projection.p2y/w) >> 10) + 32;

  projection.p3x = (*v3++);
  projection.p3y = (*v3++);
  projection.z += (*v3++);
  w = (*v3++) >> 10;
  if(GROG_UNLIKELY(w == 0))
    w = 1;
  projection.p3x = ((projection.p3x/w) >> 10) + 40;
  projection.p3y = ((projection.p3y/w) >> 10) + 32;

  projection.color = color;

  engine->pushTriangle(projection);
}

void Engine::projectScene(const SceneNode *node, const Matrix &parentMvp, uint32_t pass) noexcept
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
      int32_t outZ(0);
      //    int pouet(0);
      for(uint32_t vertexIndex = mesh.vertexCount; vertexIndex; --vertexIndex)
      {
        inX = (*inVertexBuffer++);
        inY = (*inVertexBuffer++);
        inZ = (*inVertexBuffer++);



        {
          outZ =  mvp.data[8] * inX +
                  mvp.data[9] * inY +
                  mvp.data[10] * inZ +
                  mvp.data[11] * 1024;

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
            (*outTransformedVertexBuffer++) = outZ;

            // W_ndc * 1024
            (*outTransformedVertexBuffer++) =    mvp.data[12] * inX
                                              +  mvp.data[13] * inY
                                              +  mvp.data[14] * inZ
                                              +  mvp.data[15] * 1024;
        }
      }
    }
    // end project all the coordinates in transformedVertexBuffer

    // push triangles
    {
      const uint32_t* faceIter = mesh.faces;
      const Gamebuino_Meta::ColorIndex* colorIter = mesh.colors;
      int32_t* v1, *v2, *v3;
      for(uint32_t faceIndex = mesh.faceCount; faceIndex; --faceIndex)
      {
        uint32_t ndcFlag(0);
        v1 = transformedVertexBuffer + 4 * (*faceIter++);
        v2 = transformedVertexBuffer + 4 * (*faceIter++);
        v3 = transformedVertexBuffer + 4 * (*faceIter++);

        if(v1[2] > -v1[3])
          ndcFlag = 0b001;
        if(v2[2] > -v2[3])
          ndcFlag |= 0b010;
        if(v3[2] > -v3[3])
          ndcFlag |= 0b100;

//        std::cout << "*** triangle\n";
//        std::cout << "v1\t" << v1[0] << " - " << v1[0]/((float)v1[3]) << std::endl;
//        std::cout << "  \t" << v1[1] << " - " << v1[1]/((float)v1[3]) << std::endl;
//        std::cout << "  \t" << v1[2] << " - " << v1[2]/((float)v1[3]) << std::endl;
//        std::cout << "  \t" << v1[3] << " - " << v1[3]/1024. << std::endl;
//        std::cout << std::endl;
//        std::cout << "v2\t" << v2[0] << " - " << v2[0]/((float)v2[3]) << std::endl;
//        std::cout << "  \t" << v2[1] << " - " << v2[1]/((float)v2[3]) << std::endl;
//        std::cout << "  \t" << v2[2] << " - " << v2[2]/((float)v2[3]) << std::endl;
//        std::cout << "  \t" << v2[3] << " - " << v2[3]/1024. << std::endl;
//        std::cout << std::endl;
//        std::cout << "v3\t" << v3[0] << " - " << v3[0]/((float)v3[3]) << std::endl;
//        std::cout << "  \t" << v3[1] << " - " << v3[1]/((float)v3[3]) << std::endl;
//        std::cout << "  \t" << v3[2] << " - " << v3[2]/((float)v3[3]) << std::endl;
//        std::cout << "  \t" << v3[3] << " - " << v3[3]/1024. << std::endl;
//        std::cout << std::endl;

        switch(ndcFlag)
        {
          case 0b000:
            ++colorIter;
            break;

          case 0b001:
            pushClippedTriangle1(this, v1, v2, v3, (*colorIter++));
            break;

          case 0b010:
            pushClippedTriangle1(this, v2, v3, v1, (*colorIter++));
            break;

          case 0b011:
            pushClippedTriangle2(this, v1, v2, v3, (*colorIter++));
            break;

          case 0b100:
            pushClippedTriangle1(this, v3, v1, v2, (*colorIter++));
            break;

          case 0b101:
            pushClippedTriangle2(this, v3, v1, v2, (*colorIter++));
            break;

          case 0b110:
            pushClippedTriangle2(this, v2, v3, v1, (*colorIter++));
            break;

          case 0b111:
            pushUnclippedTriangle(this, v1, v2, v3, (*colorIter++));
            break;

          default:
            ++colorIter;
            break;
        }
      }
    }
  }



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

void Engine::projectSceneOld(const SceneNode* node,
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

        {

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

        }
      }
    }
    // end project all the coordinates in transformedVertexBuffer

    // push triangles
    {

      const uint32_t* faceIter = mesh.faces;
      const Gamebuino_Meta::ColorIndex* colorIter = mesh.colors;
      int32_t* v1, *v2, *v3;
      for(uint32_t faceIndex = mesh.faceCount; faceIndex; --faceIndex)
      {
        uint32_t ndcFlag(0);
        v1 = transformedVertexBuffer + 4 * (*faceIter++);
        v2 = transformedVertexBuffer + 4 * (*faceIter++);
        v3 = transformedVertexBuffer + 4 * (*faceIter++);

        if(v1[2] > -v1[3])
          ndcFlag = 0b001;
        if(v2[2] > -v2[3])
          ndcFlag |= 0b010;
        if(v3[2] > -v3[3])
          ndcFlag |= 0b100;

        switch(ndcFlag)
        {
          case 0b000:
            ++colorIter;
            break;

          case 0b001:
            pushClippedTriangle1(this, v1, v2, v3, (*colorIter++));
            break;

          case 0b010:
            pushClippedTriangle1(this, v2, v3, v1, (*colorIter++));
            break;

          case 0b011:
            pushClippedTriangle2(this, v1, v2, v3, (*colorIter++));
            break;

          case 0b100:
            pushClippedTriangle1(this, v3, v1, v2, (*colorIter++));
            break;

          case 0b101:
            pushClippedTriangle2(this, v3, v1, v2, (*colorIter++));
            break;

          case 0b110:
            pushClippedTriangle2(this, v2, v3, v1, (*colorIter++));
            break;

          case 0b111:
            pushUnclippedTriangle(this, v1, v2, v3, (*colorIter++));
            break;

          default:
            break;
        }

//        int32_t* vertex = transformedVertexBuffer + 4 * (*faceIter++);
//        projection.p1x = (*vertex++);
//        projection.p1y = (*vertex++);
//        int32_t z = (*vertex++);
//        projection.z = z;
//        int32_t w = (*vertex++) >> 10;
//        if(GROG_UNLIKELY(w == 0))
//          w = 1;
//        projection.p1x = ((projection.p1x/w) >> 10) + 40;
//        projection.p1y = ((projection.p1y/w) >> 10) + 32;
//        if(z > -1024)
//          ndcFlag = 0b001;

//        vertex = transformedVertexBuffer + 4 * (*faceIter++);
//        projection.p2x = (*vertex++);
//        projection.p2y = (*vertex++);
//        z = (*vertex++);
//        projection.z += z;
//        w = (*vertex++) >> 10;
//        if(GROG_UNLIKELY(w == 0))
//          w = 1;
//        projection.p2x = ((projection.p2x/w) >> 10) + 40;
//        projection.p2y = ((projection.p2y/w) >> 10) + 32;
//        if(z > -1024)
//          ndcFlag |= 0b010;

//        vertex = transformedVertexBuffer + 4 * (*faceIter++);
//        projection.p3x = (*vertex++);
//        projection.p3y = (*vertex++);
//        z = (*vertex++);
//        projection.z += z;
//        w = (*vertex++) >> 10;
//        if(GROG_UNLIKELY(w == 0))
//          w = 1;
//        projection.p3x = ((projection.p3x/w) >> 10) + 40;
//        projection.p3y = ((projection.p3y/w) >> 10) + 32;
//        if(z > -1024)
//          ndcFlag |= 0b100;

//        projection.color = (*colorIter++);
//        //      std::cout << "Pushing ";
//        //      PRINT_TRIANGLE((&projection));
//        if(ndcFlag == 0b111)
//          pushTriangle(projection);
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
  while(GROG_LIKELY(currentTriangle != nullptr))
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

  if(triangleCount)
    exit(0);
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

GROG_INLINE bool displayable(const Triangle& in) noexcept
{
  return !((in.p1x < 0 && in.p2x < 0 && in.p3x < 0) ||
           (in.p1y < 0 && in.p2y < 0 && in.p3y < 0) ||
           (in.p1x > 79 && in.p2x > 79 && in.p3x > 79) ||
           (in.p1y > 63 && in.p2y > 63 && in.p3y > 63));
}

void Engine::pushTriangle(Triangle& in) noexcept
{

//  std::cout << "Pushing\n";

//  std::cout << "("  << in.p1x/1024. << ", " << in.p1y/1024. << ") ("
//                    << in.p2x/1024. << ", " << in.p2y/1024. << ") ("
//                    << in.p3x/1024. << ", " << in.p3y/1024. << ")\n";

if(grog::orient2d(in.p1x, in.p1y, in.p2x, in.p2y, in.p3x, in.p3y) <= 0)
{
  return;
}

if(!displayable(in))
{
  return;
}


  if(triangleCount == maxTriangles)
  {
    // replace triangles

    // The new head is the second = triangleStackHead;
    Triangle* insertedTriangle = triangleStackHead;
    triangleStackHead = triangleStackHead->next;
    *insertedTriangle = in;

    {
      Triangle* current = triangleStackHead;
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
