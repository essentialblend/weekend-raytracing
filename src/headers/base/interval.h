#pragma once

class Interval
{
public:
	double minValue{ 0 };
	double maxValue{ 0 };

	Interval() : minValue(+Uinf), maxValue(-Uinf) {}
	Interval(double minVal, double maxVal) : minValue(minVal), maxValue(maxVal) {}

	bool isWithinBounds(double x) const
	{
		return minValue <= x && x <= maxValue;
	}

	bool isStrictlyWithinBounds(double x) const
	{
		return minValue < x && x < maxValue;
	}

	double clampToIntervalRange(double valToClamp) const
	{
		if (valToClamp < minValue) return minValue;
		if (valToClamp > maxValue) return maxValue;
		return valToClamp;
	}

	static const Interval emptyInterval, universeInterval;
};

const static Interval emptyInterval(+Uinf, -Uinf);
const static Interval universeInterval(-Uinf, +Uinf);