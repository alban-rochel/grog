#include "Engine.h"
#ifdef __linux
#include "Gamebuino-Meta-stuff.h"
#else
#include <Gamebuino-Meta.h>
#endif
#include "Math.h"

using namespace grog;

#ifdef __linux
#include <iostream>
#define PRINT_TRIANGLE(t) if(!t) std::cout << "NULL\n"; else std::cout << t->z << " - (" << t->p1x/1024. << "," << t->p1y/1024. <<") - ("<< t->p2x/1024. << "," << t->p2y/1024. <<") - ("<< t->p3x/1024. << "," << t->p3y/1024. <<") " << (t->next ? "+next\n" : "+end\n");
#endif

Engine::~Engine() noexcept
{
  if(m_transformedVertexBuffer)
    delete[] m_transformedVertexBuffer;

  if(m_triangleStack)
    delete[] m_triangleStack;
}

void Engine::init(uint32_t in_maxVerticesPerMesh,
                  uint32_t in_maxTriangles
                  #ifdef __linux
                  , QPixmap* pixmap
                  #endif
                  ) noexcept
{
  if(m_transformedVertexBuffer)
    delete[] m_transformedVertexBuffer;

  m_transformedVertexBuffer = new int32_t[in_maxVerticesPerMesh*4];

  if(m_triangleStack)
    delete[] m_triangleStack;

  m_triangleStack = new Triangle[in_maxTriangles];
  m_triangleStackHead = nullptr;
  m_triangleCount = 0;
  m_maxTriangles = in_maxTriangles;

#ifdef __linux
  display.pixmap = pixmap;
#endif
}

/**
 * @brief Computes and pushes the triangle in the clipping area (farther than the near plane), when only 1 vertex of the input triangle is within the clipping area.
 * @param in_engine The engine.
 * @param in_v1 The MVP-transformed vertex in homogenous coordinates that is within the clipping area (farther than the near plane).
 * @param in_v2 An MVP-transformed vertex in homogenous coordinates that is outside the clipping area (closer than the near plane, or behind camera).
 * @param in_v3 An MVP-transformed vertex in homogenous coordinates that is outside the clipping area (closer than the near plane, or behind camera).
 * @param in_color The triangle color.
 */
GROG_INLINE void pushClippedTriangle1(grog::Engine* in_engine,
                                      int32_t* in_v1 /* inside */,
                                      int32_t* in_v2 /* outside */,
                                      int32_t* in_v3 /* outside */,
                                      const Gamebuino_Meta::ColorIndex& in_color)
{
  Triangle projection; // This is the triangle within the clipping area that we are going to compute.

  // The first vertex is unchanged.
  // Apply perspective, scale in the [-40,40] and [-32,32] domain, to fit the screen
  int32_t w = in_v1[3] >> 10;
  if(GROG_UNLIKELY(w==0))
    w = 1;
  projection.m_p1x = ((in_v1[0]/w) >> 10) + 40;
  projection.m_p1y = ((in_v1[1]/w) >> 10) + 32;
  projection.m_z = 3 * in_v1[2];

  // Now we are looking for gamma = gamma_num/gamma_den, such that gamma * v1 + (1-gamma) * v2 lies on the near plane, ie w=-z.
  int32_t gamma_num;
  int32_t gamma_den;

  {
    // Process v1 (in) vs v2 (out)
#define Z1 in_v1[2]
#define W1 in_v1[3]
#define Z2 in_v2[2]
#define W2 in_v2[3]
    gamma_num = - W2 - Z2;
    gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
    w = -(gamma_num * ((in_v1[2] - in_v2[2]) / gamma_den) + in_v2[2]) >> 10;
    if(GROG_UNLIKELY(w==0))
      w = 1;

    // We have our second vertex
    // Apply perspective, scale in the [-40,40] and [-32,32] domain, to fit the screen
    projection.m_p2x = gamma_num * ((in_v1[0] - in_v2[0]) / gamma_den) + in_v2[0];
    projection.m_p2x = ((projection.m_p2x/w) >> 10) + 40;
    projection.m_p2y = gamma_num * ((in_v1[1] - in_v2[1]) / gamma_den) + in_v2[1];
    projection.m_p2y = ((projection.m_p2y/w) >> 10) + 32;
  }

  // Same thing for the third vertex. Fun, ain't it?
  {
    // Process v1 (in) vs v3 (out)
#define Z1 in_v1[2]
#define W1 in_v1[3]
#define Z2 in_v3[2]
#define W2 in_v3[3]
    gamma_num = - W2 - Z2;
    gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
    w = -(gamma_num * ((in_v1[2] - in_v3[2]) / gamma_den) + in_v3[2]) >> 10;
    if(GROG_UNLIKELY(w==0))
      w = 1;

    projection.m_p3x = gamma_num * ((in_v1[0] - in_v3[0]) / gamma_den) + in_v3[0];
    projection.m_p3x = ((projection.m_p3x/w) >> 10) + 40;
    projection.m_p3y = gamma_num * ((in_v1[1] - in_v3[1]) / gamma_den) + in_v3[1];
    projection.m_p3y = ((projection.m_p3y/w) >> 10) + 32;
  }

  projection.color = in_color;

  in_engine->pushTriangle(projection);
}

