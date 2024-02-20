#include "./headers/base/util.h"

int main()
{
	switch (6)
	{
	case 1: render_RTIOW(); break;
	case 2: render_earth_RTTNW(); break;
	case 3: render_perlin_RTTNW(); break;
	case 4: render_prelimQuads_RTTNW(); break;
	case 5: render_emissiveLights_RTTNW(); break;
	case 6: render_cornellBox_RTTNW(); break;
	}

	return 0;
}

static void render_RTIOW()
{
	// Engine init.
	std::vector<ColorVec3> pixelBuffer;

	// World
	WorldObjectList primaryWOL;

	// Ground sphere.
	//std::shared_ptr<MLambertian> groundMat = std::make_shared<MLambertian>(Vec3(0.5, 0.5, 0.5));
	auto texCheckers = std::make_shared<TCheckers>(0.32, ColorVec3(.2, .3, .1), ColorVec3(.9, .9, .9));
	primaryWOL.addToWorld(std::make_shared<WOSphere>(Vec3(0, -1000, 0), 1000, std::make_shared<MLambertian>(texCheckers)));

	// All other world objects.
	for (int a{ -11 }; a < 11; a++)
	{
		for (int b{ -11 }; b < 11; b++)
		{
			double chooseMat{ UGenRNGDouble() };
			Vec3 sphereCenter(a + 0.9 * UGenRNGDouble(), 0.2, b + 0.9 * UGenRNGDouble());

			if ((sphereCenter - Vec3(4, 0.2f, 0)).computeMagnitude() > 0.9f)
			{
				std::shared_ptr<Material> sphereMat;

				// Diffuse
				if (chooseMat < 0.8)
				{
					Vec3 baseColor{ Vec3::genRandomVec3() * Vec3::genRandomVec3() };
					sphereMat = std::make_shared<MLambertian>(baseColor);

					Vec3 finalCenter = sphereCenter + Vec3(0, UGenRNGDouble(0, 0.5f), 0);

					primaryWOL.addToWorld(std::make_shared<WOSphere>(sphereCenter, finalCenter, 0.2f, sphereMat));
				}
				// Metal
				else if (chooseMat < 0.95)
				{
					Vec3 baseColor{ Vec3::genRandomVec3(0.5, 1) };
					double fuzzF{ UGenRNGDouble(0, 0.5) };
					sphereMat = std::make_shared<MMetal>(baseColor, fuzzF);
					primaryWOL.addToWorld(std::make_shared<WOSphere>(sphereCenter, 0.2f, sphereMat));

				}
				else
				{
					sphereMat = std::make_shared<MDielectric>(1.5f);
					primaryWOL.addToWorld(std::make_shared<WOSphere>(sphereCenter, 0.2f, sphereMat));
				}
			}
		}
	}

	std::shared_ptr<Material> mat1 = std::make_shared<MDielectric>(1.5f);
	primaryWOL.addToWorld(std::make_shared<WOSphere>(Vec3(-2, 1, 0), 1.f, mat1));

	std::shared_ptr<Material> mat2 = std::make_shared<MLambertian>(Vec3(0.4f, 0.2f, 0.1f));
	primaryWOL.addToWorld(std::make_shared<WOSphere>(Vec3(4, 1, 0), 1.f, mat2));

	std::shared_ptr<Material> mat3 = std::make_shared<MMetal>(Vec3(0.7f, 0.6f, 0.5f), 0.f);
	primaryWOL.addToWorld(std::make_shared<WOSphere>(Vec3(7, 1, 0), 1.f, mat3));

	primaryWOL = WorldObjectList(std::make_shared<WOBVHNode>(primaryWOL));

	// Camera init.
	Vec3 lookF(-2, 2, 1);
	Vec3 lookAt(0, 0, -1);
	Vec3 camVUP(0, 1, 0);
	ColorVec3 sceneBG(0.7, 0.8, 1.0);

	Camera mainCamera((16.0 / 9.0), RES_WIDTH_PIXELS, pixelBuffer, USE_MT, AA_NUM_SAMPLES, MAX_RAY_BOUNCES, VERTICAL_FOV, CAM_LOOKFROM_VEC, CAM_LOOKAT_VEC, WORLD_UP, CAM_DEFOCUS_ANGLE, CAM_FOCUS_DIST, sceneBG);

	mainCamera.renderFrame(primaryWOL);
}

