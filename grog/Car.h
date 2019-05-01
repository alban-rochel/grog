#pragma once

#include "SceneNode.h"

namespace demo
{
  class Car: public grog::SceneNode
  {
    public:
      explicit Car(bool mainCar);

      void setWheelRotation(int32_t rotation) noexcept;

      protected:

  };
}

