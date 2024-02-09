#pragma once

#include "vec3.h"

class Ray
{
public:
	Ray() {}
	Ray(const Vec3& originParam, const Vec3& directionParam) : rayOrigin(originParam), rayDirection(directionParam) {}

	Vec3 getRayOrigin() const
	{
		return rayOrigin;
	}
	Vec3 getRayDirection() const
	{
		return rayDirection;
	}
	Vec3 getPointOnRayAt(double scaleFac) const
	{
		return rayOrigin + (scaleFac * rayDirection);
	}

private:
	Vec3 rayOrigin;
	Vec3 rayDirection;
};
