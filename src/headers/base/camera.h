#pragma once

class Camera
{
public:
	Camera() {};

	Camera(double aspRatio, unsigned short imgW, const std::vector<Vec3>& pixelBuffer, bool useMT, int jitterSamples, int maxDepth, double vFOV, const PointVec3& lookF, const PointVec3& lookAt, const Vec3& camVUP, double defocusAngle, double focusDist, const ColorVec3& sceneBG) : aspectRatio(aspRatio), imageWidthPixels(imgW), pixelBuffer(pixelBuffer), useMT(useMT), jitterSamplesAA(jitterSamples), maxRayBouncesDepth(maxDepth), verticalFOV(vFOV), camLookFromPoint(lookF), camLookAtPoint(lookAt), camVUP(camVUP), defocusAngle(defocusAngle), focusDist(focusDist), sceneBackgroundColor(sceneBG) {}

	const std::vector<Vec3>& getPixelBuffer() const
	{
		return pixelBuffer;
	}

	void renderFrame(const WorldObject& mainWorld, const WorldObject& lights)
	{
		initializeCamera();

		int numAvailableThreads{ static_cast<int>(std::thread::hardware_concurrency()) };
		int rowsPerThread = imageHeightPixels / numAvailableThreads;

		auto logTimeStart = std::chrono::high_resolution_clock::now();

		if (useMT)
		{
			auto coutString = std::format("Starting multi-threaded rendering with {} threads...", numAvailableThreads);
			UWriteToCout(coutString);
			std::vector<std::thread> mainThreads;

			for (unsigned short t = 0; t < numAvailableThreads; ++t)
			{
				int startRow = t * rowsPerThread;
				int endRow = (t + 1) * rowsPerThread;

				// Cover remaining rows with the last thread.
				if (t == numAvailableThreads - 1)
				{
					endRow = imageHeightPixels;
				}

				coutString = std::format("Thread {} processing pixel rows {} to {}...", t, startRow, endRow);
				UWriteToCout(coutString);

				mainThreads.emplace_back(&Camera::renderRowSegments, this, startRow, endRow, std::cref(mainWorld), std::cref(lights));
			}

			// Wait for all threads to finish and log their completion.
			for (unsigned short t = 0; t < mainThreads.size(); ++t)
			{
				mainThreads[t].join();
				coutString = std::format("Thread {} finished...", t);
				UWriteToCout(coutString);
			}
		}
		else
		{
			renderRowSegments(0, imageHeightPixels, mainWorld, lights);
		}

		bool retFlag;
		handlePostRenderImageUtils(retFlag);
		if (retFlag) return;

		auto logTimeEnd = std::chrono::high_resolution_clock::now();

		UPrintSuccessLog(logTimeStart, logTimeEnd, imageWidthPixels * imageHeightPixels, jitterSamplesAA, useMT, maxRayBouncesDepth, imageWidthPixels, imageHeightPixels);

	}



private:
	// Camera and viewport variables.
	double aspectRatio{ 0 };
	unsigned short imageWidthPixels{ 0 };
	unsigned short imageHeightPixels{ 0 };
	PointVec3 cameraCenter{ PointVec3() };
	std::vector<Vec3> pixelBuffer{ 0 };
	bool useMT{ false };
	PointVec3 topLeftPixelLocation{ Vec3() };
	Vec3 viewportDeltaX{ Vec3() };
	Vec3 viewportDeltaY{ Vec3() };
	int jitterSamplesAA{ 0 };
	int jitterSqrt{ 0 };
	int maxRayBouncesDepth{ 0 };
	double verticalFOV{ 0 };
	PointVec3 camLookFromPoint{ PointVec3(0, 0, -1) };
	PointVec3 camLookAtPoint{ PointVec3(0, 0, 0) };
	Vec3 camVUP{ 0, 1, 0 };
	Vec3 camUVec, camVVec, camWVec;
	double defocusAngle{ 0 };
	double focusDist{ 10 };
	Vec3 defocusDiskUX;
	Vec3 defocusDiskVY;
	ColorVec3 sceneBackgroundColor;

