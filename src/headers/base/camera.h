#pragma once

#include "util.h"
#include "color.h"
#include "worldObject.h"

class Camera
{
public:

	Camera(double aspectR, int imageWidth, int numSamples, int rayBounces) : aspectRatio(aspectR), imageWidth(imageWidth), samplesPerPixelMSAA(numSamples), maxRayBounces(rayBounces) {}

	void renderFrame(const WorldObject& mainWorld)
	{
		initializeCamera();

		auto renderStartTime{ std::chrono::high_resolution_clock::now() };
		std::println("P3\n{} {} \n255\n", imageWidth, imageHeight);

		// For every scanline. (i = 0, j = 0) represents the top left pixel.
		for (unsigned short j{ 0 }; j < imageHeight; ++j)
		{
			std::clog << "Scanlines remaining: " << imageHeight - j << "\n" << std::flush;
			for (unsigned short i{ 0 }; i < imageWidth; ++i)
			{
				Vec3 pixelColor(0.f);
				// For every sample.
				for (int pixSample{ 0 }; pixSample < samplesPerPixelMSAA; ++pixSample)
				{
					Ray currentRay{ genRayFromCamToPixelCenter(i, j) };
					pixelColor += computeRayColor(currentRay, mainWorld, maxRayBounces);
				}
				writeColor(std::cout, pixelColor, samplesPerPixelMSAA);
			}
		}
		// Log render time.
		auto renderEndTime{ std::chrono::high_resolution_clock::now() };
		auto renderDuration{ renderEndTime - renderStartTime };
		auto renderTimeH{ std::chrono::duration_cast<std::chrono::hours>(renderDuration) };
		auto renderTimeM{ std::chrono::duration_cast<std::chrono::minutes>(renderDuration) };
		auto renderTimeS{ std::chrono::duration_cast<std::chrono::seconds>(renderDuration) };
		std::clog << "Render complete! Time: " << renderTimeH.count() << "h " << renderTimeM.count() << "m " << renderTimeS.count() << "s!\n";
	}

private:
	double aspectRatio{ 1.f };
	int imageWidth{ 100 };
	int samplesPerPixelMSAA{ 10 };
	int imageHeight{ 0 };
	int maxRayBounces = 10;

	Vec3 camCenterPoint;
	Vec3 firstPixelLocationPoint;
	Vec3 pixelDeltaUX;
	Vec3 pixelDeltaVY;

	void initializeCamera()
	{
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		double focalLength{ 1.0f };
		double viewportHeight{ 2.0f };
		double viewportWidth{ viewportHeight * (static_cast<double>(imageWidth) / imageHeight) };
		Vec3 cameraCenter{ Vec3(0.f) };

		// Viewport spanning vectors, and deltas.
		Vec3 viewportUX{ viewportWidth, 0, 0 };
		Vec3 viewportVY{ 0, -viewportHeight, 0 };
		pixelDeltaUX = viewportUX / imageWidth;
		pixelDeltaVY = viewportVY / imageHeight;

		// Location of the top left pixel.
		Vec3 viewportUpperLeftNorm = cameraCenter - Vec3(0, 0, focalLength) - (viewportUX / 2) - (viewportVY / 2);
		firstPixelLocationPoint = viewportUpperLeftNorm + (0.5f * (pixelDeltaUX + pixelDeltaVY));
	}

	Ray genRayFromCamToPixelCenter(int horPixel, int vertPixel) const
	{
		Vec3 pixelCenter{firstPixelLocationPoint + (horPixel * pixelDeltaUX) + (vertPixel * pixelDeltaVY) };
		Vec3 pixelSample{ pixelCenter + pixelSampleSquare() };

		Vec3 rayOrigin{ camCenterPoint };
		Vec3 rayDirection{ pixelSample - rayOrigin };

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 computeRayColor(const Ray& inputRay, const WorldObject& mainWorld, int rayBounces)
	{
		HitRecord hitRec;

		if (rayBounces <= 0)
		{
			return Vec3(0.f);
		}

		// If we hit an object, compute its color.
		if (mainWorld.rayHit(inputRay, Interval(0.00001, Uinf), hitRec))
		{
			Vec3 reflectedDiffuseRayDir { genRandRayOnHemisphere(hitRec.hitNormalVec) + computeUnitVector(genRandVecInUnitSphere())};
			return (0.1f * computeRayColor(Ray(hitRec.hitPoint, reflectedDiffuseRayDir), mainWorld, rayBounces - 1));
		}
		// Else, go with the background color scheme.
		Vec3 unitDirectionNorm{ computeUnitVector(inputRay.getRayDirection()) };
		double lerpFactor{ 0.5 * (unitDirectionNorm.getSecondComponent() + 1.f) };
		return (((1.f - lerpFactor) * (Vec3(1.f))) + (lerpFactor * Vec3(0.5f, 0.7f, 1.f)));
	}

	Vec3 pixelSampleSquare() const
	{
		// Generate a random value between [0, 1) and normalize to [0, 0.5).
		double pX{ -0.5f + UGenRNGDouble() };
		double pY{ -0.5f + UGenRNGDouble() };
		return (pX * pixelDeltaUX) + (pY * pixelDeltaVY);
	}

};