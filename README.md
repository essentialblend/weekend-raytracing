My First C++ Path Tracer
====================================================================================================

A brute force path-traced renderer implemented using the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series by Peter Shirley and co.

<p align="center">
  <img src="https://github.com/essentialblend/weekend-raytracing/blob/rttroyl-book3/render.png">
</p>
<p align="center">
600 x 600, ~3000 spp / 500 max bounces.
</p>

## Progress
The series is now complete as of 8th April, '24.
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)

## Current Features:
- Developed from scratch using C++. (RTiOW-P1)
- Per pixel multi-sampled anti-aliasing. (RTiOW-P1)
- Gamma Correction. (RTiOW-P1)
- Positionable camera with defocus blur (DOF). (RTiOW-P1)
- Lambertian, Metallic, and Dielectric Materials. (RTiOW-P1)
  
- Motion blur. (RTTNW-P2)
- AABB BVH. (RTTNW-P2)
- Solid color, Image, and Noise Textures. (RTTNW-P2)
- Perlin Noise with trilinear interpolation and Hermitian Smoothing. (RTTNW-P2)
- Emissive lights. (RTTNW-P2)
- Primitives: Quads, and spheres. (RTTNW-P2)
- Volumes. (RTTNW-P2)

- Monte Carlo Path Tracing. (RTTROYL-P3)
- Importance Sampling. (RTTROYL-P3)
- Generating directions with inverse CDFs and orthonormal bases. (RTTROYL-P3)
- Mixed PDFs. (RTTROYL-P3)

![render](https://github.com/essentialblend/weekend-raytracing/assets/73982939/32654630-fedf-4d0e-845f-a3b850ddc585)
*Part 1- 1920 x 1080, 150 spp, 150 max. bounces* 

## Improvements:
- Basic multi-threading.
- Uniformly distributed per-pixel jittered AA.
- Core implementation writes every pixel. We collect all color data and directly write to the image in one go.

## Branches:
- `rtiow-book1`: Code from Ray Tracing in One Weekend.
- `rttnw-book2`: Code from Ray Tracing The Next Week.
- `rttroyl-book3`: Code from Ray Tracing The Rest of Your Life.

## Directory Structure
  - `src/` --
    Main source folder.

  - `src/headers` --
    Contains all in-use headers.

  - `src/headers/base` --
    Contains all base and utility classes.

  - `src/headers/materials` --
    Contains all derived material classes.

  - `src/headers/world_objects` --
    Contains all derived world-object classes.
