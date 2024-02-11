#pragma once
#include "util.h"

class Material
{
public:
	virtual ~Material() = default;
	virtual bool scatterLight(const Ray& inputRay, const HitRecord& hitRec, Vec3 attenuationColor, Ray& scatteredRay) const = 0;
};