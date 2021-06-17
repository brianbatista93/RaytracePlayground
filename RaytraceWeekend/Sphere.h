#pragma once

#include "Hittable.h"
#include <cmath>

struct Sphere : public IHittable
{
    Sphere() {}

    Sphere(const Vector3D& center, float radius)
      : Center(center)
      , Radius(radius)
    {}

    virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;

    Vector3D Center;
    float    Radius;
};

bool
Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    Vector3D oc    = r.Origin - Center;
    float    a     = r.Direction.LengthSquared();
    float    halfB = Dot(oc, r.Direction);
    float    c     = oc.LengthSquared() - Radius * Radius;

    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0.0f)
        return false;

    float sqrtd = sqrtf(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-halfB - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-halfB + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.T                  = root;
    rec.Position           = r(rec.T);
    Vector3D outwardNormal = (rec.Position - Center) / Radius;
    rec.SetFaceNormal(r, outwardNormal);

    return true;
}