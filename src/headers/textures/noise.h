#pragma once

class TNoise : public Texture {
public:
    TNoise() {}

    TNoise(double scaleF) : noiseScaleFac(scaleF) {}

    ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& p) const override {
        PointVec3 s = noiseScaleFac * p;
        //return ColorVec3(1) * 0.5f * (1.f + perlinNoise.genNoise(s));
        return ColorVec3(1.f) * 0.5f * (1 + std::sin(s.getZ() + 10 * perlinNoise.genTurbulence(s)));
    }

private:
    UPerlin perlinNoise;
    double noiseScaleFac{ 5 };
};