/**
 * @brief Computes and pushes the triangls in the clipping area (farther than the near plane), when only 2 vertices of the input triangle is within the clipping area.
 * @param in_engine The engine.
 * @param in_v1 An MVP-transformed vertex in homogenous coordinates that is within the clipping area (farther than the near plane).
 * @param in_v2 An MVP-transformed vertex in homogenous coordinates that is within the clipping area (farther than the near plane).
 * @param in_v3 The MVP-transformed vertex in homogenous coordinates that is outside the clipping area (closer than the near plane, or behind camera).
 * @param in_color The triangle color.
 */
GROG_INLINE void pushClippedTriangle2(grog::Engine* in_engine,
                                      int32_t* in_v1 /* inside */,
                                      int32_t* in_v2 /* inside */,
                                      int32_t* in_v3 /* outside */,
                                      const Gamebuino_Meta::ColorIndex& in_color)
{
  Triangle projection1 , projection2;

  // The first vertex is unchanged.
  // Apply perspective, scale in the [-40,40] and [-32,32] domain, to fit the screen
  int32_t w = in_v1[3] >> 10;
  if(GROG_UNLIKELY(w==0))
    w = 1;
  projection1.m_p1x = projection2.m_p1x = ((in_v1[0]/w) >> 10) + 40;
  projection1.m_p1y = projection2.m_p1y = ((in_v1[1]/w) >> 10) + 32;
  projection1.m_z = projection2.m_z = in_v1[2] / w;

  // Same for v2
  w = in_v2[3] >> 10;
  if(GROG_UNLIKELY(w==0))
    w = 1;
  projection1.m_p2x = ((in_v2[0]/w) >> 10) + 40;
  projection1.m_p2y = ((in_v2[1]/w) >> 10) + 32;
  projection1.m_z = 3 * in_v2[2];

  // Now we are looking for gamma = gamma_num/gamma_den, such that gamma * v2 + (1-gamma) * v3 lies on the near plane, ie w=-z.
  int32_t gamma_num;
  int32_t gamma_den;

  {
    // Process v2 (in) vs v3 (out)
#define Z1 in_v2[2]
#define W1 in_v2[3]
#define Z2 in_v3[2]
#define W2 in_v3[3]
    gamma_num = - W2 - Z2;
    gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
    w = -(gamma_num * ((in_v2[2] - in_v3[2]) / gamma_den) + in_v3[2]) >> 10;
    if(GROG_UNLIKELY(w==0))
      w = 1;

    // We have our second vertex
    // Apply perspective, scale in the [-40,40] and [-32,32] domain, to fit the screen
    projection1.m_p3x = gamma_num * ((in_v2[0] - in_v3[0]) / gamma_den) + in_v3[0];
    projection1.m_p3x = projection2.m_p2x = ((projection1.m_p3x/w) >> 10) + 40;
    projection1.m_p3y = gamma_num * ((in_v2[1] - in_v3[1]) / gamma_den) + in_v3[1];
    projection1.m_p3y = projection2.m_p2y = ((projection1.m_p3y/w) >> 10) + 32;
  }


  // Same thing for the 4th vertex.
  {
    // Process v1 (in) vs v3 (out)
#define Z1 in_v1[2]
#define W1 in_v1[3]
#define Z2 in_v3[2]
#define W2 in_v3[3]
    gamma_num = - W2 - Z2;
    gamma_den = Z1 - Z2 - W2 + W1;
#undef Z1
#undef W1
#undef Z2
#undef W2
    w = -(gamma_num * ((in_v1[2] - in_v3[2]) / gamma_den) + in_v3[2]) >> 10;
    if(GROG_UNLIKELY(w==0))
      w = 1;

    projection2.m_p3x = gamma_num * ((in_v1[0] - in_v3[0]) / gamma_den) + in_v3[0];
    projection2.m_p3x = ((projection2.m_p3x/w) >> 10) + 40;
    projection2.m_p3y = gamma_num * ((in_v1[1] - in_v3[1]) / gamma_den) + in_v3[1];
    projection2.m_p3y = ((projection2.m_p3y/w) >> 10) + 32;
  }

  projection1.color = projection2.color = in_color;

  in_engine->pushTriangle(projection1);
  in_engine->pushTriangle(projection2);
}

