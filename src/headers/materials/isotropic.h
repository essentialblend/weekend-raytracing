#pragma once

class MIsotropic : public Material
{
public:
	MIsotropic(ColorVec3 color) : albedoVal(std::make_shared<TSolidColor>(color)) {}
	MIsotropic(std::shared_ptr<Texture> tex) : albedoVal(tex) {}

	bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation) const override
	{
		scatteredRay = Ray(hitRec.hitPoint, genNormalizedRandVec3UnitSphere(), inputRay.getRayTime());
		colorAttenuation = albedoVal->getTexColorAtCoords(hitRec.hitTexU, hitRec.hitTexV, hitRec.hitPoint);
		return true;
	}

private:
	std::shared_ptr<Texture> albedoVal;
};