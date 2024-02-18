#pragma once

class WOBVHNode : public WorldObject
{
public:
	WOBVHNode(const WorldObjectList& woList) : WOBVHNode(woList.WOList, 0, woList.WOList.size()) {}

	WOBVHNode(const std::vector<std::shared_ptr<WorldObject>>& sourceObjects, std::size_t start, std::size_t end)
	{
		std::vector<std::shared_ptr<WorldObject>> wObjs{ sourceObjects };

		int randomAxis{ UGenRNGInt(0, 2) };
		std::function comparator = (randomAxis == 0) ? boxCompareX
						       	 : (randomAxis == 1) ? boxCompareY
			                     : boxCompareZ;

		std::size_t objectSpan = end - start;

		if (objectSpan == 1)
		{
			leftNode = rightNode = wObjs[start];
		}
		else if (objectSpan == 2)
		{
			if (comparator(wObjs[start], wObjs[start + 1]))
			{
				leftNode = wObjs[start];
				rightNode = wObjs[start + 1];
			}
			else
			{
				leftNode = wObjs[start + 1];
				rightNode = wObjs[start];
			}
		}
		else
		{
			std::sort(wObjs.begin() + start, wObjs.begin() + end, comparator);

			std::size_t mid = start + objectSpan / 2;
			leftNode = std::make_shared<WOBVHNode>(wObjs, start, mid);
			rightNode = std::make_shared<WOBVHNode>(wObjs, mid, end);
		}

		boundingBox = AABB(leftNode->getWOBoundingBox(), rightNode->getWOBoundingBox());
	}

	bool checkHit(const Ray& inputRay, Interval rayInterval, HitRecord& hitRec) const override {
		if (!boundingBox.checkHit(inputRay, rayInterval))
			return false;

		bool hitLeft = leftNode->checkHit(inputRay, rayInterval, hitRec);
		bool hitRight = rightNode->checkHit(inputRay, Interval(rayInterval.getIntervalMinRange(), hitLeft ? hitRec.hitRoot : rayInterval.getIntervalMaxRange()), hitRec);

		return hitLeft || hitRight;
	}

	AABB getWOBoundingBox() const override { return boundingBox; }
private:
	std::shared_ptr<WorldObject> leftNode;
	std::shared_ptr<WorldObject> rightNode;
	AABB boundingBox;

	static bool boxCompare(const std::shared_ptr<WorldObject> firstWO, const std::shared_ptr<WorldObject> secondWO, int axisIndex)
	{
		return firstWO->getWOBoundingBox().getAABBCoordAxisInterval(axisIndex).getIntervalMinRange() < secondWO->getWOBoundingBox().getAABBCoordAxisInterval(axisIndex).getIntervalMinRange();
	}

	static bool boxCompareX(const std::shared_ptr<WorldObject> firstWO, const std::shared_ptr<WorldObject> secondWO)
	{
		return boxCompare(firstWO, secondWO, 0);
	}
	
	static bool boxCompareY(const std::shared_ptr<WorldObject> firstWO, const std::shared_ptr<WorldObject> secondWO)
	{
		return boxCompare(firstWO, secondWO, 1);
	}
	
	static bool boxCompareZ(const std::shared_ptr<WorldObject> firstWO, const std::shared_ptr<WorldObject> secondWO)
	{
		return boxCompare(firstWO, secondWO, 2);
	}
};