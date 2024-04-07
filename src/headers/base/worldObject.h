#pragma once

#include "aabb.h"

class Material;

class HitRecord
{
public:
	PointVec3 hitPoint;
	Vec3 hitNormalVec;
	double hitRoot{ 0 };
	double hitTexU{ 0 };
	double hitTexV{ 0 };
	bool frontFace{ false };
	std::shared_ptr<Material> hitMaterial;

	void setFaceNormal(const Ray& inputRay, const Vec3& outwardNormal)
	{
		frontFace = computeDotProduct(inputRay.getRayDirection(), outwardNormal) < 0;
		hitNormalVec = frontFace ? outwardNormal : -outwardNormal;
	}
};

class WorldObject
{
public:
	virtual ~WorldObject() = default;
	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const = 0;
	virtual AABB getWOBoundingBox() const = 0;


	virtual double getPDFVal(const PointVec3& o, const Vec3& v) const {
		return 0.0;
	}

	virtual Vec3 getRandomDirWithPDF(const PointVec3& o) const {
		return Vec3(1, 0, 0);
	}
};