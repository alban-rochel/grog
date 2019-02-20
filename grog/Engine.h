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

    protected:

      void pushTransform(const TransformMatrix& transform) noexcept;

      void popTransform() noexcept;

      void pushTriangle(Triangle& in) noexcept;

      coord* transformedVertexBuffer {};

      TransformMatrix* transformStack{};
      uint32_t nextTransformIndex{0};
      uint32_t transformCount{0};

  };
}
