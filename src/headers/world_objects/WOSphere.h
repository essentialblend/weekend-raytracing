#pragma once
#include "../base/worldObject.h"

class WOSphere : public WorldObject
{
public:
	WOSphere(Vec3 sphereCenter, double sphereRad) : sphereCenter(sphereCenter), sphereRad(sphereRad) {}

	bool rayHit(const Ray& currRay, UInterval validRayInterval, HitRecord& hitRec) const override
	{
		Vec3 centerToOriginDir = currRay.getRayOrigin() - sphereCenter;

		// a, b, and c for the quadratic equation.
		double a = currRay.getRayDirection().getLengthSquared();
		double halfB = computeDotProduct(centerToOriginDir, currRay.getRayDirection());
		double c = centerToOriginDir.getLengthSquared() - (sphereRad * sphereRad);
		double quadDiscr = (halfB * halfB) - (a * c);

		if (quadDiscr < 0) return false;

		double discrSqrRoot = std::sqrt(quadDiscr);

		// Find nearest root in acceptable range.
		auto minRoot = (-halfB - discrSqrRoot) / a;
		if (!validRayInterval.isStrictlyWithinBounds(minRoot))
		{
			minRoot = (-halfB + discrSqrRoot) / a;
			if(!validRayInterval.isStrictlyWithinBounds(minRoot)) return false;
		}

		hitRec.rayT = minRoot;
		hitRec.hitPoint = currRay.getPointOnRayAt(hitRec.rayT);
		hitRec.pointNormal = (hitRec.hitPoint - sphereCenter) / sphereRad;

		Vec3 outwardNormal = (hitRec.hitPoint - sphereCenter) / sphereRad;
		hitRec.setFaceNormal(currRay, outwardNormal);

		return true;
;	}

private:
	Vec3 sphereCenter;
	double sphereRad;
};