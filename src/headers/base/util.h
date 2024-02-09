#pragma once
import std;

// Constants
const double Uinf = std::numeric_limits<double>::infinity();
const double Upi = std::numbers::pi;

// Utility functions.
inline double UdegreesToRadians(double degValue)
{
	return degValue * (Upi / 180.f);
}

inline double UradiansToDegrees (double radValue)
{
	return radValue * (180.f / Upi);
}

// Common headers.
#include "UInterval.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "../world_objects/WOSphere.h"
#include "worldObjectList.h"