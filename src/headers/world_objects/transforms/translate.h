#pragma once

class WOTranslate : public WorldObject
{
public:
	WOTranslate(std::shared_ptr<WorldObject> wObj, const Vec3& displacementFac) : objectInstance(wObj), offsetFactor(displacementFac)
	{
		translateBBox = objectInstance->getWOBoundingBox() + offsetFactor;
	}
	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const override
	{
		// Shift the ray origin opposite to desired translation.
		Ray offsetRay(inputRay.getRayOrigin() - offsetFactor, inputRay.getRayDirection(), inputRay.getRayTime());

		// Determine intersection along the offset ray.
		if (!objectInstance->checkHit(offsetRay, validInterval, hitRec))
		{
			return false;
		}

		// Move intersection point forwards by offsetFactor.
		hitRec.hitPoint += offsetFactor;
		return true;
	}

	AABB getWOBoundingBox() const override { return translateBBox; }

private:
	std::shared_ptr<WorldObject> objectInstance;
	Vec3 offsetFactor;
	AABB translateBBox;
};