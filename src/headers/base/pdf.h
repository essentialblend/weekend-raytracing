#pragma once

#include "util.h"

class PDF {
public:
    virtual ~PDF() {}

    virtual double getPDFDistrValue(const Vec3& direction) const = 0;
    virtual Vec3 genDirWithPDF() const = 0;
};

// WorldObject PDF
class WOPDF : public PDF {
public:
    WOPDF(const WorldObject& _objects, const PointVec3& _origin)
        : worldObjects(_objects), objOrigin(_origin)
    {}

    double getPDFDistrValue(const Vec3& direction) const override {
        return worldObjects.getPDFVal(objOrigin, direction);
    }

    Vec3 genDirWithPDF() const override {
        return worldObjects.getRandomDirWithPDF(objOrigin);
    }

private:
    const WorldObject& worldObjects;
    PointVec3 objOrigin;
};

// Mixture PDF
class PDFMixture : public PDF
{
public:
    PDFMixture(std::shared_ptr<PDF> firstPDF, std::shared_ptr<PDF> secPDF)
    {
        memPDFs[0] = firstPDF;
        memPDFs[1] = secPDF;
    }

    double getPDFDistrValue(const Vec3& direction) const override {
        return ((0.5 * memPDFs[0]->getPDFDistrValue(direction)) + (0.5 * memPDFs[1]->getPDFDistrValue(direction)));
    }

    Vec3 genDirWithPDF() const override {
        if (UGenRNGDouble() < 0.5)
        {
            return memPDFs[0]->genDirWithPDF();
        }
        else
        {
            return memPDFs[1]->genDirWithPDF();
        }
    }

private:
    std::shared_ptr<PDF> memPDFs[2];
};

// Sphere PDF
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

// Cosine PDF
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