/**
 * @brief Pushes a triangle with all vertices farther than the near plane (no clipping).
 * @param[in] in_engine The engine.
 * @param[in] in_v1 An MVP-transformed vertex in homogenous coordinates.
 * @param[in] in_v2 An MVP-transformed vertex in homogenous coordinates.
 * @param[in] in_v3 An MVP-transformed vertex in homogenous coordinates.
 * @param[in] in_color The triangle color.
 */
GROG_INLINE void pushUnclippedTriangle(grog::Engine* in_engine,
                                       int32_t* in_v1,
                                       int32_t* in_v2,
                                       int32_t* in_v3,
                                       const Gamebuino_Meta::ColorIndex& in_color)
{
  //  std::cout << "*** unclipped\n";
  Triangle projection;

  projection.m_p1x = (*in_v1++);
  projection.m_p1y = (*in_v1++);
  projection.m_z = (*in_v1++);
  int32_t w = (*in_v1++) >> 10;
  if(GROG_UNLIKELY(w == 0))
    w = 1;
  projection.m_p1x = ((projection.m_p1x/w) >> 10) + 40;
  projection.m_p1y = ((projection.m_p1y/w) >> 10) + 32;

  projection.m_p2x = (*in_v2++);
  projection.m_p2y = (*in_v2++);
  projection.m_z += (*in_v2++);
  w = (*in_v2++) >> 10;
  if(GROG_UNLIKELY(w == 0))
    w = 1;
  projection.m_p2x = ((projection.m_p2x/w) >> 10) + 40;
  projection.m_p2y = ((projection.m_p2y/w) >> 10) + 32;

  projection.m_p3x = (*in_v3++);
  projection.m_p3y = (*in_v3++);
  projection.m_z += (*in_v3++);
  w = (*in_v3++) >> 10;
  if(GROG_UNLIKELY(w == 0))
    w = 1;
  projection.m_p3x = ((projection.m_p3x/w) >> 10) + 40;
  projection.m_p3y = ((projection.m_p3y/w) >> 10) + 32;

  projection.color = in_color;

  in_engine->pushTriangle(projection);
}

