import std;

#include "./headers/color.h"

int main()
{
	const unsigned short imageWidth{ 256 };
	const unsigned short imageHeight{ 256 };

	std::println("P3\n{} {} \n255\n", imageWidth, imageHeight);

	for (unsigned short j{ 0 }; j < imageHeight; j++)
	{
		std::clog << "Scanlines remaining: " << imageHeight - j << "\n" << std::flush;
		for (unsigned short i{ 0 }; i < imageWidth; i++)
		{
			vec3 pixelColor = vec3((static_cast<double>(i)) / (static_cast<double>(imageWidth - 1)), (static_cast<double>(j)) / (static_cast<double>(imageHeight - 1)), 0);
			writeColor(std::cout, pixelColor);
		}
	}
	std::clog << "----DONE-----\n";
	return 0;
}