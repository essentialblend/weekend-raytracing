import std;

#include "./headers/base/util.h"

int main()
{
	// World
	WorldObjectList primaryWorldObjList;
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(0, 0, -1.f), 0.5f));
	primaryWorldObjList.addWOToList(std::make_shared<WOSphere>(Vec3(0, -100.5, -1.f), 100.f));

	// Render. Parameters: Aspect Ratio, NumRowPixels, SPP for AA, NumRayBounces (Depth).
	Camera mainCamera((16.f / 9.f), 400, 40, 50);
	mainCamera.renderFrame(primaryWorldObjList);
	
	return 0;
}


