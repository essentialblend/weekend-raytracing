#pragma once
import std;

// Common headers.
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "../world_objects/WOSphere.h"
#include "worldObjectList.h"

// Constants
const double inf{ std::numeric_limits<double>::infinity() };
const double pi{ std::numbers::pi };

// Utility functions.
inline double degreesToRadians(double degValue)
{
	return degValue * (pi / 180.f);
}

inline double radiansToDegrees (double radValue)
{
	return radValue * (180.f / pi);
}

