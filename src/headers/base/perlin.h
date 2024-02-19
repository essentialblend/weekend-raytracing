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
        int i = static_cast<int>(4 * p.getX()) & 255;
        int j = static_cast<int>(4 * p.getY()) & 255;
        int k = static_cast<int>(4 * p.getZ()) & 255;

        return randomFloat[permXArr[i] ^ permYArr[j] ^ permZArr[k]];
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
};