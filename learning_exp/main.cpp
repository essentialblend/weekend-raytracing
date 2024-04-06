#include "../src/headers/base/util.h"
#include "main.h"

int main()
{
    for (int i = 0; i < 200; i++) {
        auto r1 = UGenRNGDouble();
        auto r2 = UGenRNGDouble();
        auto x = cos(2 * Upi * r1) * 2 * sqrt(r2 * (1 - r2));
        auto y = sin(2 * Upi * r1) * 2 * sqrt(r2 * (1 - r2));
        auto z = 1 - 2 * r2;
        std::cout << x << " " << y << " " << z << '\n';
    }

	return 0;
}