	void initializeCamera()
	{
		jitterSqrt = static_cast<int>(std::round(std::sqrt(jitterSamplesAA)));

		imageHeightPixels = static_cast<unsigned short>(imageWidthPixels / aspectRatio);
		imageHeightPixels = imageHeightPixels < 1 ? 1 : imageHeightPixels;
		pixelBuffer.resize(static_cast<unsigned long long>(imageWidthPixels * imageHeightPixels));
		cameraCenter = camLookFromPoint;

		camWVec = computeUnitVector(camLookFromPoint - camLookAtPoint);
		camUVec = computeUnitVector(computeCrossProduct(camVUP, camWVec));
		camVVec = computeCrossProduct(camWVec, camUVec);


		double theta = UDegreesToRadians(verticalFOV);
		double h = std::tan(theta / 2);

		double viewportHeight{ (2.f * h * focusDist) };
		double viewportWidth{ viewportHeight * (static_cast<double>(imageWidthPixels) / imageHeightPixels) };

		Vec3 viewportUX{ viewportWidth * camUVec };
		Vec3 viewportVY{ viewportHeight * -camVVec };

		viewportDeltaX = viewportUX / imageWidthPixels;
		viewportDeltaY = viewportVY / imageHeightPixels;

		PointVec3 viewportUpperLeftPoint = cameraCenter - (focusDist * camWVec) - (viewportUX / 2) - (viewportVY / 2);
		topLeftPixelLocation = viewportUpperLeftPoint + (0.5 * (viewportDeltaX + viewportDeltaY));

		// Defocus basis vectors.
		Vec3 defocusRadius{ focusDist * std::tan(UDegreesToRadians(defocusAngle / 2)) };
		defocusDiskUX = camUVec * defocusRadius;
		defocusDiskVY = camVVec * defocusRadius;
	}

	ColorVec3 computePixelColor(const Ray& inputRay, int bounceDepthParam, const WorldObject& mainWorld, const WorldObject& lights) const
	{
		ScatterRecord tempScatterRec;
		HitRecord tempRec;
		Ray scatteredRay;
		ColorVec3 attenuationValue;
		double pdfDistValue{};

		// If we hit max bounces for the input ray, return no color.
		if (bounceDepthParam <= 0)
		{
			return ColorVec3(0.f);
		}

		// If we don't hit anything, return scene bg color.
		if (!mainWorld.checkHit(inputRay, Interval(0.001, +Uinf), tempRec))
		{
			return sceneBackgroundColor;
		}

		ColorVec3 colorFromEmission = tempRec.hitMaterial->getEmittedLight(inputRay, tempRec, tempRec.hitTexU, tempRec.hitTexV, tempRec.hitPoint);
		
		// If there's no scatter (if we have emissive materials), return emission color.
		if (!tempRec.hitMaterial->handleRayScatter(inputRay, tempRec, tempScatterRec))
		{
			return colorFromEmission;
		}

		if (tempScatterRec.skipPDF)
		{
			return tempScatterRec.attenuationParam * computePixelColor(tempScatterRec.skipPDFRay, bounceDepthParam - 1, mainWorld, lights);
		}

		auto lightPDF = std::make_shared<WOPDF>(lights, tempRec.hitPoint);
		PDFMixture mixedPDF(lightPDF, tempScatterRec.PDFPointer);

		scatteredRay = Ray(tempRec.hitPoint, mixedPDF.genDirWithPDF(), inputRay.getRayTime());
		pdfDistValue = mixedPDF.getPDFDistrValue(scatteredRay.getRayDirection());
		auto scatteringPDF = tempRec.hitMaterial->scatteringPDF(inputRay, tempRec, scatteredRay);

		ColorVec3 sampleColor = computePixelColor(scatteredRay, bounceDepthParam - 1, mainWorld, lights);

		ColorVec3 colorFromScatter = (tempScatterRec.attenuationParam * scatteringPDF * sampleColor) / pdfDistValue;

		return colorFromEmission + colorFromScatter;
	}

