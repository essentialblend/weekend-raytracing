#pragma once

class TNoise : public Texture {
public:
    TNoise() {}

    TNoise(double scaleF) : noiseScaleFac(scaleF) {}

    ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& p) const override {
        return ColorVec3(1) * 0.5f * (1.f + perlinNoise.genNoise(noiseScaleFac * p));
    }

private:
    UPerlin perlinNoise;
    double noiseScaleFac{ 5 };
};