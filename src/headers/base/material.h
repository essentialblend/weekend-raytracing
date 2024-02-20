#pragma once

class Material
{
public:
	virtual ~Material() = default;

	virtual ColorVec3 emittedLight(double u, double v, const PointVec3& p) const {
		return ColorVec3(0);
	}

	virtual bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation) const = 0;
};