#pragma once

class UInterval
{
public:
	double minValue{ 0 };
	double maxValue{ 0 };

	UInterval() : minValue(+Uinf), maxValue(-Uinf) {}
	UInterval(double minVal, double maxVal) : minValue(minVal), maxValue(maxVal) {}

	bool isWithinBounds(double x) const
	{
		return minValue <= x && x <= maxValue;
	}

	bool isStrictlyWithinBounds(double x) const
	{
		return minValue < x && x < maxValue;
	}

	static const UInterval emptyInterval, universeInterval;
};

const static UInterval emptyInterval(+Uinf, -Uinf);
const static UInterval universeInterval(-Uinf, +Uinf);