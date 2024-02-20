#pragma once

class MEmissive : public Material
{
public:
	MEmissive(std::shared_ptr<Texture> tex) : emittedTex(tex) {}
	MEmissive(ColorVec3 lightCol) : emittedTex(std::make_shared<TSolidColor>(lightCol)) {}

	bool handleRayScatter(const Ray& inputRay, Ray& scatteredRay, const HitRecord& hitRec, ColorVec3& colorAttenuation) const override
	{
		return false;
	}

	virtual ColorVec3 getEmittedLight(double u, double v, const PointVec3& p) const override
	{
		return emittedTex->getTexColorAtCoords(u, v, p);
	}

private:
	std::shared_ptr<Texture> emittedTex;
};