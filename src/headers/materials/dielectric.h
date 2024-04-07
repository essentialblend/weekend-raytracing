#pragma once


class MDielectric : public Material
{
public:
	MDielectric(const double refInd) : refractionIndex(refInd) {}

	bool handleRayScatter(const Ray& inputRay, const HitRecord& hitRec, ScatterRecord& scattRec) const override
	{
		scattRec.attenuationParam = ColorVec3(1);
		scattRec.PDFPointer = nullptr;
		scattRec.skipPDF = true;

		double refractionRatio = hitRec.frontFace ? (1.f / refractionIndex) : refractionIndex;
		Vec3 unitDirectionVec = computeUnitVector(inputRay.getRayDirection());
		double cosineTheta{ std::fmin(computeDotProduct(-unitDirectionVec, hitRec.hitNormalVec), 1.f) };
		double sinTheta{ std::sqrt(1.f - (cosineTheta * cosineTheta)) };

		bool cannotRefract{ (refractionRatio * sinTheta) > 1.f };
		Vec3 rayDir;

		if (cannotRefract || computeSchlicksApproxForReflectance(cosineTheta, refractionRatio) > UGenRNGDouble())
		{
			rayDir = computeReflectionDirection(unitDirectionVec, hitRec.hitNormalVec);
		}
		else
		{
			rayDir = computeRefractionDirection(unitDirectionVec, hitRec.hitNormalVec, refractionRatio);
		}

		scattRec.skipPDFRay = Ray(hitRec.hitPoint, rayDir, inputRay.getRayTime());

		return true;
	}

private:
	double refractionIndex;

	static double computeSchlicksApproxForReflectance(double cosine, double refIDX)
	{
		double r0Coeff = (1 - refIDX) / (1 + refIDX);
		r0Coeff = r0Coeff * r0Coeff;
		return r0Coeff + ((1 - r0Coeff) * std::pow(1 - cosine, 5));
	}

};