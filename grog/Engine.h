#pragma once

#include "SceneNode.h"

namespace grog
{
  class Engine
  {
    public:
      Engine() noexcept;

      ~Engine() noexcept;

      void init(uint32_t maxVerticesPerMesh,
                uint32_t maxTriangles,
                uint32_t maxTransforms) noexcept;

      void projectScene(const SceneNode* node) noexcept;

      void render(bufferType* frameBuffer) noexcept;

      void setProjection(const Matrix& projection) noexcept;
      void setView(const TransformMatrix& view) noexcept;

    protected:

      void projectScene(const SceneNode* node,
                        const Matrix& parentMvp)noexcept;

      void pushTransform(const TransformMatrix& transform) noexcept;

      void popTransform() noexcept;

      void pushTriangle(Triangle& in) noexcept;

      void newFrame();

      coord* transformedVertexBuffer {nullptr};

      Matrix projection;
      TransformMatrix view;

//      TransformMatrix* transformStack{nullptr};
//      uint32_t nextTransformIndex{0};
//      uint32_t transformCount{0};

      Triangle* triangleStack {nullptr};
      Triangle* triangleStackHead {nullptr};
      uint32_t triangleCount {0};
      uint32_t maxTriangles {0};

  };
}
