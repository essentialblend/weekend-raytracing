#pragma once
import std;

#include "worldObject.h"

class WorldObjectList : public WorldObject
{
public:
	std::vector<std::shared_ptr<WorldObject>> worldObjects;

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

	bool rayHit(const Ray& currRay, UInterval validRayInterval, HitRecord& hitRec) const override
	{
		HitRecord tempRec;
		bool didItHit{ false };
		double closestYet = validRayInterval.maxValue;

		for (const auto& object : worldObjects)
		{
			if (object->rayHit(currRay, UInterval(validRayInterval.minValue, closestYet), tempRec))
			{
				didItHit = true;
				closestYet = tempRec.rayT;
				hitRec = tempRec;
			}
		}
		return didItHit;
	}
};