void Engine::projectScene(const SceneNode *in_node, const Matrix &in_parentMvp, uint32_t in_pass) noexcept
{
  if(GROG_UNLIKELY(in_node == nullptr))
    return;

  Matrix mvp;
  Matrix::Transform(in_parentMvp, in_node->m_transform, mvp);

  if(in_pass & in_node->m_renderPass)
  {
    const Mesh& mesh = in_node->m_mesh;

    // project all the coordinates in transformedVertexBuffer
    {
      const int32_t* inVertexBuffer = mesh.m_vertexBuffer;
      int32_t* outTransformedVertexBuffer = m_transformedVertexBuffer;
      int32_t inX(0), inY(0), inZ(0);
      int32_t outZ(0);

      for(uint32_t vertexIndex = mesh.m_vertexCount; vertexIndex; --vertexIndex)
      {
        inX = (*inVertexBuffer++);
        inY = (*inVertexBuffer++);
        inZ = (*inVertexBuffer++);

        {
          outZ =  mvp.m_data[8] * inX +
              mvp.m_data[9] * inY +
              mvp.m_data[10] * inZ +
              mvp.m_data[11] * 1024;

          // X_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.m_data[0] * inX
              +  mvp.m_data[1] * inY
              +  mvp.m_data[2] * inZ
              +  mvp.m_data[3] * 1024;

          // Y_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.m_data[4] * inX
              +  mvp.m_data[5] * inY
              +  mvp.m_data[6] * inZ
              +  mvp.m_data[7] * 1024;

          // Z_ndc * 1024
          (*outTransformedVertexBuffer++) = outZ;

          // W_ndc * 1024
          (*outTransformedVertexBuffer++) =    mvp.m_data[12] * inX
              +  mvp.m_data[13] * inY
              +  mvp.m_data[14] * inZ
              +  mvp.m_data[15] * 1024;
        }
      }
    }
    // end project all the coordinates in transformedVertexBuffer

    // push triangles
    {
      const uint32_t* faceIter = mesh.m_faces;
      const Gamebuino_Meta::ColorIndex* colorIter = mesh.m_colors;
      int32_t* v1, *v2, *v3;
      for(uint32_t faceIndex = mesh.m_faceCount; faceIndex; --faceIndex)
      {
        uint32_t ndcFlag(0); // This flag indicates which faces fall within the clipping box.
        // To make things easier and faster, only worry about faces accross the near plane.
        // The near plane is such that z=-w in homogenous coordinates (corresponds to z=-1).
        v1 = m_transformedVertexBuffer + 4 * (*faceIter++);
        v2 = m_transformedVertexBuffer + 4 * (*faceIter++);
        v3 = m_transformedVertexBuffer + 4 * (*faceIter++);

        if(v1[2] > -v1[3])
          ndcFlag = 0b001;
        if(v2[2] > -v2[3])
          ndcFlag |= 0b010;
        if(v3[2] > -v3[3])
          ndcFlag |= 0b100;

        switch(ndcFlag)
        {
          case 0b000: // Face outside clipping box
            ++colorIter;
            break;

          case 0b001: // Face partially outside clipping box
            pushClippedTriangle1(this, v1, v2, v3, (*colorIter++));
            break;

          case 0b010: // Face partially outside clipping box
            pushClippedTriangle1(this, v2, v3, v1, (*colorIter++));
            break;

          case 0b011: // Face partially outside clipping box
            pushClippedTriangle2(this, v1, v2, v3, (*colorIter++));
            break;

          case 0b100: // Face partially outside clipping box
            pushClippedTriangle1(this, v3, v1, v2, (*colorIter++));
            break;

          case 0b101: // Face partially outside clipping box
            pushClippedTriangle2(this, v3, v1, v2, (*colorIter++));
            break;

          case 0b110: // Face partially outside clipping box
            pushClippedTriangle2(this, v2, v3, v1, (*colorIter++));
            break;

          case 0b111: // Face within clipping box
            pushUnclippedTriangle(this, v1, v2, v3, (*colorIter++));
            break;

          default: // ?
            ++colorIter;
            break;
        }
      }
    }
  }

  // recursive call to children
  {
    SceneNode** child = in_node->m_children;
    for(uint32_t childIndex = in_node->m_childCount;
        childIndex;
        --childIndex, ++child)
    {
      projectScene(*child, mvp, in_pass);
    }
  }
  // end recursive call
}


void Engine::projectScene(const SceneNode *in_node, uint32_t in_pass) noexcept
{
  Matrix mvp;
  Matrix::Transform(m_projection, m_view, mvp);

  projectScene(in_node, mvp, in_pass); // recursively descends through the whole scene.

  render(); // Draw the triangles we've gathered.
}

