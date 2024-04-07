#pragma once

class MEmissive : public Material
{
public:
	MEmissive(std::shared_ptr<Texture> tex) : emittedTex(tex) {}
	MEmissive(ColorVec3 lightCol) : emittedTex(std::make_shared<TSolidColor>(lightCol)) {}

	bool handleRayScatter(const Ray& inputRay, const HitRecord& hitRec, ScatterRecord& scattRec) const override
	{
		return false;
	}

	virtual ColorVec3 getEmittedLight(const Ray& inputRay, const HitRecord& hitRec, double u, double v, const PointVec3& p) const override
	{
		if (!hitRec.frontFace)
			return ColorVec3(0);
		return emittedTex->getTexColorAtCoords(u, v, p);
	}

private:
	std::shared_ptr<Texture> emittedTex;
};