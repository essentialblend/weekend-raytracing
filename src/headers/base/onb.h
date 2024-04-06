#pragma once

#include "util.h"

class ONB
{
public:
	ONB() {};

	Vec3 operator[](int i) const { return axis[i]; }
	Vec3& operator[](int i) { return axis[i]; }

	Vec3 getU() const { return axis[0]; }
	Vec3 getV() const { return axis[1]; }
	Vec3 getW() const { return axis[2]; }

	Vec3 getLocalCoords(double a, double b, double c) const
	{
		return (a * getU()) + (b * getV()) + (c * getW());
	}

	Vec3 getLocalCoords(const Vec3& a) const
	{
		return (a.getX() * getU()) + (a.getY() * getV()) + (a.getZ() * getW());
	}

	void buildFromW(const Vec3& w)
	{
		Vec3 unitW{ computeUnitVector(w) };

		Vec3 a = std::fabs(unitW.getX()) > 0.9 ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
		Vec3 v = computeUnitVector(computeCrossProduct(unitW, a));
		Vec3 u = computeCrossProduct(unitW, v);

		axis[0] = u;
		axis[1] = v;
		axis[2] = unitW;
	}

private:
	Vec3 axis[3]{};
};