void Engine::render() noexcept
{
  const Triangle* currentTriangle = m_triangleStackHead;
  while(GROG_LIKELY(currentTriangle != nullptr)) // painter's algorithm
  {
#ifdef __linux
    currentTriangle->rasterize(display.buffer);
#else
    currentTriangle->rasterize((uint8_t*)gb.display._buffer);
#endif
    currentTriangle = currentTriangle->m_next;
  }

  passDone();
}

void Engine::setProjection(const Matrix& in_projection) noexcept
{
  m_projection = in_projection;
}

void Engine::setView(const TransformMatrix& in_view) noexcept
{
  m_view = in_view;
}

/**
 * @brief Tells whether a triangle is likely to be displayed.
 * Fast and rough test giving false positives, but no false negative.
 * @param[in] in_triangle The triangle to test.
 * @return Whether to bother trying to display this triangle.
 */
GROG_INLINE bool displayable(const Triangle& in_triangle) noexcept
{
  return !((in_triangle.m_p1x < 0 &&  in_triangle.m_p2x < 0 &&  in_triangle.m_p3x < 0) ||
           (in_triangle.m_p1y < 0 &&  in_triangle.m_p2y < 0 &&  in_triangle.m_p3y < 0) ||
           (in_triangle.m_p1x > 79 && in_triangle.m_p2x > 79 && in_triangle.m_p3x > 79) ||
           (in_triangle.m_p1y > 63 && in_triangle.m_p2y > 63 && in_triangle.m_p3y > 63));
}


void Engine::pushTriangle(Triangle& in_triangle) noexcept
{
  if(Math::Orient2d(in_triangle.m_p1x, in_triangle.m_p1y,
                    in_triangle.m_p2x, in_triangle.m_p2y,
                    in_triangle.m_p3x, in_triangle.m_p3y) <= 0)
  {
    // Not visible because of wrong orientation
    return;
  }

  if(!displayable(in_triangle))
  {
    // Not visible because outside of screen
    return;
  }

  if(m_triangleCount == m_maxTriangles) // Replace triangle
  {
    // The farthest triangle is removed, move the triangle stack head one step further, and free room for the new triangle.
    Triangle* insertedTriangle = m_triangleStackHead;
    m_triangleStackHead = m_triangleStackHead->m_next;
    *insertedTriangle = in_triangle;

    {
      Triangle* current = m_triangleStackHead;
      while(current->m_next && current->m_next->m_z > insertedTriangle->m_z)
      {
        current = current->m_next;
      }

      if(GROG_UNLIKELY(current->m_next == nullptr))
      {
        insertedTriangle->m_next = nullptr;
        current->m_next = insertedTriangle;
      }
      else // current->z > insertedTriangle->z && current->next->z <= insertedTriangle->z
      {
        insertedTriangle->m_next= current->m_next;
        current->m_next = insertedTriangle;
      }
    }

  }
  else
  {
    // standard insertion

    Triangle* insertedTriangle = m_triangleStack + m_triangleCount;
    *insertedTriangle = in_triangle;

    if(GROG_UNLIKELY(m_triangleCount == 0))
    {
      insertedTriangle->m_next = nullptr;
      m_triangleStackHead = insertedTriangle;
    }
    else
    {
      Triangle* current = m_triangleStackHead;

      if(GROG_UNLIKELY(current->m_z <= insertedTriangle->m_z))
      {
        insertedTriangle->m_next = current;
        m_triangleStackHead = insertedTriangle;
      }
      else
      {
        while(current->m_next && current->m_next->m_z > insertedTriangle->m_z)
        {
          current = current->m_next;
        }

        if(GROG_UNLIKELY(current->m_next == nullptr))
        {
          insertedTriangle->m_next = nullptr;
          current->m_next = insertedTriangle;
        }
        else
        {
          insertedTriangle->m_next= current->m_next;
          current->m_next = insertedTriangle;
        }
      }

    }

    ++m_triangleCount;
  }
}

void Engine::passDone() noexcept
{
  m_triangleCount = 0;
  m_triangleStackHead = nullptr;
}
