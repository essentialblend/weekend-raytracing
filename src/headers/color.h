#pragma once

#include "vec3.h"

void writeColor(std::ostream& outStream, vec3 pixelColor)
{
	outStream << static_cast<int>(255.999 * pixelColor.getFirstVec3()) << ' ' << static_cast<int>(255.999 * pixelColor.getSecondVec3()) << ' ' << static_cast<int>(255.999 * pixelColor.getThirdVec3()) << '\n';
}