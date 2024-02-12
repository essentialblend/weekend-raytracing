My First C++ Path Tracer
====================================================================================================

A brute force path-traced renderer implemented using the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series by Peter Shirley and co.

![image](https://github.com/essentialblend/weekend-raytracing/assets/73982939/36711d91-64f9-4ef1-b07e-52240abe6800)
*600 x 337, 150 spp, 75 max. bounces.*

Current Features:
- Developed from scratch using C++.
- Super-sampled Anti-aliasing.
- Gamma Correction.
- Positionable camera with defocus blur (DOF).
- Lambertian, Metallic, and Dielectric Materials.

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
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html) (In Progress!)
