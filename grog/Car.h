#pragma once

#include "SceneNode.h"

namespace demo
{
  /**
   * @brief The Car class is a scene node containing the full description of a car.
   * It contains pointers to the meshes, but also the hierarchy of meshes (wheels etc.).
   */
  class Car: public grog::SceneNode
  {
    public:
      /**
       * @brief Constructor.
       * @param[in] in_mainCar Whether the car is the main one (red) or not (blue).
       * @throws std::bad_alloc if allocation fails.
       */
      explicit Car(bool in_mainCar);

      /**
       * @brief Sets the absolute rotation of the wheels.
       * @param in_rotation The absolute rotation, as 1024th of \f$2*\pi\f$.
       * @see grog::Math::Cos().
       */
      void setWheelRotation(int32_t in_rotation) noexcept;
  };
}

