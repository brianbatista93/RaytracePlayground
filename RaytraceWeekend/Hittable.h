#pragma once

#include "Math/Ray.h"

class IMaterial;

struct HitRecord
{
    Vector3D                   Position;
    Vector3D                   Normal;
    std::shared_ptr<IMaterial> matPtr;
    float                      T;
    bool                       IsFrontFace;

    inline void SetFaceNormal(const Ray& r, const Vector3D& outwardNormal)
    {
        IsFrontFace = Dot(r.Direction, outwardNormal) < 0;
        Normal      = IsFrontFace ? outwardNormal : -outwardNormal;
    }
};

class IHittable
{
  public:
    virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};