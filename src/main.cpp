import std;

#include "./headers/base/util.h"
#include "./headers/materials/lambertian.h"
#include "./headers/materials/metal.h"

int main()
{
	// World
	WorldObjectList primaryWorldObjList;

	std::shared_ptr<MLambertian> groundMaterial = std::make_shared<MLambertian>(Vec3(0.8f, 0.8f, 0.f));
	std::shared_ptr<MLambertian> middleSphereMaterial = std::make_shared<MLambertian>(Vec3(0.7f, 0.3f, 0.3f));
	std::shared_ptr<MMetal> leftSphereMaterial = std::make_shared<MMetal>(Vec3(0.8f, 0.8f, 0.8f));
	std::shared_ptr<MMetal> rightSphereMaterial = std::make_shared<MMetal>(Vec3(0.8f, 0.6f, 0.2f));

	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(0.f, -100.5f, -1.f), 100.f, groundMaterial));
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(0.f, 0.f, -1.f), 0.5f, middleSphereMaterial));
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(-1.f, 0.f, -1.f), 0.5f, leftSphereMaterial));
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(1.f, 0.f, -1.f), 0.5f, rightSphereMaterial));




	// Render. Parameters: Aspect Ratio, NumRowPixels, SPP for AA, NumRayBounces (Depth).
	Camera mainCamera((16.f / 9.f), 400, 512, 100);
	mainCamera.renderFrame(primaryWorldObjList);
	
	return 0;
}


