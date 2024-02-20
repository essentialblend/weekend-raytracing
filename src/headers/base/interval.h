#pragma once

class Interval
{
public:
	Interval() : minBound(-Uinf), maxBound(+Uinf) {}
	Interval(double m, double ma) : minBound(m), maxBound(ma) {}
	Interval(const Interval& first, const Interval& second)
	{
		minBound = std::fmin(first.getIntervalMinRange(), second.getIntervalMinRange());
		maxBound = std::fmax(first.getIntervalMaxRange(), second.getIntervalMaxRange());
	}

	bool isWithinInterval(double inputV) const
	{
		return (minBound <= inputV && inputV <= maxBound);
	}

	bool isStrictlyWithinInterval(double inputV) const
	{
		return (minBound < inputV && inputV < maxBound);
	}

	double getIntervalMinRange() const
	{
		return minBound;
	}

	double getIntervalMaxRange() const
	{
		return maxBound;
	}

	void setIntervalMinRange(const double value)
	{
		minBound = value;
	}

	void setIntervalMaxRange(const double value)
	{
		maxBound = value;
	}

	double getIntervalSize() const { return maxBound - minBound; }

	double clampWithinInterval(double x) const
	{
		if (x < minBound) return minBound;
		if (x > maxBound) return maxBound;
		return x;
	}

	Interval expandIntervalForPadding(double deltaV) const
	{
		return Interval(minBound - (deltaV / 2), maxBound + (deltaV / 2));
	}

private:
	double minBound;
	double maxBound;
};

Interval operator+(const Interval& iv, double dispVal)
{
	return Interval(iv.getIntervalMinRange() + dispVal, iv.getIntervalMaxRange() + dispVal);
}

Interval operator+(double dispVal, const Interval& iv)
{
	return iv + dispVal;
}