#pragma once
#include "util.h"

class Material
{
public:
	virtual ~Material() = default;
	virtual bool scatterRay(const Ray& inputRay, const HitRecord& hitRec, Vec3& attenuationVal, Ray& scatteredRay) const = 0;
};
