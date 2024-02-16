#pragma once

class Interval
{
public:
	Interval() : minRange(-Uinf), maxRange(+Uinf) {}
	Interval(double m, double ma) : minRange(m), maxRange(ma) {}

	bool isWithinInterval(double inputV) const
	{
		return (minRange <= inputV && inputV <= maxRange);
	}

	bool isStrictlyWithinInterval(double inputV) const
	{
		return (minRange < inputV && inputV < maxRange);
	}

	double getIntervalMinRange() const
	{
		return minRange;
	}

	double getIntervalMaxRange() const
	{
		return maxRange;
	}

	void setIntervalMinRange(const double value)
	{
		minRange = value;
	}

	void setIntervalMaxRange(const double value)
	{
		maxRange = value;
	}

	double getIntervalSize() const { return maxRange - minRange };

	Interval expandIntervalForPadding(double deltaV) const
	{
		return Interval(minRange - (deltaV / 2), maxRange + (deltaV / 2));
	}

private:
	double minRange;
	double maxRange;
};