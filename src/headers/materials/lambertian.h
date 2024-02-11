#pragma once

#include "../base/material.h"

class MLambertian : public Material
{
public:
	MLambertian(const Vec3& matCol) : albedoLambertian(matCol) {}

	bool scatterLight(const Ray& inputRay, const HitRecord& hitRec, Vec3& attenuationVal, Ray& scatteredRay) const override
	{
		Vec3 scatteredRayDir = hitRec.hitNormalVec + computeUnitVector(genRandVecInUnitSphere());

		if (scatteredRayDir.checkZeroScatter())
		{
			scatteredRayDir = hitRec.hitNormalVec;
		}

		scatteredRay = Ray(hitRec.hitPoint, scatteredRayDir);
		attenuationVal = albedoLambertian;
		return true;
	}
private:
	Vec3 albedoLambertian{ 0.f };


};