static void render_earth_RTTNW()
{
	// Engine init.
	std::vector<ColorVec3> pixelBuffer;


	// World
	WorldObjectList primaryWOL;

	std::shared_ptr<TImage> earthTex = std::make_shared<TImage>("earthmap.jpg");
	std::shared_ptr<MLambertian> earthSurface = std::make_shared<MLambertian>(earthTex);
	std::shared_ptr<WOSphere> globeEarth = std::make_shared<WOSphere>(PointVec3(0, 0, 0), 2, earthSurface);

	ColorVec3 sceneBG(0.7, 0.8, 1.0);

	// Camera init.
	Camera mainCamera((16.0 / 9.0), RES_WIDTH_PIXELS, pixelBuffer, USE_MT, AA_NUM_SAMPLES, MAX_RAY_BOUNCES, VERTICAL_FOV, CAM_LOOKFROM_VEC, CAM_LOOKAT_VEC, WORLD_UP, CAM_DEFOCUS_ANGLE, CAM_FOCUS_DIST, sceneBG);

	mainCamera.renderFrame(WorldObjectList(globeEarth));
}

static void render_perlin_RTTNW()
{
	// Engine init.
	std::vector<ColorVec3> pixelBuffer;

	// World
	WorldObjectList primaryWOL;

	std::shared_ptr<TNoise> perlinNoiseTex = std::make_shared<TNoise>(4);

	primaryWOL.addToWorld(std::make_shared<WOSphere>(PointVec3(0, -1000, 0), 1000, std::make_shared<MLambertian>(perlinNoiseTex)));
	primaryWOL.addToWorld(std::make_shared<WOSphere>(PointVec3(0, 2, 0), 2, std::make_shared<MLambertian>(perlinNoiseTex)));


	ColorVec3 sceneBG(0.7, 0.8, 1.0);

	// Camera init.
	Camera mainCamera((16.0 / 9.0), RES_WIDTH_PIXELS, pixelBuffer, USE_MT, AA_NUM_SAMPLES, MAX_RAY_BOUNCES, VERTICAL_FOV, CAM_LOOKFROM_VEC, CAM_LOOKAT_VEC, WORLD_UP, CAM_DEFOCUS_ANGLE, CAM_FOCUS_DIST, sceneBG);

	mainCamera.renderFrame(primaryWOL);
}

static void render_prelimQuads_RTTNW()
{
	// Engine init.
	std::vector<ColorVec3> pixelBuffer;

	// World
	WorldObjectList primaryWOL;

	// Materials
	std::shared_ptr<MLambertian> leftRedMat = std::make_shared<MLambertian>(ColorVec3(1.0, 0.2, 0.2));
	std::shared_ptr<MLambertian> backGreenMat = std::make_shared<MLambertian>(ColorVec3(0.2, 1.0, 0.2));
	std::shared_ptr<MLambertian> rightBlueMat = std::make_shared<MLambertian>(ColorVec3(0.2, 0.2, 1.0));
	std::shared_ptr<MLambertian> upperOrangeMat = std::make_shared<MLambertian>(ColorVec3(1.0, 0.5, 0.0));
	std::shared_ptr<MLambertian> lowerTealMat = std::make_shared<MLambertian>(ColorVec3(0.2, 0.8, 0.8));

	// Quads
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), leftRedMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), backGreenMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), rightBlueMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upperOrangeMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lowerTealMat));

	ColorVec3 sceneBG(0.7, 0.8, 1.0);

	// Camera init.
	Camera mainCamera((16.0 / 9.0), RES_WIDTH_PIXELS, pixelBuffer, USE_MT, AA_NUM_SAMPLES, MAX_RAY_BOUNCES, VERTICAL_FOV, CAM_LOOKFROM_VEC, CAM_LOOKAT_VEC, WORLD_UP, CAM_DEFOCUS_ANGLE, CAM_FOCUS_DIST, sceneBG);

	mainCamera.renderFrame(primaryWOL);
}

