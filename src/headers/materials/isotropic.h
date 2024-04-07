#pragma once

class MIsotropic : public Material
{
public:
	MIsotropic(ColorVec3 color) : albedoTex(std::make_shared<TSolidColor>(color)) {}
	MIsotropic(std::shared_ptr<Texture> tex) : albedoTex(tex) {}

	bool handleRayScatter(const Ray& inputRay, const HitRecord& hitRec, ScatterRecord& scattRec) const override
	{
		scattRec.attenuationParam = albedoTex->getTexColorAtCoords(hitRec.hitTexU, hitRec.hitTexV, hitRec.hitPoint);
		scattRec.PDFPointer = std::make_shared<PDFSphere>();
		scattRec.skipPDF = false;
		return true;
	}

	virtual double scatteringPDF(const Ray& inputRay, const HitRecord& hitRec, const Ray& scatteredRay) const
	{
		return 1 / (4 * Upi);
	}

private:
	std::shared_ptr<Texture> albedoTex;
};