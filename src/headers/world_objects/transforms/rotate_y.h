#pragma once

class WORotateY : public WorldObject
{
public:
	WORotateY(std::shared_ptr<WorldObject> wObj, double angleTheta) : objectInstance(wObj)
	{
		double radians = UDegreesToRadians(angleTheta);

		sinTheta = std::sin(radians);
		cosTheta = std::cos(radians);

		rotateBBox = objectInstance->getWOBoundingBox();

		PointVec3 min(Uinf, Uinf, Uinf);
		PointVec3 max(-Uinf, -Uinf, -Uinf);

		// For each point of our bounding box.
		for (int i{ 0 }; i < 2; i++)
		{
			for (int j{ 0 }; j < 2; j++)
			{
				for (int k{ 0 }; k < 2; k++)
				{
					double x = i * rotateBBox.xInterval.getIntervalMaxRange() + (1 - i) * rotateBBox.xInterval.getIntervalMinRange();
					double y = j * rotateBBox.yInterval.getIntervalMaxRange() + (1 - j) * rotateBBox.yInterval.getIntervalMinRange();
					double z = k * rotateBBox.zInterval.getIntervalMaxRange() + (1 - k) * rotateBBox.zInterval.getIntervalMinRange();

					auto newX = (cosTheta * x) + (sinTheta * z);
					auto newZ = (-sinTheta * x) + (cosTheta * z);

					Vec3 tester(newX, y, newZ);

					for (int c{ 0 }; c < 3; c++)
					{
						min[c] = std::fmin(min[c], tester[c]);
						max[c] = std::fmax(max[c], tester[c]);
					}
				}
			}
		}

		rotateBBox = AABB(min, max);
	}

	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const override
	{
		// Change from world to object space.
		PointVec3 rayOrig = inputRay.getRayOrigin();
		Vec3 rayDir = inputRay.getRayDirection();
		// To x and z.
		rayOrig[0] = cosTheta * (inputRay.getRayOrigin())[0] - sinTheta * (inputRay.getRayOrigin()[2]);
		rayOrig[2] = sinTheta * (inputRay.getRayOrigin())[0] + cosTheta * (inputRay.getRayOrigin()[2]);

		rayDir[0] = cosTheta * (inputRay.getRayDirection()[0]) - sinTheta * inputRay.getRayDirection()[2];
		rayDir[2] = sinTheta * (inputRay.getRayDirection()[0]) + cosTheta * inputRay.getRayDirection()[2];

		Ray rotatedRay(rayOrig, rayDir, inputRay.getRayTime());

		// Determine intersection at obj space.
		if (!objectInstance->checkHit(rotatedRay, validInterval, hitRec))
		{
			return false;
		}

		// Change int. point from obj space to world space.
		PointVec3 p = hitRec.hitPoint;
		p[0] = cosTheta * hitRec.hitPoint[0] + sinTheta * hitRec.hitPoint[2];
		p[2] = -sinTheta * hitRec.hitPoint[0] + cosTheta * hitRec.hitPoint[2];

		// Change normals.
		Vec3 normalVec = hitRec.hitNormalVec;
		normalVec[0] = cosTheta * hitRec.hitNormalVec[0] + sinTheta * hitRec.hitNormalVec[2];
		normalVec[2] = -sinTheta * hitRec.hitNormalVec[0] + cosTheta * hitRec.hitNormalVec[2];

		hitRec.hitPoint = p;
		hitRec.hitNormalVec = normalVec;

		return true;
	}

	AABB getWOBoundingBox() const override { return rotateBBox; }

private:
	std::shared_ptr<WorldObject> objectInstance;
	double sinTheta;
	double cosTheta;
	AABB rotateBBox;
};