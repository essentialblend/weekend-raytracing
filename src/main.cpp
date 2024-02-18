#include "./headers/base/util.h"
#include "main.h"

int main()
{
	switch (1)
	{
	case 1: render_RTIOW(); break;
	case 2: render_RTIOW(); break;
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
	Camera mainCamera((16.0 / 9.0), RES_WIDTH_PIXELS, pixelBuffer, USE_MT, AA_NUM_SAMPLES, MAX_RAY_BOUNCES, VERTICAL_FOV, CAM_LOOKFROM_VEC, CAM_LOOKAT_VEC, WORLD_UP, CAM_DEFOCUS_ANGLE, CAM_FOCUS_DIST);

	mainCamera.renderFrame(primaryWOL);
}

