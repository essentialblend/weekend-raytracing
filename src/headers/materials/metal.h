#pragma once


class MMetal : public Material
{
public:
	MMetal(const ColorVec3& albedo, double fuzzFactor) : albedoTex(albedo), fuzzFactor(fuzzFactor < 1 ? fuzzFactor : 1) {}

	bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation, double& pdf) const override
	{
		Vec3 newReflRayDir = computeReflectionDirection(computeUnitVector(inputRay.getRayDirection()), hitRec.hitNormalVec);

		scatteredRay = Ray(hitRec.hitPoint, newReflRayDir + (fuzzFactor * genNormalizedRandVec3UnitSphere()), inputRay.getRayTime());
		colorAttenuation = albedoTex;
		return computeDotProduct(scatteredRay.getRayDirection(), hitRec.hitNormalVec) > 0;
	}

private:
	ColorVec3 albedoTex;
	double fuzzFactor;

};