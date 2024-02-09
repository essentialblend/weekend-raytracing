#pragma once

#include "vec3.h"

void writeColor(std::ostream& outStream, Vec3 pixelColor, int samplesPerPixel)
{
	double r = pixelColor.getFirstComponent();
	double g = pixelColor.getSecondComponent();
	double b = pixelColor.getThirdComponent();

	// Divide color by num samples.
	double pixelColorScale = 1.f / samplesPerPixel;
	r *= pixelColorScale;
	g *= pixelColorScale;
	b *= pixelColorScale;

	// Write the translated [0, 255] value per component.
	static const UInterval pixelIntensity(0.f, 0.999f);
	outStream << static_cast<int>(256 * pixelIntensity.clampToIntervalRange(r)) << ' ' << static_cast<int>(256 * pixelIntensity.clampToIntervalRange(g)) << ' ' << static_cast<int>(256 * pixelIntensity.clampToIntervalRange(b)) << ' ' << '\n';
}