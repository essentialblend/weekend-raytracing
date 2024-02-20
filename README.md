My First C++ Path Tracer
====================================================================================================

A brute force path-traced renderer implemented using the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series by Peter Shirley and co.

![render](https://github.com/essentialblend/weekend-raytracing/assets/73982939/4f645b09-51dd-4a99-aa34-4163398a2e88)
*Part 2- 1440 x 1440, 1000 spp, 1000 max. bounces*

Current Features:
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


![render](https://github.com/essentialblend/weekend-raytracing/assets/73982939/32654630-fedf-4d0e-845f-a3b850ddc585)
*1920 x 1080, 150 spp, 150 max. bounces*

Improvements:
- Basic multi-threading.
- Uniformly distributed per-pixel jittered AA.
- Core implementation writes every pixel. We collect all color data and directly write to the image in one go.

Directory Structure
-------------------
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

Progress
--------------------
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) (Now complete!)
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html) (Now complete!)


Third-party libraries:
---------------------
- [_stb_image_](https://github.com/nothings/stb/blob/master/stb_image.h)
- [_ImageMagick_](https://github.com/imagemagick/imagemagick)
