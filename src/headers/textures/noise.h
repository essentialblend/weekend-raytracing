#pragma once

class TNoise : public Texture {
public:
    TNoise() {}

    ColorVec3 getTexColorAtCoords(double u, double v, const PointVec3& p) const override {
        return ColorVec3(1) * perlinNoise.genNoise(p);
    }

private:
    UPerlin perlinNoise;
};