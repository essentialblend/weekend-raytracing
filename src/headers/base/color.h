#pragma once

inline double linearToGamma(double linearComp)
{
	return std::pow(linearComp, 1 / 2.2);
}

void writeColor(std::ostream& outStream, Vec3 pixelColor, int samplesPerPixel)
{
	double r{ pixelColor.getX() };
	double g{ pixelColor.getY() };
	double b{ pixelColor.getZ() };

	// Divide color by num samples.
	double pixelColorScale{ 1.f / samplesPerPixel };
	r *= pixelColorScale;
	g *= pixelColorScale;
	b *= pixelColorScale;

	//Apply linear to gamma conversion.
	r = linearToGamma(r);
	g = linearToGamma(g);
	b = linearToGamma(b);


	// Write the translated [0, 255] value per component.
	static const Interval pixelIntensity(0.f, 0.999f);
	outStream << static_cast<int>(256 * pixelIntensity.clampWithinInterval(r)) << ' ' << static_cast<int>(256 * pixelIntensity.clampWithinInterval(g)) << ' ' << static_cast<int>(256 * pixelIntensity.clampWithinInterval(b)) << ' ' << '\n';
}

