#pragma once
import std;

#include "worldObject.h"

class WorldObjectList : public WorldObject
{
public:


	WorldObjectList() {}
	WorldObjectList(std::shared_ptr<WorldObject> worldObjToAdd) { addWOToList(worldObjToAdd); }

	void addWOToList(std::shared_ptr<WorldObject> worldObjToAdd)
	{
		worldObjects.push_back(worldObjToAdd);
	}
	void clearWObjList() 
	{
		worldObjects.clear();
	}

	bool rayHit(const Ray& currRay, Interval validRayInterval, HitRecord& hitRec) const override
	{
		HitRecord tempRec;
		bool didItHit{ false };
		double closestRootYet = validRayInterval.maxValue;

		for (const auto& object : worldObjects)
		{
			if (object->rayHit(currRay, Interval(validRayInterval.minValue, closestRootYet), tempRec))
			{
				didItHit = true;
				closestRootYet = tempRec.hitRoot;
				hitRec = tempRec;
			}
		}
		return didItHit;
	}
private:
	std::vector<std::shared_ptr<WorldObject>> worldObjects;
};