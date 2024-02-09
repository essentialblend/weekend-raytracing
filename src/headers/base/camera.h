#pragma once

#include "util.h"
#include "color.h"
#include "worldObject.h"

class Camera
{
public:
	double aspectRatio = 1.f;
	int imageWidth = 100;
	int samplesPerPixelMSAA = 10;

	void render(const WorldObject& mainWorld)
	{
		initializeCamera();

		// Render
		std::println("P3\n{} {} \n255\n", imageWidth, imageHeight);

		for (unsigned short j{ 0 }; j < imageHeight; ++j)
		{
			std::clog << "Scanlines remaining: " << imageHeight - j << "\n" << std::flush;
			for (unsigned short i{ 0 }; i < imageWidth; ++i)
			{
				Vec3 pixelColor(0.f);
				for (int pixSample{ 0 }; pixSample < samplesPerPixelMSAA; ++pixSample)
				{
					Ray currentRay = getRay(i, j);
					pixelColor += getRayColor(currentRay, mainWorld);
				}
				writeColor(std::cout, pixelColor, samplesPerPixelMSAA);
			}
		}
		std::clog << "----DONE-----\n";
	}

private:
	int imageHeight;
	Vec3 camCenterPoint;
	Vec3 firstPixelLocationPoint;
	Vec3 pixelDeltaUX;
	Vec3 pixelDeltaVY;

	void initializeCamera()
	{
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		double focalLength = 1.0f;
		double viewportHeight = 2.0f;
		double viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
		Vec3 cameraCenter = Vec3(0.f);

		// Viewport spanning vectors, and deltas.
		Vec3 viewportUX{ viewportWidth, 0, 0 };
		Vec3 viewportVY{ 0, -viewportHeight, 0 };
		pixelDeltaUX = viewportUX / imageWidth;
		pixelDeltaVY = viewportVY / imageHeight;

		// Location of the top left pixel.
		Vec3 viewportUpperLeftNorm = cameraCenter - Vec3(0, 0, focalLength) - (viewportUX / 2) - (viewportVY / 2);
		firstPixelLocationPoint = viewportUpperLeftNorm + (0.5f * (pixelDeltaUX + pixelDeltaVY));
	}

	Ray getRay(int i, int j) const
	{
		Vec3 pixelCenter{ firstPixelLocationPoint + (i * pixelDeltaUX) + (j * pixelDeltaVY) };
		Vec3 pixelSample = pixelCenter + pixelSampleSquare();

		Vec3 rayOrigin = camCenterPoint;
		Vec3 rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 getRayColor(const Ray& inputRay, const WorldObject& mainWorld)
	{
		HitRecord hitRec;

		if (mainWorld.rayHit(inputRay, UInterval(0, Uinf), hitRec))
		{
			return (0.5f * (hitRec.pointNormal + Vec3(1.f)));
		}

		Vec3 unitDirectionNorm = computeUnitVector(inputRay.getRayDirection());
		double lerpFactor = 0.5 * (unitDirectionNorm.getSecondComponent() + 1.f);

		return (((1.f - lerpFactor) * (Vec3(1.f))) + (lerpFactor * Vec3(0.5f, 0.7f, 1.f)));

	}

	Vec3 pixelSampleSquare() const
	{
		double pixX = -0.5f + UGenRNGDouble();
		double pixY = -0.5f + UGenRNGDouble();
		return (pixX * pixelDeltaUX) + (pixY * pixelDeltaVY);
	}

};