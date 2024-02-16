#pragma once

class WOSphere : public WorldObject
{
public:
	// Stationary sphere.
	WOSphere(PointVec3 sphC, double sphR, std::shared_ptr<Material> assignMat) : initialSphereCenter(sphC), sphereRadius(sphR), sphereMaterial(assignMat), isMoving(false) {}

	// Moving sphere.
	WOSphere(PointVec3 sphC, PointVec3 finalC, double sphR, std::shared_ptr<Material> assignMat) : initialSphereCenter(sphC), sphereRadius(sphR), sphereMaterial(assignMat), isMoving(true) 
	{
		centerVec = finalC - initialSphereCenter;
	}


	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const override
	{
		// Alter sphereCenter based on the ray's time.
		Vec3 sphereCenterToRayOriginDir = inputRay.getRayOrigin() - (isMoving ? moveSphere(inputRay.getRayTime()) : initialSphereCenter);
		// Quadratic coeffs: a = b . b, b = 2b . (A - C), c = (A - C) . (A - C) - r^2.
		double a{ inputRay.getRayDirection().computeMagnitudeSquared() };
		double half_b{ computeDotProduct(sphereCenterToRayOriginDir, inputRay.getRayDirection()) };
		double c{ sphereCenterToRayOriginDir.computeMagnitudeSquared() - (sphereRadius * sphereRadius) };
		double discr{ (half_b * half_b) - (a * c) };

		if (discr < 0) return false;

		double solutionRoot{ (-half_b - std::sqrt(discr)) / a };
		if (!validInterval.isWithinInterval(solutionRoot))
		{
			solutionRoot = (-half_b + std::sqrt(discr)) / a;
			if (!validInterval.isWithinInterval(solutionRoot))
			{
				return false;
			}
		}
		hitRec.hitRoot = solutionRoot;
		hitRec.hitPoint = inputRay.getPointOnRayAt(hitRec.hitRoot);
		Vec3 outwardNormal = (hitRec.hitPoint - initialSphereCenter) / sphereRadius;
		hitRec.setFaceNormal(inputRay, outwardNormal);
		hitRec.hitMaterial = sphereMaterial;

		return true;
	}


private:
	PointVec3 initialSphereCenter;
	Vec3 centerVec;
	double sphereRadius;
	std::shared_ptr<Material> sphereMaterial;
	bool isMoving;

	PointVec3 moveSphere(double currTime) const
	{
		return (initialSphereCenter + (currTime * centerVec));
	}
};