#pragma once

#include "Math/MathConstants.h"
#include "Math/Ray.h"
#include "Math/Vector3D.h"

class Camera
{
  public:
    Camera()
    {
        float aspect_ratio    = 16.0f / 9.0f;
        float viewport_height = 2.0f;
        float viewport_width  = aspect_ratio * viewport_height;
        float focal_length    = 1.0f;

        origin            = Vector3D(0.0f, 0.0f, 0.0f);
        horizontal        = Vector3D(viewport_width, 0.0f, 0.0f);
        vertical          = Vector3D(0.0f, viewport_height, 0.0f);
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vector3D(0.0f, 0.0f, focal_length);
    }

    Ray GetRay(float u, float v) const { return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin); }

  private:
    Vector3D origin;
    Vector3D lower_left_corner;
    Vector3D horizontal;
    Vector3D vertical;
};