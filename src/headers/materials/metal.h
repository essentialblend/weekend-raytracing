#pragma once
#include ".././base/material.h"

class MMetal : public Material
{
public:
	MMetal(const Vec3& albedoCol) : albedoMetal(albedoCol) {}

	bool scatterLight(const Ray& inputRay, const HitRecord& hitRec, Vec3& attenuationVal, Ray& scatteredRay) const override
	{
		Vec3 reflectedRay = reflectRay(computeUnitVector(inputRay.getRayDirection()), hitRec.hitNormalVec);
		scatteredRay = Ray(hitRec.hitPoint, reflectedRay);
		attenuationVal = albedoMetal;
		return true;
	}

private:
	Vec3 albedoMetal;
};