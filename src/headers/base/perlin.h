#pragma once

class UPerlin
{
public:
	UPerlin()
	{
        randomFloat = new double[pointCount];
        for (int i{ 0 }; i < pointCount; ++i)
        {
            randomFloat[i] = UGenRNGDouble();
        }

        permXArr = perlinGenPermute();
        permYArr = perlinGenPermute();
        permZArr = perlinGenPermute();
	}

    ~UPerlin()
    {
        delete[] randomFloat; delete[] permXArr; delete[] permYArr; delete[] permZArr;
    }

    double genNoise(const PointVec3& p) const
    {
        double u = p.getX() - std::floor(p.getX());
        double v = p.getY() - std::floor(p.getY());
        double w = p.getZ() - std::floor(p.getZ());

        // Hermitian Smoothing.
        u = u * u * (3 - (2 * u));
        v = v * v * (3 - (2 * v));
        w = w * w * (3 - (2 * w));

        int i = static_cast<int>(std::floor(p.getX()));
        int j = static_cast<int>(std::floor(p.getY()));
        int k = static_cast<int>(std::floor(p.getZ()));

        double c[2][2][2];

        for (int di{ 0 }; di < 2; di++)
        {
            for (int dj{ 0 }; dj < 2; dj++)
            {
                for (int dk{ 0 }; dk < 2; dk++)
                {
                    c[di][dj][dk] = randomFloat[permXArr[(i + di) & 255] ^ permYArr[(j + dj) & 255] ^ permZArr[(k + dk) & 255]];
                }
            }
        }

        return genTrilinearInterpolation(c, u, v, w);
    }

private:
    static const int pointCount = 256;
    double* randomFloat;
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

    static double genTrilinearInterpolation(double c[2][2][2], double u, double v, double w)
    {
        float accum = 0.f;

        for (int i{ 0 }; i < 2; i++)
        {
            for (int j{ 0 }; j < 2; j++)
            {
                for (int k{ 0 }; k < 2; k++)
                {
                    accum += (((i * u) + (1 - i) * (1 - u)) * ((j * v) + (1 - j) * (1 - v)) * ((k * w) + (1 - k) * (1 - w)) * c[i][j][k]);
                }
            }
        }

        return accum;
    }
};