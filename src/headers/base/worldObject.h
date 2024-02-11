#pragma once

#include "ray.h"

class HitRecord
{
public:
	Vec3 hitPoint;
	Vec3 hitNormalVec;
	// Scalar (t) for a + t*b.
	double hitRoot;
	bool frontFace;

	void setFaceNormal(const Ray& currRay, const Vec3& outwardNormal)
	{
		// Set the hit record normal vector. outwardNormal is expected to be normalized.
		frontFace = computeDotProduct(currRay.getRayDirection(), outwardNormal) < 0;
		hitNormalVec = frontFace ? outwardNormal : -outwardNormal;
	}
};

class WorldObject
{
public:
	virtual ~WorldObject() = default;

	virtual bool rayHit(const Ray& currRay, Interval validRayInterval,  HitRecord& hitRec) const = 0;
};