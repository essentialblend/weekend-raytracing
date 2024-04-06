#pragma once

class Ray
{
public:
	Ray() {}

	Ray(const PointVec3& rayO, const Vec3& rayD, double rayT = 0) : rayOrigin(rayO), rayDirection(rayD), rayTime(rayT) {}

	PointVec3 getRayOrigin() const { return rayOrigin; }

	PointVec3 getRayDirection() const { return rayDirection; }

	PointVec3 getPointOnRayAt(double t) const { return rayOrigin + (t * rayDirection); }

	double getRayTime() const { return rayTime; }

private:
	PointVec3 rayOrigin;
	Vec3 rayDirection;
	double rayTime{ 0 };
};