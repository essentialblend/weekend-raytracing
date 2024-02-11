#pragma once

#include "vec3.h"

inline double linearToGamma(double linearComp)
{
	return std::pow(linearComp, 1 / 2.2);
}

void writeColor(std::ostream& outStream, Vec3 pixelColor, int samplesPerPixel)
{
	double r{ pixelColor.getFirstComponent() };
	double g{ pixelColor.getSecondComponent() };
	double b{ pixelColor.getThirdComponent() };

	// Divide color by num samples.
	double pixelColorScale{ 1.f / samplesPerPixel };
	r *= linearToGamma(pixelColorScale);
	g *= linearToGamma(pixelColorScale);
	b *= linearToGamma(pixelColorScale);

	// Write the translated [0, 255] value per component.
	static const Interval pixelIntensity(0.f, 0.999f);
	outStream << static_cast<int>(256 * pixelIntensity.clampToIntervalRange(r)) << ' ' << static_cast<int>(256 * pixelIntensity.clampToIntervalRange(g)) << ' ' << static_cast<int>(256 * pixelIntensity.clampToIntervalRange(b)) << ' ' << '\n';
}

