#pragma once

#include "../base/material.h"

class MDielectric : public Material
{
public:
	MDielectric(double refIn) : refractionIndex(refIn) {}
	
	bool scatterRay(const Ray& inputRay, const HitRecord& hitRec, Vec3& attenuationVal, Ray& scatteredRay) const override
	{
		attenuationVal = Vec3(1.f);
		double refractionRatio = hitRec.frontFace ? (1.f / refractionIndex) : refractionIndex;

		Vec3 unitDirection = computeUnitVector(inputRay.getRayDirection());

		double cosTheta = std::fmin(computeDotProduct(-unitDirection, hitRec.hitNormalVec), 1.f);
		double sinTheta = std::sqrt(1.f - (cosTheta * cosTheta));
		bool cannotRefract = (refractionRatio * sinTheta) > 1.f;
		Vec3 rayDir{ 0.f };

		if (cannotRefract || computeSchlickApprox(cosTheta, refractionRatio) > UGenRNGDouble())
		{
			rayDir = genReflectedRay(unitDirection, hitRec.hitNormalVec);
		}
		else
		{
			rayDir = genRefractedRay(unitDirection, hitRec.hitNormalVec, refractionRatio);
		}

		scatteredRay = Ray(hitRec.hitPoint, rayDir);
		return true;
	}


private:
	double refractionIndex;

	static double computeSchlickApprox(double cosine, double refIDX)
	{
		double rZ = (1 - refIDX) / (1 + refIDX);
		rZ *= rZ;
		return rZ + ((1 - rZ) * std::pow((1 - cosine), 5));
	}


};