static void render_emissiveLights_RTTNW()
{
	// Engine init.
	std::vector<ColorVec3> pixelBuffer;
	

	// World
	WorldObjectList primaryWOL;

	std::shared_ptr<TNoise> perlinTexture = std::make_shared<TNoise>(3);

	primaryWOL.addToWorld(std::make_shared<WOSphere>(PointVec3(0, -1000, 0), 1000, std::make_shared<MLambertian>(perlinTexture)));
	primaryWOL.addToWorld(std::make_shared<WOSphere>(PointVec3(0, 2, 0), 2, std::make_shared<MLambertian>(perlinTexture)));

	std::shared_ptr<MEmissive> emissiveLightTex = std::make_shared<MEmissive>(ColorVec3(6, 0, 0));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), emissiveLightTex));
	
	int AA{ 500 };
	int maxDepth{ 500 };
	int resW{ 400 };
	double vFOV{ 20 };
	double camDefocusAngle{ 0 };
	double camFocusDist{ 1 };

	ColorVec3 sceneBG(0);
	Vec3 camLF(26, 3, 6);
	Vec3 camLA(0, 2, 0);
	Vec3 camWUP(0, 1, 0);

	// Camera init.
	Camera mainCamera((16.0 / 9.0), static_cast<unsigned short>(resW), pixelBuffer, USE_MT, AA, maxDepth, vFOV, camLF, camLA, camWUP, camDefocusAngle, camFocusDist, sceneBG);

	mainCamera.renderFrame(primaryWOL);
}

static void render_cornellBox_RTTNW()
{
	// Engine init.
	std::vector<ColorVec3> pixelBuffer;

	// World
	WorldObjectList primaryWOL;

	// Materials
	std::shared_ptr<MLambertian> redWallMat{ std::make_shared<MLambertian>(ColorVec3(0.65f, 0.05f, 0.05f)) };
	std::shared_ptr<MLambertian> whiteWallMat{ std::make_shared<MLambertian>(ColorVec3(0.73f, 0.73f, 0.73f)) };
	std::shared_ptr<MLambertian> greenWallMat{ std::make_shared<MLambertian>(ColorVec3(0.12f, 0.45f, 0.15f)) };
	std::shared_ptr<MEmissive> emissiveLightMat{ std::make_shared<MEmissive>(ColorVec3(15.f, 15.f, 15.f)) };

	// Quads.
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), greenWallMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), redWallMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emissiveLightMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), whiteWallMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), whiteWallMat));
	primaryWOL.addToWorld(std::make_shared<WOQuad>(PointVec3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), whiteWallMat));

	// Boxes
	std::shared_ptr<WorldObject> leftBox = genBoxFromQuads(PointVec3(0, 0, 0), PointVec3(165, 330, 165), whiteWallMat);
	leftBox = std::make_shared<WORotateY>(leftBox, 15);
	leftBox = std::make_shared<WOTranslate>(leftBox, Vec3(265, 0, 295));
	primaryWOL.addToWorld(leftBox);

	std::shared_ptr<WorldObject> rightBox = genBoxFromQuads(PointVec3(0, 0, 0), PointVec3(165), whiteWallMat);
	rightBox = std::make_shared<WORotateY>(rightBox, -18);
	rightBox = std::make_shared<WOTranslate>(rightBox, Vec3(130, 0, 65));
	primaryWOL.addToWorld(rightBox);

	int AA{ 1000 };
	int maxDepth{ 1000 };
	int resW{ 1440 };
	double vFOV{ 40 };
	double camDefocusAngle{ 0 };
	double camFocusDist{ 20 };

	ColorVec3 sceneBG(0);
	Vec3 camLF(278, 278, -800);
	Vec3 camLA(278, 278, 0);
	Vec3 camWUP(0, 1, 0);

	// Camera init.
	Camera mainCamera((1), static_cast<unsigned short>(resW), pixelBuffer, USE_MT, AA, maxDepth, vFOV, camLF, camLA, camWUP, camDefocusAngle, camFocusDist, sceneBG);

	mainCamera.renderFrame(primaryWOL);
}