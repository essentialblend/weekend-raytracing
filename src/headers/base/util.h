#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <numbers>
#include <fstream>
#include <random>
#include <thread>
#include <format>
#include <functional>

const double Uinf = std::numeric_limits<double>::infinity();
const double Upi = std::numbers::pi;
thread_local std::mt19937 rngEng(std::random_device{}());

// Utility functions.

// Geometry
inline double UDegreesToRadians(double degValue)
{
    return degValue * (Upi / 180.f);
}

inline double URadiansToDegrees(double radValue)
{
    return radValue * (180.f / Upi);
}

// RNG
inline double UGenRNGDouble()
{
    // Use high res clock as a seed.
    /*auto rngSeed{ std::chrono::high_resolution_clock::now().time_since_epoch().count() };
    std::mt19937 rngEng(static_cast<unsigned int>(rngSeed));*/

    // Define the dist. range [0.f, 1.f).
    std::uniform_real_distribution<double> uniDist(0.f, 1.f);

    return uniDist(rngEng);
}

inline double UGenRNGDouble(double minVal, double maxVal)
{
    // Use high res clock as a seed.
   /* auto rngSeed{ std::chrono::high_resolution_clock::now().time_since_epoch().count() };
    std::mt19937 rngEng(static_cast<unsigned int>(rngSeed));*/

    // Define the dist. range [0.f, 1.f).
    std::uniform_real_distribution<double> uniDist(minVal, maxVal);

    return uniDist(rngEng);
}

inline int UGenRNGInt(int minVal, int maxVal)
{
    return static_cast<int>(UGenRNGDouble(minVal, maxVal + 1));
}

inline void UWriteToClog(const std::string& outString)
{
    std::clog << outString << "\n";
}

inline void UWriteToCout(const std::string& outString)
{
    std::cout << outString << "\n";
}

inline void UPrintSuccessLog(
    const std::chrono::time_point<std::chrono::steady_clock>& start,
    const std::chrono::time_point<std::chrono::steady_clock>& end,
    int totalPixels,
    int jitterAA,
    bool useMT, int maxBounces, int resW, int resH)
{
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto logTotalTimeSeconds = duration.count() / 1000.0; // Convert to seconds as a floating-point.
    auto logHours = static_cast<int>(logTotalTimeSeconds / 3600);
    auto logMinutes = static_cast<int>((logTotalTimeSeconds / 60)) % 60;


    std::string timeDetails = std::format("Time taken: {}h {}m {:.2f}s...", logHours, logMinutes, logTotalTimeSeconds - logHours * 3600 - logMinutes * 60);
    std::string graphicsDetails = std::format("Jittered aliasing with {} spp and {} max. ray bounces...", jitterAA, maxBounces);
    std::string pixelProcessDetails;
    if (logTotalTimeSeconds > 0) {
        double pixelsPerSecond = totalPixels / logTotalTimeSeconds;
        pixelProcessDetails = std::format("Processing speed: {:.2f} pixels per second...", pixelsPerSecond);
    }
    else
    {
        pixelProcessDetails = "Processing speed: Calculation not possible due to short duration...";
    }
    std::string renderingModeDetails = std::format("\nRendering mode: {}", useMT ? "Multi-threaded..." : "Single-threaded...");
    

    UWriteToClog(renderingModeDetails);
    UWriteToClog(timeDetails);
    UWriteToClog(std::format("Image resolution: {} x {}...", resW, resH));
    UWriteToClog(graphicsDetails);
    UWriteToClog(pixelProcessDetails);
}

// Includes.
#include "interval.h"
#include "vec3.h"
#include "ray.h"
#include "worldObject.h"
#include "worldObjectList.h"
#include "../world_objects/sphere.h"
#include "../base/texture.h"
#include "../textures/checkers.h"
#include "material.h"
#include "../materials/lambertian.h"
#include "../materials/metal.h"
#include "../materials/dielectric.h"
#include "camera.h"
#include "../base/aabb.h"
#include "../base/bvh_node.h"
#include "image_helper_stb.h"
#include "../textures/image.h"


// CONSTS
constexpr bool USE_MT{ true };
constexpr int AA_NUM_SAMPLES{ 250 };
constexpr int MAX_RAY_BOUNCES{ 250 };
constexpr double VERTICAL_FOV{ 20 };
constexpr int RES_WIDTH_PIXELS{ 1920 };
constexpr double CAM_DEFOCUS_ANGLE{ 0 };
constexpr double CAM_FOCUS_DIST{ 12 };

const Vec3 CAM_LOOKFROM_VEC(0, 0, 12);
const Vec3 CAM_LOOKAT_VEC(0, 0, 0);
const Vec3 WORLD_UP(0, 1, 0);

static void render_RTIOW();
static void render_earth_RTTNW();