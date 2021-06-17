#pragma once

#include "Hittable.h"
#include "pch.h"

class HittableList : public IHittable
{
  public:
    HittableList() {}

    HittableList(std::shared_ptr<IHittable> object) { Add(object); }

    void Clear() { Objects.clear(); }

    void Add(std::shared_ptr<IHittable> object) { Objects.push_back(object); }

    virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

  public:
    std::vector<std::shared_ptr<IHittable>> Objects;
};

bool
HittableList::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    HitRecord tempRec;
    bool      hasHitAnything = false;
    float     closestSoFar   = t_max;

    for (const auto& object : Objects) {
        if (object->Hit(r, t_min, closestSoFar, tempRec)) {
            hasHitAnything = true;
            closestSoFar   = tempRec.T;
            rec            = tempRec;
        }
    }

    return hasHitAnything;
}