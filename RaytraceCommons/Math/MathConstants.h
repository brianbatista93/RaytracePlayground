#pragma once

#include <limits>
#include <random>

const float infinity = std::numeric_limits<float>::infinity();
const float pi       = 3.1415926535897932385f;

constexpr float
DegToRad(float degrees)
{
    return degrees * pi / 180.0f;
}

inline float
Random()
{
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937                          generator;
    return distribution(generator);
}

inline float
Random(float min, float max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * Random();
}

#include "Math/Vector3D.h"

[[nodiscard]] inline Vector3D
RandomVector()
{
    return Vector3D(Random(), Random(), Random());
}

[[nodiscard]] inline Vector3D
RandomVector(float min, float max)
{
    return Vector3D(Random(min, max), Random(min, max), Random(min, max));
}

[[nodiscard]] inline Vector3D
RandomInUnitSphere()
{
    while (true) {
        Vector3D p = RandomVector(-1.0f, 1.0f);
        if (p.LengthSquared() >= 1.0f)
            continue;
        return p;
    }
}

[[nodiscard]] inline Vector3D
RandomUnitSphere()
{
    return Normalize(RandomInUnitSphere());
}

[[nodiscard]] inline Vector3D
RandomInHemisphere(const Vector3D& normal)
{
    Vector3D inUnitSphere = RandomInUnitSphere();
    if (Dot(inUnitSphere, normal) > 0.0f) {
        return inUnitSphere;
    }

    return -inUnitSphere;
}
