#pragma once

#include "ray.h"

class hitRecord
{
public:
	// 
	vec3 hitPoint;
	vec3 pointNormal;
	// Scalar (t) for a + t*b.
	double rayT;
	bool frontFace;

	void setFaceNormal(const ray& currRay, const vec3& outwardNormal)
	{
		// Set the hit record normal vector. outwardNormal is expected to be normalized.

		frontFace = computeDotProduct(currRay.getRayDirection(), outwardNormal) < 0;
		pointNormal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class worldObject
{
public:
	virtual ~worldObject() = default;

	virtual bool rayHit(const ray& currRay, UInterval validRayInterval,  hitRecord& hitRec) const = 0;
};