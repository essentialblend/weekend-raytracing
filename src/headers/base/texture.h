#pragma once

class Texture
{
public:
	virtual ~Texture() = default;

	virtual ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& currPoint) const = 0;
};

