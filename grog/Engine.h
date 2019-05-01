#pragma once

#include "grog.h"
#include "SceneNode.h"
#include "Triangle.h"
#ifdef __linux
#include <QPixmap>
#include "Display.h"
#endif

namespace grog
{
  /**
   * @brief The Engine is the entry point to manage the various stages of the rendering process.
   */
  class Engine
  {
    public:
      /**
       * @brief Constructor.
       */
      Engine() noexcept = default;

      /**
       * @brief Destructor.
       */
      ~Engine() noexcept;

      /**
       * @brief Sets up the engine by allocating the memory segments it will use.
       * @param[in] in_maxVerticesPerMesh The maximum number of vertices per mesh. This is used to size the buffer of transformed vertices properly
       * @param[in] in_maxTriangles The maximum number of triangles to draw. This is use to size the queue of triangles to render. Only the closest triangles are kept for rasterization.
       * @warning Providing too small a value for in_maxVerticesPerMesh will result in a crash.
       * @warning Providing too large a value for in_maxVerticesPerMesh will eat up more memory than needed.
       * @warning Providing too small a value for in_maxTriangles will result in too few triangles being drawn.
       * @warning Providing too large a value for in_maxTriangles will eat up more memory than needed and result in longer rendering times.
       * @throws std::bad_alloc if allocation failed.
       */
      void init(uint32_t in_maxVerticesPerMesh,
                uint32_t in_maxTriangles
#ifdef __linux
                , QPixmap* pixmap
#endif
                ) noexcept;

      /**
       * @brief Renders a node and its children to the frame buffer, if they are the provided pass.
       * @param[in] in_node The node to draw.
       * @param[in] in_pass The pass to draw.
       */
      void projectScene(const SceneNode* in_node, uint32_t in_pass) noexcept;

      /**
       * @brief Sets the projection for the next rendering passes.
       * @param[in] in_projection The new projection.
       */
      void setProjection(const Matrix& in_projection) noexcept;

      /**
       * @brief Sets the view transformation for the next rendering passes.
       * @param[in] in_view The new view.
       */
      void setView(const TransformMatrix& in_view) noexcept;

      /**
       * @brief Adds a triangle to the list of triangles to draw during the current pass.
       * @param[in] in_triangle The triangle to add.
       * The triangle is kept if and only if there are fewer triangles in queue than expected, or it this triangle is closer to another one. In this case, the farthest one will be discarded.
       */
      void pushTriangle(Triangle& in_triangle) noexcept;

    protected:

      /**
       * @brief Rasterizes the triangles in queue, and closes the current pass.
       */
      void render() noexcept;

      /**
       * @brief Transforms and projects the scene node accordingly to the current transformations.
       * @param in_node The node to project.
       * @param in_parentMvp The parent Model/View/Projection matrix.
       * @param in_pass The current pass.
       */
      void projectScene(const SceneNode* in_node,
                        const Matrix& in_parentMvp,
                        uint32_t in_pass) noexcept;

      /**
       * @brief Cleans up the current rendering pass.
       */
      void passDone() noexcept;

      int32_t* m_transformedVertexBuffer {nullptr}; ///< Temporary buffer of vertices transformed according to current Model/View/Projection.

      Matrix m_projection; ///< Projection (perspective) matrix.
      TransformMatrix m_view; ///< View transform matrix.

      Triangle* m_triangleStack {nullptr}; ///< A buffer of triangles.
      Triangle* m_triangleStackHead {nullptr}; ///< The head of the list of triangles to rasterize. The triangles are allocated in m_triangleStack.
      uint32_t m_triangleCount {0}; ///< Number of triangles in stack.
      uint32_t m_maxTriangles {0}; ///< Maximum number of triangles in stack.

#ifdef __linux
    public:
      Display display {};
#endif

  };
}
