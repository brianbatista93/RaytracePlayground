#pragma once

#include "Hittable.h"
#include "Math/Color.h"
#include "Math/MathConstants.h"
#include "Math/Ray.h"
#include "Sphere.h"

class IMaterial
{
  public:
    virtual bool Scatter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) = 0;

    virtual Color Emit() { return Color(0.0f, 0.0f, 0.0f); }
};

class LambertMaterial : public IMaterial
{
  public:
    LambertMaterial(const Color& color)
      : Albedo(color)
    {}

    virtual bool Scatter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) override
    {
        Vector3D scatterDirection = RandomInHemisphere(rec.Normal);

        if (scatterDirection.IsNearZero()) {
            scatterDirection = rec.Normal;
        }

        scattered   = Ray(rec.Position, scatterDirection);
        attenuation = Albedo;
        return true;
    }

    Color Albedo;
};

class MetalMaterial : public IMaterial
{
  public:
    MetalMaterial(const Color& color, float fuzz)
      : Albedo(color)
      , Fuzz(fuzz)
    {}

    virtual bool Scatter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) override
    {
        Vector3D reflected = Reflect(Normalize(ray.Direction), rec.Normal);
        scattered          = Ray(rec.Position, reflected + Fuzz * RandomInUnitSphere());
        attenuation        = Albedo;
        return (Dot(scattered.Direction, rec.Normal) > 0.0f);
    }

    Color Albedo;
    float Fuzz;
};

class EmissiveMaterial : public IMaterial
{
  public:
    EmissiveMaterial(const Color& color)
      : Albedo(color)
    {}

    virtual bool Scatter(const Ray& ray, const HitRecord& rec, Color& attenuation, Ray& scattered) override { return false; }

    virtual Color Emit() { return Albedo.A * Albedo; }

    Color Albedo;
};