#pragma once

#include "util.h"

class PDF {
public:
    virtual ~PDF() {}

    virtual double getPDFDistrValue(const Vec3& direction) const = 0;
    virtual Vec3 genDirWithPDF() const = 0;
};

class WOPDF : public PDF {
public:
    WOPDF(const WorldObject& _objects, const PointVec3& _origin)
        : worldObject(_objects), objOrigin(_origin)
    {}

    double getPDFDistrValue(const Vec3& direction) const override {
        return worldObject.getPDFValue(objOrigin, direction);
    }

    Vec3 genDirWithPDF() const override {
        return worldObject.getRandomX(objOrigin);
    }

private:
    const WorldObject& worldObject;
    PointVec3 objOrigin;
};

class PDFSphere : public PDF
{
public:
    PDFSphere() {}

    double getPDFDistrValue(const Vec3& direction) const override
    {
        return 1 / (4 * Upi);
    }

    Vec3 genDirWithPDF() const override
    {
        return genRandVec3UnitSphere();
    }
};

class PDFCosine : public PDF
{
public:
    PDFCosine(const Vec3& w) 
    {
        uvw.buildFromW(w);
    }

    double getPDFDistrValue(const Vec3& direction) const override
    {
        auto cosineTheta = computeDotProduct(computeUnitVector(direction), uvw.getW());
        return std::fmax(0, cosineTheta / Upi);
    }

    Vec3 genDirWithPDF() const override
    {
        return uvw.getLocalCoords(genRandomCosineDirRay());
    }
    
private:
    ONB uvw{};
};


