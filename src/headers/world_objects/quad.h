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

		setQuadBB();
	}

	virtual void setQuadBB()
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

private:
	PointVec3 botLeftQ;
	Vec3 horizontalU, verticalV;
	std::shared_ptr<Material> quadMaterial;
	AABB quadBBox;
	Vec3 planeNormal;
	double D;
	Vec3 wVec; // For simplification.
};