	// Render chunks if MT, else treat it as a typical render function for the entire screen for ST.
	void renderRowSegments(int startRow, int endRow, const WorldObject& mainWorld, const WorldObject& lights)
	{
		for (int j = startRow; j < endRow; ++j)
		{
			for (int i = 0; i < imageWidthPixels; ++i)
			{
				ColorVec3 pixelColor(0.f);

				// Accumulate color from jittered samples (AA).
				for (int pixelY{ 0 }; pixelY < jitterSqrt; ++pixelY)
				{
					for (int pixelX{ 0 }; pixelX < jitterSqrt; ++pixelX)
					{
						Ray generatedRay{ generateRay(pixelX, pixelY, i, j) };
						
						pixelColor += computePixelColor(generatedRay, maxRayBouncesDepth, mainWorld, lights);
					}
				}

				if (pixelColor.getX() != pixelColor.getX())
					pixelColor.setX(0);
				if (pixelColor.getY() != pixelColor.getY())
					pixelColor.setY(0);
				if (pixelColor.getZ() != pixelColor.getZ())
					pixelColor.setZ(0);

				pixelColor /= jitterSamplesAA;

				pixelColor = linearToGamma(pixelColor);

				int bufferIndex = j * imageWidthPixels + i;
				pixelBuffer[bufferIndex] = ColorVec3(static_cast<int>(256 * std::clamp(pixelColor.getX(), 0.000, 0.999)), static_cast<int>(256 * std::clamp(pixelColor.getY(), 0.000, 0.999)), static_cast<int>(256 * std::clamp(pixelColor.getZ(), 0.000, 0.999)));
			}
		}
	}

	ColorVec3 linearToGamma(const ColorVec3& linearPixelColor)
	{
		ColorVec3 gammaCorrected(std::sqrt(linearPixelColor.getX()), std::sqrt(linearPixelColor.getY()), std::sqrt(linearPixelColor.getZ()));
		return gammaCorrected;
	}

	void handlePostRenderImageUtils(bool& retFlag)
	{
		retFlag = true;
		// Write to image file.
		std::ofstream outPPM("image.ppm");
		if (!outPPM)
		{
			std::cerr << "Error: Could not open output image." << std::endl;
			return;
		}
		outPPM << "P3\n" << imageWidthPixels << " " << imageHeightPixels << "\n255\n";
		for (const auto& pixel : pixelBuffer)
		{
			outPPM << pixel.getX() << " " << pixel.getY() << " " << pixel.getZ() << "\n";
		}
		outPPM.close();

		// Using ImageMagick CLI to convert to PNG and delete PPM.
		bool failConvertPPM_PNG = std::system("magick convert image.ppm render.png");
		if (failConvertPPM_PNG)
		{
			std::cerr << "Error: PPM to PNG conversion failed!\n";
			return;
		}
		bool failDeletePPM = static_cast<bool>(std::system("del image.ppm"));
		if (failDeletePPM)
		{
			std::cerr << "Error: Conversion to PNG succesful, but PPM deletion failed!\n";
			return;
		}
		retFlag = false;
	}

	PointVec3 defocusDiskSample() const
	{
		Vec3 randV{ genRandVec3UnitDisk() };
		return cameraCenter + (randV[0] * defocusDiskUX) + (randV[1] * defocusDiskVY);
	}

	Ray generateRay(int pixelX, int pixelY, int i, int j) const
	{
		double offsetX{ (pixelX + UGenRNGDouble()) / jitterSqrt };
		double offsetY{ (pixelY + UGenRNGDouble()) / jitterSqrt };

		PointVec3 samplePoint{ topLeftPixelLocation + ((i + offsetX) * viewportDeltaX) + ((j + offsetY) * viewportDeltaY) };
		Vec3 rayOrigin = (defocusAngle <= 0) ? cameraCenter : defocusDiskSample();
		Vec3 rayDirection = samplePoint - rayOrigin;

		// Motion blur.
		double rayTime{ UGenRNGDouble() };

		return Ray(rayOrigin, rayDirection, rayTime);
	}
};




