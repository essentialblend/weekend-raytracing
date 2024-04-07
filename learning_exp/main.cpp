#include "../src/headers/base/util.h"
#include "main.h"
inline Vec3 gg();

int main()
{
	//for (int i = 0; i < 200; i++) {
	//	auto r1 = UGenRNGDouble();
	//	auto r2 = UGenRNGDouble();
	//	auto x = cos(2 * Upi * r1) * 2 * sqrt(r2 * (1 - r2));
	//	auto y = sin(2 * Upi * r1) * 2 * sqrt(r2 * (1 - r2));
	//	auto z = 1 - r2;
	//	std::cout << "[" << x << ", " << y << ", " << z << "]," << "\n";
	//}

	for (int i{}; i < 50; ++i)
	{
		Vec3 testVec = gg();
		std::cout << "[" << testVec.getX() << ", " << testVec.getY() << ", " << testVec.getZ() << "]," << "\n";
	}



	return 0;
}

inline Vec3 gg()
{
	auto r1 = UGenRNGDouble();
	auto r2 = UGenRNGDouble();

	auto phi = 2 * Upi * r1;
	auto x = cos(phi) * std::sqrt(r2);
	auto y = sin(phi) * std::sqrt(r2);
	auto z = std::sqrt(1 - r2);

	Vec3 cosine(x, y, z);

	return cosine;
}