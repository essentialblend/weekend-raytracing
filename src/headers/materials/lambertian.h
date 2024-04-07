#pragma once


class MLambertian : public Material
{
public:
	MLambertian(const ColorVec3& albedo) : albedoTex(std::make_shared<TSolidColor>(albedo)) {}

	MLambertian(std::shared_ptr<Texture> tex) : albedoTex(tex) {}

	bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation, double& pdf) const override
	{
		
		colorAttenuation = albedoTex->getTexColorAtCoords(hitRec.hitTexU, hitRec.hitTexV, hitRec.hitPoint);
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