#pragma once


class MLambertian : public Material
{
public:
	MLambertian(const ColorVec3& albedo) : albedoTex(std::make_shared<TSolidColor>(albedo)) {}

	MLambertian(std::shared_ptr<Texture> tex) : albedoTex(tex) {}

	bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation) const override
	{
		Vec3 newReflRayDir{ hitRec.hitNormalVec + genNormalizedRandVec3UnitSphere() };

		if (newReflRayDir.checkNearZero()) { newReflRayDir = hitRec.hitNormalVec; }
		
		scatteredRay = Ray(hitRec.hitPoint, newReflRayDir, inputRay.getRayTime());
		
		colorAttenuation = albedoTex->getTexColorAtCoords(hitRec.hitTexU, hitRec.hitTexV, hitRec.hitPoint);
		
		return true;
	}

private:
	std::shared_ptr<Texture> albedoTex;

};