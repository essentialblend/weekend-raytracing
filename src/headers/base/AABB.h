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

	const Interval& getAxisInterval(int numVal) const
	{
		if (numVal == 1) return yInterval;
		if (numVal == 2) return zInterval;
		return xInterval;
	}

	bool checkHit(const Ray& inputRay, Interval rayInterval) const
	{
		for (int a{ 0 }; a < 3; a++) 
		{
			double t0 = std::fmin((getAxisInterval(a).getIntervalMinRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]), getAxisInterval(a).getIntervalMaxRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]);
			double t1 = std::fmax((getAxisInterval(a).getIntervalMinRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]), getAxisInterval(a).getIntervalMaxRange() - inputRay.getRayOrigin()[a]) / (inputRay.getRayDirection()[a]);

			rayInterval.setIntervalMinRange(std::fmax(t0, rayInterval.getIntervalMinRange()));
			rayInterval.setIntervalMaxRange(std::fmin(t1, rayInterval.getIntervalMaxRange()));

			if (rayInterval.getIntervalMaxRange() <= rayInterval.getIntervalMinRange())
			{
				return false;
			}
			
			return true;
		}
	}
};