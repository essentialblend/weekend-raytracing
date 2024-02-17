#pragma once

class AABB
{
public:
	Interval xInterval, yInterval, zInterval;

	AABB() {}

	AABB(const Interval& intX, const Interval& intY, const Interval& intZ)
		: xInterval(intX), yInterval(intY), zInterval(intZ) {}

	AABB(const PointVec3& valA, const PointVec3& valB)
	{
		xInterval = Interval(std::fmin(valA[0], valB[0]), std::fmax(valA[0], valB[0]));
		yInterval = Interval(std::fmin(valA[1], valB[1]), std::fmax(valA[1], valB[1]));
		zInterval = Interval(std::fmin(valA[2], valB[2]), std::fmax(valA[2], valB[2]));
	}

	AABB(const AABB& first, const AABB& second)
	{
		xInterval = Interval(first.xInterval, second.xInterval);
		yInterval = Interval(first.yInterval, second.yInterval);
		zInterval = Interval(first.zInterval, second.zInterval);
	}

	const Interval& getAABBCoordAxisInterval(int numVal) const
	{
		if (numVal == 1) return yInterval;
		if (numVal == 2) return zInterval;
		return xInterval;
	}

	bool checkHit(const Ray& inputRay, Interval rayInterval) const
	{
		for (int a{ 0 }; a < 3; a++)
		{
			// Original computation to check for AABB hit.
			/*double t0 = std::fmin((getAABBCoordAxisInterval(a).getIntervalMinRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]), getAABBCoordAxisInterval(a).getIntervalMaxRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]);
			double t1 = std::fmax((getAABBCoordAxisInterval(a).getIntervalMinRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]), getAABBCoordAxisInterval(a).getIntervalMaxRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]);

			rayInterval.setIntervalMinRange(std::fmax(t0, rayInterval.getIntervalMinRange()));
			rayInterval.setIntervalMaxRange(std::fmin(t1, rayInterval.getIntervalMaxRange()));

			if (rayInterval.getIntervalMaxRange() <= rayInterval.getIntervalMinRange())
			{
				return false;
			}

			return true;*/

			// Andrew Kensler's optimized approach.
			double inverseDir{ 1 / inputRay.getRayDirection()[a] };
			double coordRayOrigin{ inputRay.getRayOrigin()[a] };

			double t0{ (getAABBCoordAxisInterval(a).getIntervalMinRange() - coordRayOrigin) * inverseDir };
			double t1{ (getAABBCoordAxisInterval(a).getIntervalMaxRange() - coordRayOrigin) * inverseDir };

			if (inverseDir < 0)
			{
				std::swap(t0, t1);
			}

			if (t0 > rayInterval.getIntervalMinRange())
			{
				rayInterval.setIntervalMinRange(t0);
			}
			if (t1 < rayInterval.getIntervalMaxRange())
			{
				rayInterval.setIntervalMaxRange(t1);
			}
			if (rayInterval.getIntervalMaxRange() < rayInterval.getIntervalMinRange())
			{
				return false;
			}

			return true;
		}
	}
};