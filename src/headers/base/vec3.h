#pragma once

class Vec3
{
public:
	Vec3() : member{ 0, 0, 0 } {}
	Vec3(double f) : member{ f, f, f } {}
	Vec3(double f, double s, double t) : member{ f, s, t } {}

	// Member getters and operators.
	double getX() const { return member[0]; }
	double getY() const { return member[1]; }
	double getZ() const { return member[2]; }

	Vec3 operator-() const { return Vec3(-member[0], -member[1], -member[2]); }
	double operator[](unsigned short i) const { return member[i]; }
	double& operator[](unsigned short i) { return member[i]; }

	Vec3& operator+=(const Vec3& inpV)
	{
		member[0] += inpV.getX();
		member[1] += inpV.getY();
		member[2] += inpV.getZ();
		return *this;
	}

	Vec3& operator*=(const Vec3& inpV)
	{
		member[0] *= inpV.getX();
		member[1] *= inpV.getY();
		member[2] *= inpV.getZ();
		return *this;
	}

	Vec3& operator/=(const double inpV)
	{
		return *this *= 1 / inpV;
	}

	double computeMagnitude() const
	{
		return std::sqrt(computeMagnitudeSquared());
	}

	double computeMagnitudeSquared() const
	{
		return (member[0] * member[0]) + (member[1] * member[1]) + (member[2] * member[2]);
	}

	static Vec3 genRandomVec3()
	{
		return Vec3(UGenRNGDouble(), UGenRNGDouble(), UGenRNGDouble());
	}

	static Vec3 genRandomVec3(double minV, double maxV)
	{
		return Vec3(UGenRNGDouble(minV, maxV), UGenRNGDouble(minV, maxV), UGenRNGDouble(minV, maxV));
	}

	bool checkNearZero() const
	{
		auto nearInf = 1e-8;
		return((std::fabs(member[0]) < nearInf) && (std::fabs(member[1]) < nearInf) && (std::fabs(member[2]) < nearInf));
	}

private:
	double member[3];
};

// Constants
using PointVec3 = Vec3;
using ColorVec3 = Vec3;

// Vector utilities.

inline std::ostream& operator<<(std::ostream& outStream, const Vec3& inpV)
{
	return outStream << inpV[0] << ' ' << inpV[1] << ' ' << inpV[2];
}

inline Vec3 operator+(const Vec3& f, const Vec3& s)
{
	return Vec3(f.getX() + s.getX(), f.getY() + s.getY(), f.getZ() + s.getZ());
}

inline Vec3 operator-(const Vec3& f, const Vec3& s)
{
	return Vec3(f.getX() - s.getX(), f.getY() - s.getY(), f.getZ() - s.getZ());
}

inline Vec3 operator*(const Vec3& f, const Vec3& s)
{
	return Vec3(f.getX() * s.getX(), f.getY() * s.getY(), f.getZ() * s.getZ());
}

inline Vec3 operator*(const double t, const Vec3& s)
{
	return Vec3(t * s.getX(), t * s.getY(), t * s.getZ());
}

inline Vec3 operator*(const Vec3& f, const double t)
{
	return t * f;
}

inline Vec3 operator/(const Vec3& f, const double t)
{
	return (1 / t) * f;
}

inline double computeDotProduct(const Vec3& f, const Vec3& s)
{
	return f.getX() * s.getX() + f.getY() * s.getY() + f.getZ() * s.getZ();
}

inline Vec3 computeCrossProduct(const Vec3& f, const Vec3& s)
{
	return Vec3(f.getY() * s.getZ() - f.getZ() * s.getY(), f.getZ() * s.getX() - f.getX() * s.getZ(), f.getX() * s.getY() - f.getY() * s.getX());
}

inline Vec3 computeUnitVector(const Vec3& f)
{
	return f / f.computeMagnitude();
}

inline Vec3 genRandVec3UnitSphere()
{
	while (true)
	{
		Vec3 test = Vec3::genRandomVec3(-1, 1);
		if (test.computeMagnitudeSquared() < 1) return test;
	}
}

inline Vec3 genNormalizedRandVec3UnitSphere()
{
	return computeUnitVector(genRandVec3UnitSphere());
}

inline Vec3 genNormalizedRandVec3OnHemisphere(const Vec3& surfaceNormal)
{
	Vec3 randUnitSph = genNormalizedRandVec3UnitSphere();
	if (computeDotProduct(randUnitSph, surfaceNormal) > 0.0f)
	{
		return randUnitSph;
	}
	else
		return -randUnitSph;
}

inline Vec3 computeReflectionDirection(const Vec3& inputVec, const Vec3& hitNormalVec)
{
	return inputVec - (2 * computeDotProduct(inputVec, hitNormalVec) * hitNormalVec);
}

inline Vec3 computeRefractionDirection(const Vec3& inputVec, const Vec3& normalVec, double etaByEtaPrime)
{
	double cosineTheta{ std::fmin(computeDotProduct(-inputVec, normalVec), 1.f) };

	Vec3 rOutPerp{ etaByEtaPrime * (inputVec + (cosineTheta * normalVec)) };
	Vec3 rOutPar{ -std::sqrt(std::fabs(1.f - rOutPerp.computeMagnitudeSquared())) * normalVec };
	return rOutPerp + rOutPar;
}

inline Vec3 genRandVec3UnitDisk()
{
	while (true)
	{
		Vec3 resultVec(UGenRNGDouble(-1, 1), UGenRNGDouble(-1, 1), 0);
		if (resultVec.computeMagnitudeSquared() < 1)
		{
			return resultVec;
		}
	}
}