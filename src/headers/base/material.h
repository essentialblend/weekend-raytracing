#pragma once

class Material
{
public:
	virtual ~Material() = default;

	virtual ColorVec3 getEmittedLight(const Ray& inputRay, const HitRecord& hitRec, double u, double v, const PointVec3& p) const {
		return ColorVec3(0);
	}

	virtual bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation, double& pdf) const = 0;

	virtual double scatteringPDF(const Ray& inputRay, const HitRecord& hitRec, const Ray& scatteredRay) const
	{
		return 0;
	}
};