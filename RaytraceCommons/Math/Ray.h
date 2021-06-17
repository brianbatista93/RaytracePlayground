#pragma once

#include "Math/Vector3D.h"

struct Ray
{
    constexpr Ray() {}

    constexpr Ray(const Vector3D& origin, const Vector3D direction)
      : Origin(origin)
      , Direction(direction)
    {}

    Vector3D operator()(double t) const { return Origin + t * Direction; }

    Vector3D Origin;
    Vector3D Direction;
};