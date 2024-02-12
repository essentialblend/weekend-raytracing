#pragma once

#include "util.h"
#include "color.h"
#include "worldObject.h"
#include "material.h"

class Camera
{
public:

	Camera(double aspectR, int imageWidth, int numSamples, int rayBounces, double vFOV) : aspectRatio(aspectR), imageWidth(imageWidth), samplesPerPixelMSAA(numSamples), maxRayBounces(rayBounces), verticalFOV(vFOV) {}

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

	void setVerticalFOV(double fovVal)
	{
		verticalFOV = fovVal;
	}

	void setCameraParams(const Vec3 lookFrom, const Vec3 lookAt, const Vec3 relativeUp, double defocusA, double focusD)
	{
		lookFromPoint = lookFrom;
		lookAtPoint = lookAt;
		orthoCamUpV = relativeUp;
		defocusAngle = defocusA;
		focusDist = focusD;
	}

private:
	double aspectRatio{ 1.f };
	int imageWidth{ 100 };
	int samplesPerPixelMSAA{ 10 };
	int imageHeight{ 0 };
	int maxRayBounces = 10;

	// Camera
	double verticalFOV{ 90.f };
	Vec3 camCenterPoint;
	Vec3 lookFromPoint{ Vec3(0, 0, -1) };
	Vec3 lookAtPoint{ Vec3(0) };
	Vec3 orthoCamUpV{ Vec3(0, 1, 0) };
	Vec3 camOrthoBasisU{ Vec3(0.f) }, camOrthoBasisV{ Vec3(0.f) }, camOrthoBasisW{ Vec3(0.f) };
	//Variation angle of rays through each pixel.
	double defocusAngle{ 0.f };
	// Dist from camera lookFrom point to plane of perfect focus.
	double focusDist{ 10.f };
	Vec3 defocusDiskHor{ Vec3(0.f) };
	Vec3 defocusDiskVert{ Vec3(0.f) };


	Vec3 firstPixelLocationPoint;
	Vec3 pixelDeltaUX;
	Vec3 pixelDeltaVY;

	void initializeCamera()
	{
		imageHeight = static_cast<int>(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;


		camCenterPoint = lookFromPoint;

		double theta = UDegreesToRadians(verticalFOV);
		double h = std::tan(theta / 2);
		double viewportHeight{ 2.0f * h * focusDist };
		double viewportWidth{ viewportHeight * (static_cast<double>(imageWidth) / imageHeight) };
		
		// Calculate unit basis vectors for our camera coordinate frame.
		camOrthoBasisW = computeUnitVector(lookFromPoint - lookAtPoint);
		camOrthoBasisU = computeUnitVector(computeCrossProduct(orthoCamUpV, camOrthoBasisW));
		camOrthoBasisV = computeCrossProduct(camOrthoBasisW, camOrthoBasisU);


		// Viewport spanning vectors, and deltas.
		Vec3 viewportUX{ viewportWidth * camOrthoBasisU };
		Vec3 viewportVY{ viewportHeight * -camOrthoBasisV };
		pixelDeltaUX = viewportUX / imageWidth;
		pixelDeltaVY = viewportVY / imageHeight;

		// Location of the top left pixel.
		Vec3 viewportUpperLeftNorm = camCenterPoint - (focusDist * camOrthoBasisW) - (viewportUX / 2) - (viewportVY / 2);
		firstPixelLocationPoint = viewportUpperLeftNorm + (0.5f * (pixelDeltaUX + pixelDeltaVY));

		double defocusRad{ focusDist * (std::tan(UDegreesToRadians(defocusAngle / 2))) };
		defocusDiskHor = camOrthoBasisU * defocusRad;
		defocusDiskVert = camOrthoBasisV * defocusRad;
	}

	Ray genRayFromCamToPixelCenter(int horPixel, int vertPixel) const
	{
		Vec3 pixelCenter{firstPixelLocationPoint + (horPixel * pixelDeltaUX) + (vertPixel * pixelDeltaVY) };
		Vec3 pixelSample{ pixelCenter + pixelSampleSquare() };

		Vec3 rayOrigin{ defocusAngle <= 0 ? camCenterPoint : defocusDiskSample() };
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
		if (mainWorld.rayHit(inputRay, Interval(0.001, Uinf), hitRec))
		{
			Ray scatteredRay;
			Vec3 attenuationValue;
			if (hitRec.hitRecMaterial->scatterRay(inputRay, hitRec, attenuationValue, scatteredRay))
			{
				return attenuationValue * computeRayColor(scatteredRay, mainWorld, rayBounces - 1);
			}
			return Vec3(0.f);
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

	Vec3 defocusDiskSample() const
	{
		// Return a random point in the camera defocus disk.
		Vec3 randP{ genRandVecInUnitDisk() };
		return camCenterPoint + (randP[0] * defocusDiskHor) + (randP[1] * defocusDiskVert);
	}

};