#pragma once

#include "SceneNode.h"
#include "Display.h"
#ifdef __linux
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
                uint32_t maxTriangles,
                uint32_t maxTransforms
#ifdef __linux
                , QPixmap* pixmap
#endif
                ) noexcept;

      void projectScene(const SceneNode* node) noexcept;

      void render(bool finalPass) noexcept;

      void setProjection(const Matrix& projection) noexcept;
      void setView(const TransformMatrix& view) noexcept;

      void debugTriangleStack();

      void pushDebugTriangles();

    protected:

      void projectScene(const SceneNode* node,
                        const Matrix& parentMvp)noexcept;

      void pushTriangle(Triangle& in) noexcept;

      void newFrame();

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

      Display display {};

  };
}
