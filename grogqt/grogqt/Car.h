#pragma once

#include "SceneNode.h"

namespace grog
{
  class Car: public SceneNode
  {
    public:
      explicit Car(bool mainCar);

      void setWheelRotation(int32_t rotation) noexcept;

      protected:

  };
}

