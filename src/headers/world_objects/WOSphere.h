#pragma once
#include "../base/worldObject.h"

class WOSphere : public WorldObject
{
public:
	WOSphere(Vec3 sphereCenter, double sphereRad, std::shared_ptr<Material> WOMaterial) : sphereCenter(sphereCenter), sphereRad(sphereRad), WOMaterial(WOMaterial) {}

	bool rayHit(const Ray& currRay, Interval validRayInterval, HitRecord& hitRec) const override
	{
		Vec3 centerToOriginDir = currRay.getRayOrigin() - sphereCenter;

		// a, b, and c for the quadratic equation.
		double a{ currRay.getRayDirection().getLengthSquared() };
		double halfB{ computeDotProduct(centerToOriginDir, currRay.getRayDirection()) };
		double c{ centerToOriginDir.getLengthSquared() - (sphereRad * sphereRad) };
		double quadDiscr{ (halfB * halfB) - (a * c) };

		if (quadDiscr < 0) return false;

		double discrSqrRoot{ std::sqrt(quadDiscr) };

		// Find nearest root in acceptable range.
		auto minRoot{ (-halfB - discrSqrRoot) / a };
		if (!validRayInterval.isStrictlyWithinBounds(minRoot))
		{
			minRoot = (-halfB + discrSqrRoot) / a;
			if(!validRayInterval.isStrictlyWithinBounds(minRoot)) return false;
		}

		hitRec.hitRoot = minRoot;
		hitRec.hitPoint = currRay.getPointOnRayAt(hitRec.hitRoot);
		hitRec.hitNormalVec = (hitRec.hitPoint - sphereCenter) / sphereRad;

		hitRec.setFaceNormal(currRay, hitRec.hitNormalVec);
		hitRec.hitRecMaterial = WOMaterial;

		return true;
;	}

private:
	Vec3 sphereCenter;
	double sphereRad;
	std::shared_ptr<Material> WOMaterial;
};