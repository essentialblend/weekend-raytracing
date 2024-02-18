#pragma once

class TImage : public Texture
{
public:
	TImage(const char* fileName) : imageFile(fileName) {}

	ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& currPoint) const override
	{
		// If we don't find tex data, return solid cyan.
		if (imageFile.height() <= 0) return ColorVec3(0, 1, 1);

		// Clamp input texcoords to [0, 1] x [1, 0]
		u = Interval(0, 1).clampWithinInterval(u);
		v = 1.0 - Interval(0, 1).clampWithinInterval(v);

		int i{ static_cast<int>(u * imageFile.width()) };
		int j{ static_cast<int>(v * imageFile.height()) };
		const unsigned char* pixel{ imageFile.pixelData(i, j) };

		double colorScale{ 1.f / 255.f };
		return ColorVec3(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}
private:
	ImageHelperSTB imageFile;
};