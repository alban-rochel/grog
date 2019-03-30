#pragma once

#include "SceneNode.h"
#ifdef __linux
#include "Display.h"
#include <QPixmap>
#endif

namespace grog
{
  class Engine
  {
    public:
      Engine() noexcept;

      ~Engine() noexcept;

      void init(uint32_t maxVerticesPerMesh,
                uint32_t maxTriangles
#ifdef __linux
                , QPixmap* pixmap
#endif
                ) noexcept;

      void projectScene(const SceneNode* node, uint32_t pass) noexcept;

      //void setProjection(const Matrix& projection) noexcept;
      void setProjection(float fov,
                         float near,
                         float far) noexcept;
      void setView(const TransformMatrix& view) noexcept;

#ifdef __linux
      void debugTriangleStack();

      void pushDebugTriangles();
#endif

    protected:

      void render() noexcept;

      void projectScene(const SceneNode* node,
                        const Matrix& parentMvp,
                        uint32_t pass) noexcept;

      void pushTriangle(Triangle& in) noexcept;

      void passDone() noexcept;

      int32_t* transformedVertexBuffer {nullptr};

      Matrix projection;
      TransformMatrix view;

//      TransformMatrix* transformStack{nullptr};
//      uint32_t nextTransformIndex{0};
//      uint32_t transformCount{0};

      Triangle* triangleStack {nullptr};
      Triangle* triangleStackHead {nullptr};
      uint32_t triangleCount {0};
      uint32_t maxTriangles {0};

      uint32_t near{0};

#ifdef __linux
    public:
      Display display {};
#endif

  };
}
