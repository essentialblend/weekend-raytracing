#pragma once

import std;

class vec3 {
public:
	double baseVec3[3];

	vec3() : baseVec3{ 0, 0, 0 } {};
	vec3(double first) : baseVec3{ first, first, first } {};
	vec3(double first, double second, double third) : baseVec3{ first, second, third } {};

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

	vec3 operator-() const 
	{
		return vec3(-baseVec3[0], -baseVec3[1], -baseVec3[2]);
	}
	double operator[](const int i) const
	{
		return baseVec3[i];
	}
	double& operator[](const int i)
	{
		return baseVec3[i];
	}

	vec3& operator+= (const vec3& inputVec)
	{
		baseVec3[0] += inputVec[0];
		baseVec3[1] += inputVec[1];
		baseVec3[2] += inputVec[2];
		return *this;
	}
	vec3& operator*= (const double scaleFactor)
	{
		baseVec3[0] *= scaleFactor;
		baseVec3[1] *= scaleFactor;
		baseVec3[2] *= scaleFactor;
		return *this;
	}

	const vec3& operator/= (const double scaleFactor)
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

};

inline std::ostream& operator<<(std::ostream& outStream, const vec3& inputVec)
{
	return outStream << inputVec.baseVec3[0] << ' ' << inputVec.baseVec3[1] << inputVec.baseVec3[2];
}

inline vec3 operator+(const vec3& firstVec, const vec3& secondVec)
{
	return vec3(firstVec.baseVec3[0] + secondVec.baseVec3[0], firstVec.baseVec3[1] + secondVec.baseVec3[1], firstVec.baseVec3[2] + secondVec.baseVec3[2]);
}

inline vec3 operator-(const vec3& firstVec, const vec3& secondVec)
{
	return vec3(firstVec.baseVec3[0] - secondVec.baseVec3[0], firstVec.baseVec3[1] - secondVec.baseVec3[1], firstVec.baseVec3[2] - secondVec.baseVec3[2]);
}

inline vec3 operator*(const vec3& firstVec, const vec3& secondVec)
{
	return vec3(firstVec.baseVec3[0] * secondVec.baseVec3[0], firstVec.baseVec3[1] * secondVec.baseVec3[1], firstVec.baseVec3[2] * secondVec.baseVec3[2]);
}

inline vec3 operator*(double scaleFac, const vec3& firstVec)
{
	return vec3(scaleFac * firstVec.getFirstComponent(), scaleFac * firstVec.getSecondComponent(), scaleFac * firstVec.getThirdComponent());
}

inline vec3 operator*(const vec3& firstVec, double scaleFac)
{
	return scaleFac * firstVec;
}


inline vec3 operator/(vec3 firstVec, double scaleFac)
{
	return (1 / scaleFac) * firstVec;
}

inline double computeDotProduct(const vec3& firstVec, const vec3& secondVec)
{
	return (firstVec[0] * secondVec[0]) + (firstVec[1] * secondVec[1]) + (firstVec[2] * secondVec[2]);
};

inline vec3 computeCrossProduct(const vec3& firstVec, const vec3& secondVec)
{
	return vec3((firstVec.baseVec3[1] * secondVec.baseVec3[2] - firstVec.baseVec3[2] * secondVec.baseVec3[1]), (firstVec.baseVec3[2] * secondVec.baseVec3[0] - firstVec.baseVec3[0] * secondVec.baseVec3[2]), (firstVec.baseVec3[0] * secondVec.baseVec3[1] - firstVec.baseVec3[1] * secondVec.baseVec3[0]));
}

inline vec3 computeUnitVector(const vec3& inputVec)
{
	return inputVec / inputVec.getLength();
}







