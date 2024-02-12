import std;

#include "./headers/base/util.h"


int main()
{
	// World
	WorldObjectList primaryWOL;

	// Ground sphere.
	std::shared_ptr<MLambertian> groundMat = std::make_shared<MLambertian>(Vec3(0.5, 0.5, 0.5));
	primaryWOL.addWOToList(std::make_shared<WOSphere>(Vec3(0, -1000, 0), 1000, groundMat));

	// All other world objects.
	for (int a{ -11 }; a < 11; a++)
	{
		for (int b{ -11 }; b < 11; b++)
		{
			double chooseMat{ UGenRNGDouble() };
			Vec3 sphereCenter(a + 0.9 * UGenRNGDouble(), 0.2, b + 0.9 * UGenRNGDouble());

			if ((sphereCenter - Vec3(4, 0.2f, 0)).getLength() > 0.9f)
			{
				std::shared_ptr<Material> sphereMat;

				// Diffuse
				if (chooseMat < 0.8)
				{
					Vec3 baseColor{ Vec3::genRNGVec3() * Vec3::genRNGVec3() };
					sphereMat = std::make_shared<MLambertian>(baseColor);
					primaryWOL.addWOToList(std::make_shared<WOSphere>(sphereCenter, 0.2f, sphereMat));
				}
				// Metal
				else if (chooseMat < 0.95)
				{
					Vec3 baseColor{ Vec3::genRNGVec3(0.5, 1) };
					double fuzzF{ UGenRNGDouble(0, 0.5) };
					sphereMat = std::make_shared<MMetal>(baseColor, fuzzF);
					primaryWOL.addWOToList(std::make_shared<WOSphere>(sphereCenter, 0.2f, sphereMat));

				}
				else
				{
					sphereMat = std::make_shared<MDielectric>(1.5f);
					primaryWOL.addWOToList(std::make_shared<WOSphere>(sphereCenter, 0.2f, sphereMat));
				}
			}
		}
	}

	std::shared_ptr<Material> mat1 = std::make_shared<MDielectric>(1.5f);
	primaryWOL.addWOToList(std::make_shared<WOSphere>(Vec3(0, 1, 0), 1.f, mat1));

	std::shared_ptr<Material> mat2 = std::make_shared<MLambertian>(Vec3(0.4f, 0.2f, 0.1f));
	primaryWOL.addWOToList(std::make_shared<WOSphere>(Vec3(-4, 1, 0), 1.f, mat2));

	std::shared_ptr<Material> mat3 = std::make_shared<MMetal>(Vec3(0.7f, 0.6f, 0.5f), 0.f);
	primaryWOL.addWOToList(std::make_shared<WOSphere>(Vec3(4, 1, 0), 1.f, mat3));


	// Render. Parameters: Aspect Ratio, NumRowPixels, SPP for AA, NumRayBounces (Depth), VFOV.
	Camera mainCamera((16.f / 9.f), 600, 150, 75, 20);
	mainCamera.setCameraParams(Vec3(13, 2, 4), Vec3(0, 0, 0), Vec3(0, 1, 0), 0.6f, 10.f);
	mainCamera.renderFrame(primaryWOL);
	
	return 0;
}


