import std;

#include "./headers/base/util.h"

int main()
{
	// World
	WorldObjectList primaryWorldObjList;
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(0, 0, -1.f), 0.5f));
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(0, -100.5, -1.f), 100.f));

	// Camera
	Camera mainCamera;
	mainCamera.aspectRatio = 16.f / 9.f;
	mainCamera.imageWidth = 400;
	mainCamera.samplesPerPixelMSAA = 100;

	// Render
	mainCamera.render(primaryWorldObjList);
	
	return 0;
}


