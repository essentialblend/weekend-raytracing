#pragma once
import std;

// Constants
const double Uinf = std::numeric_limits<double>::infinity();
const double Upi = std::numbers::pi;

// Utility functions.

// Geometry
inline double UDegreesToRadians(double degValue)
{
	return degValue * (Upi / 180.f);
}

inline double URadiansToDegrees (double radValue)
{
	return radValue * (180.f / Upi);
}

// RNG
inline double UGenRNGDouble()
{
	// Use high res clock as a seed.
	auto rngSeed{ std::chrono::high_resolution_clock::now().time_since_epoch().count() };
	std::mt19937 rngEng(static_cast<unsigned int>(rngSeed));

	// Define the dist. range [0.f, 1.f).
	std::uniform_real_distribution<double> uniDist(0.f, 1.f);

	return uniDist(rngEng);
}

inline double UGenRNGDouble(double minVal, double maxVal)
{
	// Use high res clock as a seed.
	auto rngSeed{ std::chrono::high_resolution_clock::now().time_since_epoch().count() };
	std::mt19937 rngEng(static_cast<unsigned int>(rngSeed));

	// Define the dist. range [0.f, 1.f).
	std::uniform_real_distribution<double> uniDist(minVal, maxVal);

	return uniDist(rngEng);
}


// Common headers.
#include "Interval.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "../world_objects/WOSphere.h"
#include "worldObjectList.h"
#include "camera.h"