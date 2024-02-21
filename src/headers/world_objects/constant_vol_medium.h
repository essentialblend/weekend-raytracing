#pragma once

class WOConstantVolMedium : public WorldObject
{
public:

	WOConstantVolMedium(std::shared_ptr<WorldObject> wObj, double d, std::shared_ptr<Texture> tex) : volBoundary(wObj), negInvDensity(-1 / d), phaseFunction(std::make_shared<MIsotropic>(tex)) {}

	WOConstantVolMedium(std::shared_ptr<WorldObject> wObj, double d, ColorVec3 colorVec) : volBoundary(wObj), negInvDensity(- 1 / d), phaseFunction(std::make_shared<MIsotropic>(colorVec)) {}


	virtual bool checkHit(const Ray& inputRay, Interval validInterval, HitRecord& hitRec) const override
	{
		const bool enableDebug{ false };
		const bool debuggingStatus = enableDebug && UGenRNGDouble() < 0.00001;

		HitRecord firstHit, secondHit;

        // First, we check whether the ray pervades the initial boundary.
        if (!volBoundary->checkHit(inputRay, Interval::universalInterval, firstHit))
            return false;

        if (!volBoundary->checkHit(inputRay, Interval(firstHit.hitRoot + 0.0001, Uinf), secondHit))
            return false;


        if (debuggingStatus) std::clog << "\nray_tmin=" << firstHit.hitRoot << ", ray_tmax=" << secondHit.hitRoot << '\n';


        if (firstHit.hitRoot < validInterval.getIntervalMinRange()) 
            firstHit.hitRoot = validInterval.getIntervalMinRange();
        if (secondHit.hitRoot > validInterval.getIntervalMaxRange()) 
            secondHit.hitRoot = validInterval.getIntervalMaxRange();

        if (firstHit.hitRoot >= secondHit.hitRoot)
            return false;

        if (firstHit.hitRoot < 0)
            firstHit.hitRoot = 0;

        double rayLength = inputRay.getRayDirection().computeMagnitude();
        auto distInsideBoundary = (secondHit.hitRoot - firstHit.hitRoot) * rayLength;
        auto hitDist = negInvDensity * std::log(UGenRNGDouble());

        if (hitDist > distInsideBoundary)
            return false;

        hitRec.hitRoot = firstHit.hitRoot + hitDist / rayLength;
        hitRec.hitPoint = inputRay.getPointOnRayAt(hitRec.hitRoot);

        if (debuggingStatus) {
            std::clog << "hit_distance = " << hitDist << '\n'
                << "rec.t = " << hitRec.hitRoot << '\n'
                << "rec.p = " << hitRec.hitPoint << '\n';
        }

        hitRec.hitNormalVec = Vec3(1, 0, 0);  // arbitrary
        hitRec.frontFace = true;     // also arbitrary
        hitRec.hitMaterial = phaseFunction;

        return true;

	}

	AABB getWOBoundingBox() const override { return volBoundary->getWOBoundingBox(); }

private:
	std::shared_ptr<WorldObject> volBoundary;
	double negInvDensity;
	std::shared_ptr<Material> phaseFunction;
};
