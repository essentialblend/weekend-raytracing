#pragma once

class WorldObjectList : public WorldObject
{
public:
	std::vector<std::shared_ptr<WorldObject>> WOList;

	WorldObjectList() {}

	WorldObjectList(std::shared_ptr<WorldObject> wObj) { addToWorld(wObj); }

	void addToWorld(std::shared_ptr<WorldObject> wObj)
	{
		WOList.push_back(wObj);
		boundingBox = AABB(boundingBox, wObj->getWOBoundingBox());
	}
	void clearWorld() { WOList.clear(); };

	AABB getWOBoundingBox() const override { return boundingBox; }

	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const override
	{
		HitRecord tempRec;
		bool didItHit{ false };
		double closestHitPointYet = validInterval.getIntervalMaxRange();

		for (const std::shared_ptr<WorldObject>& WO : WOList)
		{
			if (WO->checkHit(inputRay, Interval(validInterval.getIntervalMinRange(), closestHitPointYet), tempRec))
			{
				didItHit = true;
				closestHitPointYet = tempRec.hitRoot;
				hitRec = tempRec;
			}
		}

		return didItHit;
	}

	double getPDFVal(const PointVec3& o, const Vec3& v) const override
	{
		if (!WOList.empty())
		{
			auto weight = 1.0 / WOList.size();
			auto sum = 0.0;

			for (const auto& object : WOList)
				sum += weight * object->getPDFVal(o, v);

			return sum;
		}
		return 0;
	}

	Vec3 getRandomDirWithPDF(const PointVec3& o) const override
	{
		if (!WOList.empty())
		{
			auto int_size = static_cast<int>(WOList.size());
			return WOList[UGenRNGInt(0, int_size - 1)]->getRandomDirWithPDF(o);
		}
		return 0;
	}

private:
	AABB boundingBox;
};