#pragma once

#include "vec3.h"

void writeColor(std::ostream& outStream, vec3 pixelColor)
{
	outStream << static_cast<int>(255.999 * pixelColor.getFirstComponent()) << ' ' << static_cast<int>(255.999 * pixelColor.getSecondComponent()) << ' ' << static_cast<int>(255.999 * pixelColor.getThirdComponent()) << ' ' << '\n';
}