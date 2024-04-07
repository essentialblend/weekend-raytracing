#pragma once


class MMetal : public Material
{
public:
	MMetal(const ColorVec3& albedo, double fuzzFactor) : albedoTex(albedo), fuzzFactor(fuzzFactor < 1 ? fuzzFactor : 1) {}

	bool handleRayScatter(const Ray& inputRay, const HitRecord& hitRec, ScatterRecord& scattRec) const override
	{
		scattRec.attenuationParam = albedoTex;
		scattRec.PDFPointer = nullptr;
		scattRec.skipPDF = true;

		Vec3 newReflRayDir = computeReflectionDirection(computeUnitVector(inputRay.getRayDirection()), hitRec.hitNormalVec);
		scattRec.skipPDFRay = Ray(hitRec.hitPoint, newReflRayDir + (fuzzFactor * genNormalizedRandVec3UnitSphere()), inputRay.getRayTime());

		return true;
	}

private:
	ColorVec3 albedoTex;
	double fuzzFactor;

};