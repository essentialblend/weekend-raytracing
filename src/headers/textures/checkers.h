#pragma once

class TCheckers : public Texture
{
public:
	TCheckers(double scaleFac, std::shared_ptr<Texture> evenTex, std::shared_ptr<Texture> oddTex) : evenTex(evenTex), oddTex(oddTex), invScale(1 / scaleFac) {}

	TCheckers(double scaleFac, ColorVec3 first, ColorVec3 second)
	{
		invScale = 1.f / scaleFac;
		evenTex = std::make_shared<TSolidColor>(first);
		oddTex = std::make_shared<TSolidColor>(second);
	}

	ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& currPoint) const override
	{
		int xInt = static_cast<int>(std::floor(invScale * currPoint.getX()));
		int yInt = static_cast<int>(std::floor(invScale * currPoint.getY()));
		int zInt = static_cast<int>(std::floor(invScale * currPoint.getZ()));

		bool isEven = (xInt + yInt + zInt) % 2 == 0;

		return isEven ? evenTex->getTexColorAtCoords(u, v, currPoint) : oddTex->getTexColorAtCoords(u, v, currPoint);
	}

private:
	double invScale;
	std::shared_ptr<Texture> evenTex;
	std::shared_ptr<Texture> oddTex;

};