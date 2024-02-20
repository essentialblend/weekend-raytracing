#pragma once

#pragma warning (push, 0)
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../../dep/stb_image.h"


class ImageHelperSTB {
public:
    ImageHelperSTB() : data(nullptr), bytesPerScanline(0) {}

    ImageHelperSTB(const char* imgFileName) : bytesPerScanline(0), data(nullptr) {

        std::string fileName = std::string(imgFileName);
        char* imageDir = getenv("images");

        // Hunt for the image file in some likely locations.
        if (imageDir && load(std::string(imageDir) + "/" + imgFileName)) return;
        if (load(fileName)) return;
        if (load("images/" + fileName)) return;
        if (load("../images/" + fileName)) return;
        if (load("../../images/" + fileName)) return;
        if (load("../../../images/" + fileName)) return;
        if (load("../../../../images/" + fileName)) return;
        if (load("../../../../../images/" + fileName)) return;
        if (load("../../../../../../images/" + fileName)) return;

        std::cerr << "ERROR: Could not load image file '" << imgFileName << "'.\n";
    }

    ~ImageHelperSTB() { STBI_FREE(data); }

    bool load(const std::string filename) {

        auto n = bytesPerPixel;
        data = stbi_load(filename.c_str(), &imgW, &imgH, &n, bytesPerPixel);
        bytesPerScanline = imgW * bytesPerPixel;
        return data != nullptr;
    }

    int width()  const { return (data == nullptr) ? 0 : imgW; }
    int height() const { return (data == nullptr) ? 0 : imgH; }

    const unsigned char* pixelData (int x, int y) const {
        // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
        static unsigned char magenta[] = { 255, 0, 255 };
        if (data == nullptr) return magenta;

        x = clamp(x, 0, imgW);
        y = clamp(y, 0, imgH);

        return data + y * bytesPerScanline + x * bytesPerPixel;
    }

private:
    const int bytesPerPixel = 3;
    unsigned char* data;
    int imgW{ 0 }, imgH{ 0 };
    int bytesPerScanline;

    static int clamp(int x, int low, int high) {
        // [low, high).
        if (x < low) return low;
        if (x < high) return x;
        return high - 1;
    }
};
#pragma warning (pop)