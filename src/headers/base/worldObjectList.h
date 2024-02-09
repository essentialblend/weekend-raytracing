#pragma once
import std;

#include "worldObject.h"

class worldObjectList : public worldObject
{
public:
	std::vector<std::shared_ptr<worldObject>> worldObjects;

	worldObjectList() {}
	worldObjectList(std::shared_ptr<worldObject> worldObjToAdd) { addWOToList(worldObjToAdd); }

	void addWOToList(std::shared_ptr<worldObject> worldObjToAdd)
	{
		worldObjects.push_back(worldObjToAdd);
	}
	void clearWObjList() 
	{
		worldObjects.clear();
	}

	bool rayHit(const ray& currRay, UInterval validRayInterval, hitRecord& hitRec) const override
	{
		hitRecord tempRec;
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