#pragma once

class UPerlin
{
public:
	UPerlin()
	{
        randomVec = new Vec3[pointCount];
        for (int i{ 0 }; i < pointCount; ++i)
        {
            randomVec[i] = computeUnitVector(Vec3::genRandomVec3(-1, 1));
        }

        permXArr = perlinGenPermute();
        permYArr = perlinGenPermute();
        permZArr = perlinGenPermute();
	}

    ~UPerlin()
    {
        delete[] randomVec; delete[] permXArr; delete[] permYArr; delete[] permZArr;
    }

    double genNoise(const PointVec3& p) const
    {
        double u = p.getX() - std::floor(p.getX());
        double v = p.getY() - std::floor(p.getY());
        double w = p.getZ() - std::floor(p.getZ());

        int i = static_cast<int>(std::floor(p.getX()));
        int j = static_cast<int>(std::floor(p.getY()));
        int k = static_cast<int>(std::floor(p.getZ()));

        Vec3 c[2][2][2];

        for (int di{ 0 }; di < 2; di++)
        {
            for (int dj{ 0 }; dj < 2; dj++)
            {
                for (int dk{ 0 }; dk < 2; dk++)
                {
                    c[di][dj][dk] = randomVec[permXArr[(i + di) & 255] ^ permYArr[(j + dj) & 255] ^ permZArr[(k + dk) & 255]];
                }
            }
        }

        return genTrilinearInterpolation(c, u, v, w);
    }

    double genTurbulence(const PointVec3& p, int depth = 7) const
    {
        double accum = 0.f;
        PointVec3 tempP = p;
        double weight = 1.f;

        for (int i = 0; i < depth; i++)
        {
            accum += weight * genNoise(tempP);
            weight *= 0.5f;
            tempP *= 2;
        }

        return std::fabs(accum);
    }

private:
    static const int pointCount = 256;
    Vec3* randomVec;
    int* permXArr;
    int* permYArr;
    int* permZArr;

    static int* perlinGenPermute()
    {
        int* p = new int[pointCount];

        for (int i{ 0 }; i < UPerlin::pointCount; i++)
        {
            p[i] = i;
        }
        genPermute(p, pointCount);

        return p;
    }

    static void genPermute(int* intArrToShuffle, int n)
    {
        for (int i{ n - 1 }; i > 0; i--)
        {
            int target{ UGenRNGInt(0, i) };
            int temp{ intArrToShuffle[i] };
            intArrToShuffle[i] = intArrToShuffle[target];
            intArrToShuffle[target] = temp;
        }
    }

    static double genTrilinearInterpolation(Vec3 c[2][2][2], double u, double v, double w)
    {
        double accum = 0.f;
        double uu = u * u * (3 - (2 * u));
        double vv = v * v * (3 - (2 * v));
        double ww = w * w * (3 - (2 * w));

        for (int i{ 0 }; i < 2; i++)
        {
            for (int j{ 0 }; j < 2; j++)
            {
                for (int k{ 0 }; k < 2; k++)
                {
                    Vec3 weightV(u - i, v - j, w - k);
                    accum += ((i * uu) + ((1 - i) * (1 - uu)))
                        * ((j * vv) + ((1 - j) * (1 - vv)))
                        * ((k * ww) + ((1 - k) * (1 - ww)))
                        * computeDotProduct(c[i][j][k], weightV);
                }
            }
        }

        return accum;
    }
};