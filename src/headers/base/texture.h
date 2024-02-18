#pragma once

class Texture
{
public:
	virtual ~Texture() = default;

	virtual ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& currPoint) const = 0;
};

class TSolidColor : public Texture
{
public:
	TSolidColor(ColorVec3 color) : colorValue(color) {}

	TSolidColor(double redV, double greenV, double blueV) : TSolidColor(ColorVec3(redV, greenV, blueV)) {}

	ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& currPoint) const override
	{
		return colorValue;
	}

private:
	ColorVec3 colorValue;
};