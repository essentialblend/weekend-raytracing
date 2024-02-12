#pragma once
#include ".././base/material.h"

class MMetal : public Material
{
public:
	MMetal(const Vec3& albedoCol, double fuzzF) : albedoMetal(albedoCol), fuzzFactor(fuzzF < 1 ? fuzzF : 1) {}

	bool scatterRay(const Ray& inputRay, const HitRecord& hitRec, Vec3& attenuationVal, Ray& scatteredRay) const override
	{
		Vec3 reflectedRay = genReflectedRay(computeUnitVector(inputRay.getRayDirection()), hitRec.hitNormalVec);
		scatteredRay = Ray(hitRec.hitPoint, reflectedRay + (fuzzFactor * computeUnitVector(genRandVecInUnitSphere())));
		attenuationVal = albedoMetal;
		return (computeDotProduct(scatteredRay.getRayDirection(), hitRec.hitNormalVec) > 0);
	}

private:
	Vec3 albedoMetal;
	double fuzzFactor;
};