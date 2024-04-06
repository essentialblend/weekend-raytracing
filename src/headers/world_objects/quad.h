#pragma once

class WOQuad : public WorldObject
{
public:

	WOQuad(const PointVec3& botLeftQ, const Vec3& horizontalU, const Vec3& verticalV, std::shared_ptr<Material> quadMat) : botLeftQ(botLeftQ), horizontalU(horizontalU), verticalV(verticalV), quadMaterial(quadMat)
	{
		Vec3 n = computeCrossProduct(horizontalU, verticalV);
		planeNormal = computeUnitVector(n);
		
		D = computeDotProduct(planeNormal, botLeftQ);

		wVec = n / computeDotProduct(n, n);

		quadArea = n.computeMagnitude();

		setQuadBB();
	}

	void setQuadBB()
	{
		quadBBox = AABB(botLeftQ, botLeftQ + horizontalU + verticalV).padBoundingBox();
	}

	AABB getWOBoundingBox() const override { return quadBBox; }

	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const override
	{
		double denom = computeDotProduct(planeNormal, inputRay.getRayDirection());

		if (std::fabs(denom) < 1e-8) return false;

		double t = (D - computeDotProduct(planeNormal, inputRay.getRayOrigin())) / denom;
		if (!validInterval.isWithinInterval(t)) return false;

		PointVec3 rayPlaneIntersectionPoint = inputRay.getPointOnRayAt(t);
		Vec3 quadQToPoint{ rayPlaneIntersectionPoint - botLeftQ };
		double alphaCoeff = computeDotProduct(wVec, computeCrossProduct(quadQToPoint, verticalV));
		double betaCoeff = computeDotProduct(wVec, computeCrossProduct(horizontalU, quadQToPoint));

		if (!isPointInsideQuad(alphaCoeff, betaCoeff, hitRec)) return false;

		hitRec.hitPoint = rayPlaneIntersectionPoint;
		hitRec.hitRoot = t;
		hitRec.hitMaterial = quadMaterial;
		hitRec.setFaceNormal(inputRay, planeNormal);

		return true;
	}

	virtual bool isPointInsideQuad(double a, double b, HitRecord& hitRec) const
	{
		if ((a < 0) || (1 < a) || (b < 0) || (1 < b)) return false;

		hitRec.hitTexU = a;
		hitRec.hitTexV = b;

		return true;
	}

	double getPDFValue(const PointVec3& o, const Vec3& v) const override
	{
		HitRecord tempRec;

		if (!this->checkHit(Ray(o, v), Interval(0.001, Uinf), tempRec))
		{
			return 0;
		}

		// I don't understand this properly.
		auto distSq = tempRec.hitRoot * tempRec.hitRoot * v.computeMagnitudeSquared();
		auto cosine = std::fabs(computeDotProduct(v, tempRec.hitNormalVec) / v.computeMagnitude());

		return (distSq / (cosine * quadArea));
	}

	Vec3 getRandomX(const PointVec3& o) const override
	{
		// Potential mistake here, as the code uses dummy variable names, I'm not sure.
		auto p = botLeftQ + (UGenRNGDouble() * horizontalU) + (UGenRNGDouble() * verticalV);
		return (p - o);
	}


private:
	PointVec3 botLeftQ;
	Vec3 horizontalU, verticalV;
	std::shared_ptr<Material> quadMaterial;
	AABB quadBBox;
	Vec3 planeNormal;
	double D;
	Vec3 wVec;
	double quadArea{};
};

inline std::shared_ptr<WorldObjectList> genBoxFromQuads(const PointVec3& min, const PointVec3& max, std::shared_ptr<Material> boxMat)
{
	std::shared_ptr<WorldObjectList> boxSides{ std::make_shared<WorldObjectList>() };

	// Construct two opposite coords.
	PointVec3 minV = PointVec3((std::fmin(min.getX(), max.getX())), (std::fmin(min.getY(), max.getY())), (std::fmin(min.getZ(), max.getZ())));
	PointVec3 maxV = PointVec3((std::fmax(min.getX(), max.getX())), (std::fmax(min.getY(), max.getY())), (std::fmax(min.getZ(), max.getZ())));

	Vec3 dX{ Vec3(max.getX() - min.getX(), 0, 0) };
	Vec3 dY{ Vec3(0, max.getY() - min.getY(), 0) };
	Vec3 dZ{ Vec3(0, 0, max.getZ() - min.getZ()) };

	boxSides->addToWorld(std::make_shared<WOQuad>(PointVec3(min.getX(), min.getY(), max.getZ()), dX, dY, boxMat));
	boxSides->addToWorld(std::make_shared<WOQuad>(PointVec3(max.getX(), min.getY(), max.getZ()), -dZ, dY, boxMat));
	boxSides->addToWorld(std::make_shared<WOQuad>(PointVec3(max.getX(), min.getY(), min.getZ()), -dX, dY, boxMat));
	boxSides->addToWorld(std::make_shared<WOQuad>(PointVec3(min.getX(), min.getY(), min.getZ()), dZ, dY, boxMat));
	boxSides->addToWorld(std::make_shared<WOQuad>(PointVec3(min.getX(), max.getY(), max.getZ()), dX, -dZ, boxMat));
	boxSides->addToWorld(std::make_shared<WOQuad>(PointVec3(min.getX(), min.getY(), min.getZ()), dX, dZ, boxMat));

	return boxSides;
}