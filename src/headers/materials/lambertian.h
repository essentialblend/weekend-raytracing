#pragma once


class MLambertian : public Material
{
public:
	MLambertian(const ColorVec3& albedo) : albedoTex(std::make_shared<TSolidColor>(albedo)) {}

	MLambertian(std::shared_ptr<Texture> tex) : albedoTex(tex) {}

	bool handleRayScatter(const Ray& inputRay, const HitRecord& hitRec, ScatterRecord& scattRec) const override
	{
		scattRec.attenuationParam = albedoTex->getTexColorAtCoords(hitRec.hitTexU, hitRec.hitTexV, hitRec.hitPoint);
		scattRec.PDFPointer = std::make_shared<PDFCosine>(hitRec.hitNormalVec);
		scattRec.skipPDF = false;

		return true;
	}

	virtual double scatteringPDF(const Ray& inputRay, const HitRecord& hitRec, const Ray& scatteredRay) const
	{
		auto cosTheta = computeDotProduct(hitRec.hitNormalVec, computeUnitVector(scatteredRay.getRayDirection()));
		if (cosTheta < 0)
		{
			return 0;
		}
		else
		{
			return cosTheta / Upi;
		}
	}

private:
	std::shared_ptr<Texture> albedoTex;

};