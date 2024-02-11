#pragma once

import std;

class Vec3 {
public:
	double baseVec3[3];

	Vec3() : baseVec3{ 0, 0, 0 } {};
	Vec3(double first) : baseVec3{ first, first, first } {};
	Vec3(double first, double second, double third) : baseVec3{ first, second, third } {};

	double getFirstComponent() const
	{
		return baseVec3[0];
	}
	double getSecondComponent() const
	{
		return baseVec3[1];
	}
	double getThirdComponent() const
	{
		return baseVec3[2];
	}

	Vec3 operator-() const 
	{
		return Vec3(-baseVec3[0], -baseVec3[1], -baseVec3[2]);
	}
	double operator[](const int i) const
	{
		return baseVec3[i];
	}
	double& operator[](const int i)
	{
		return baseVec3[i];
	}

	Vec3& operator+= (const Vec3& inputVec)
	{
		baseVec3[0] += inputVec[0];
		baseVec3[1] += inputVec[1];
		baseVec3[2] += inputVec[2];
		return *this;
	}
	Vec3& operator*= (const double scaleFactor)
	{
		baseVec3[0] *= scaleFactor;
		baseVec3[1] *= scaleFactor;
		baseVec3[2] *= scaleFactor;
		return *this;
	}

	const Vec3& operator/= (const double scaleFactor)
	{
		return (*this *= (1 / scaleFactor));
	}

	double getLengthSquared() const
	{
		return ((baseVec3[0] * baseVec3[0]) + (baseVec3[1] * baseVec3[1]) + (baseVec3[2] * baseVec3[2]));
	}

	double getLength() const
	{
		return std::sqrt(getLengthSquared());
	}

	static Vec3 genRNGVec3()
	{
		return Vec3(UGenRNGDouble(), UGenRNGDouble(), UGenRNGDouble());
	}

	static Vec3 genRNGVec3(double minRange, double maxRange)
	{
		return Vec3(UGenRNGDouble(minRange, maxRange), UGenRNGDouble(minRange, maxRange), UGenRNGDouble(minRange, maxRange));
	}
};

inline std::ostream& operator<<(std::ostream& outStream, const Vec3& inputVec)
{
	return outStream << inputVec.baseVec3[0] << ' ' << inputVec.baseVec3[1] << inputVec.baseVec3[2];
}

inline Vec3 operator+(const Vec3& firstVec, const Vec3& secondVec)
{
	return Vec3(firstVec.baseVec3[0] + secondVec.baseVec3[0], firstVec.baseVec3[1] + secondVec.baseVec3[1], firstVec.baseVec3[2] + secondVec.baseVec3[2]);
}

inline Vec3 operator-(const Vec3& firstVec, const Vec3& secondVec)
{
	return Vec3(firstVec.baseVec3[0] - secondVec.baseVec3[0], firstVec.baseVec3[1] - secondVec.baseVec3[1], firstVec.baseVec3[2] - secondVec.baseVec3[2]);
}

inline Vec3 operator*(const Vec3& firstVec, const Vec3& secondVec)
{
	return Vec3(firstVec.baseVec3[0] * secondVec.baseVec3[0], firstVec.baseVec3[1] * secondVec.baseVec3[1], firstVec.baseVec3[2] * secondVec.baseVec3[2]);
}

inline Vec3 operator*(double scaleFac, const Vec3& firstVec)
{
	return Vec3(scaleFac * firstVec.getFirstComponent(), scaleFac * firstVec.getSecondComponent(), scaleFac * firstVec.getThirdComponent());
}

inline Vec3 operator*(const Vec3& firstVec, double scaleFac)
{
	return scaleFac * firstVec;
}


inline Vec3 operator/(Vec3 firstVec, double scaleFac)
{
	return (1 / scaleFac) * firstVec;
}

inline double computeDotProduct(const Vec3& firstVec, const Vec3& secondVec)
{
	return (firstVec[0] * secondVec[0]) + (firstVec[1] * secondVec[1]) + (firstVec[2] * secondVec[2]);
};

inline Vec3 computeCrossProduct(const Vec3& firstVec, const Vec3& secondVec)
{
	return Vec3((firstVec.baseVec3[1] * secondVec.baseVec3[2] - firstVec.baseVec3[2] * secondVec.baseVec3[1]), (firstVec.baseVec3[2] * secondVec.baseVec3[0] - firstVec.baseVec3[0] * secondVec.baseVec3[2]), (firstVec.baseVec3[0] * secondVec.baseVec3[1] - firstVec.baseVec3[1] * secondVec.baseVec3[0]));
}

inline Vec3 computeUnitVector(const Vec3& inputVec)
{
	return inputVec / inputVec.getLength();
}

inline Vec3 genRandVecInUnitSphere()
{
	while (true)
	{
		Vec3 randVec{ Vec3::genRNGVec3(-1.f, 1.f) };
		if (randVec.getLengthSquared() < 1) 
		{ 
			return randVec;
		}
	}
}

inline Vec3 genRandRayOnHemisphere(const Vec3& normalVec)
{
	Vec3 vecOnUnitSphere = computeUnitVector(genRandVecInUnitSphere());
	if (computeDotProduct(vecOnUnitSphere, normalVec) > 0.f)
	{
		return vecOnUnitSphere;
	}
	else
		return -vecOnUnitSphere;
}


