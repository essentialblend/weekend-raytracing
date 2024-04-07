#pragma once

#include "../base/aabb.h"

#include "../base/onb.h"

class WOSphere : public WorldObject
{
public:
	// Stationary sphere.
	WOSphere(PointVec3 sphC, double sphR, std::shared_ptr<Material> assignMat) : initialSphereCenter(sphC), sphereRadius(sphR), sphereMaterial(assignMat), isMoving(false) 
	{
		Vec3 sphereRad{ Vec3(sphereRadius, sphereRadius, sphereRadius) };
		boundingBox = AABB(initialSphereCenter - sphereRad, initialSphereCenter + sphereRad);

	}

	// Moving sphere.
	WOSphere(PointVec3 sphC, PointVec3 finalC, double sphR, std::shared_ptr<Material> assignMat) : initialSphereCenter(sphC), sphereRadius(sphR), sphereMaterial(assignMat), isMoving(true) 
	{
		Vec3 sphereRad{ Vec3(sphereRadius, sphereRadius, sphereRadius) };
		AABB initBB(initialSphereCenter - sphereRad, initialSphereCenter + sphereRad);
		AABB finalBB(finalC - sphereRad, finalC + sphereRad);

		centerVec = finalC - initialSphereCenter;
	}

	AABB getWOBoundingBox() const override { return boundingBox; }

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
		getSphereUV(outwardNormal, hitRec.hitTexU, hitRec.hitTexV);
		hitRec.hitMaterial = sphereMaterial;

		return true;
	}

	double getPDFVal(const PointVec3& o, const Vec3& v) const override
	{
		HitRecord tempRec;

		if (!this->checkHit(Ray(o, v), Interval(0.001, Uinf), tempRec))
		{
			return 0;
		}

		auto cosineThetaMax = std::sqrt(1 - ((sphereRadius * sphereRadius) / (initialSphereCenter - o).computeMagnitudeSquared()));
		auto solidAngle = 2 * Upi * (1 - cosineThetaMax);

		return 1 / solidAngle;
	}

	Vec3 getRandomDirWithPDF(const PointVec3& o) const override
	{
		Vec3 direction = initialSphereCenter - o;
		auto distSq = direction.computeMagnitudeSquared();
		ONB uvw;
		uvw.buildFromW(direction);
		return uvw.getLocalCoords(randToSphere(sphereRadius, distSq));
	}


private:
	PointVec3 initialSphereCenter;
	Vec3 centerVec;
	double sphereRadius;
	std::shared_ptr<Material> sphereMaterial;
	bool isMoving;
	AABB boundingBox;

	PointVec3 moveSphere(double currTime) const
	{
		return (initialSphereCenter + (currTime * centerVec));
	}

	static void getSphereUV(const PointVec3& currPoint, double& u, double& v)
	{
		double theta = std::acos(-currPoint.getY());
		double phi = std::atan2(-currPoint.getZ(), currPoint.getX()) + Upi;

		u = phi / (2 * Upi);
		v = theta / Upi;
	}

	static Vec3 randToSphere(double radius, double distance_squared) {
		auto r1 = UGenRNGDouble();
		auto r2 = UGenRNGDouble();
		auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

		auto phi = 2 * Upi * r1;
		auto x = cos(phi) * sqrt(1 - z * z);
		auto y = sin(phi) * sqrt(1 - z * z);

		return Vec3(x, y, z);
	}
};