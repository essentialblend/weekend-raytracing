import std;

#include "./headers/base/util.h"

static vec3 getRayColor(const ray& inputRay, const worldObject& mainWorld);

int main()
{
	// Image
	const double aspectRatio{ 16.0 / 9.0 };
	const unsigned short imageWidth{ 400 };
	
	unsigned short imageHeight{ static_cast<int>(imageWidth / aspectRatio) };
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// World
	worldObjectList primaryWorldObjList;
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(vec3(0, 0, -1.f), 0.5f));
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(vec3(0, -100.5, -1.f), 100.f));

	// Camera
	double focalLength = 1.0f;
	double viewportHeight = 2.0f;
	double viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
	vec3 cameraCenter = vec3(0.f);

	// Viewport spanning vectors, and deltas.
	vec3 viewportUX{ viewportWidth, 0, 0 };
	vec3 viewportVY{ 0, -viewportHeight, 0 };
	vec3 pixelDeltaUX{ viewportUX / imageWidth };
	vec3 pixelDeltaVY{ viewportVY / imageHeight };

	// Location of the top left pixel.
	vec3 viewportUpperLeftNorm = cameraCenter - vec3(0, 0, focalLength) - (viewportUX / 2) - (viewportVY / 2);
	vec3 topLeftPixelLocation = viewportUpperLeftNorm + (0.5f * (pixelDeltaUX + pixelDeltaVY));


	// Render
	std::println("P3\n{} {} \n255\n", imageWidth, imageHeight);

	for (unsigned short j{ 0 }; j < imageHeight; ++j)
	{
		std::clog << "Scanlines remaining: " << imageHeight - j << "\n" << std::flush;
		for (unsigned short i{ 0 }; i < imageWidth; ++i)
		{
			vec3 pixelCenter = topLeftPixelLocation + (i * pixelDeltaUX) + (j * pixelDeltaVY);
			vec3 rayDirection = pixelCenter - cameraCenter;
			ray cameraRay{ cameraCenter, rayDirection };

			vec3 pixelColor = getRayColor(cameraRay, primaryWorldObjList);
			writeColor(std::cout, pixelColor);
		}
	}
	std::clog << "----DONE-----\n";
	return 0;
}

static vec3 getRayColor(const ray& inputRay, const worldObject& mainWorld)
{
	hitRecord hitRec;

	if (mainWorld.rayHit(inputRay, UInterval(0, Uinf), hitRec))
	{
		return (0.5f * (hitRec.pointNormal + vec3(1.f)));
	}

	vec3 unitDirectionNorm = computeUnitVector(inputRay.getRayDirection());
	double lerpFactor = 0.5 * (unitDirectionNorm.getSecondComponent() + 1.f);

	return (((1.f - lerpFactor) * (vec3(1.f))) + (lerpFactor * vec3(0.5f, 0.7f, 1.f)));

}
