#pragma once

#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const vec3& originParam, const vec3& directionParam) : rayOrigin(originParam), rayDirection(directionParam) {}

	vec3 getRayOrigin() const
	{
		return rayOrigin;
	}
	vec3 getRayDirection() const
	{
		return rayDirection;
	}
	vec3 getPointOnRayAt(double scaleFac) const
	{
		return rayOrigin + (rayDirection * scaleFac);
	}

private:
	vec3 rayOrigin;
	vec3 rayDirection;
};
