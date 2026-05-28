#ifndef CLYDE_3D_H
#define CLYDE_3D_H

#include "clyde.h"

namespace clyde
{

#pragma region Geometry

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Shape
    //-----------------------------------------------------------------------------

    /**
     * @defgroup geometry Geometry 3D
     * @brief A collection of 3D Geometric primitives.
     * @{
     */

    class Cube
    {
    };

    class Prism
    {
    };

    class Sphere
    {
    };

    class Capsule
    {
    };

    class Cylinder
    {
    };

    /** @} */

#pragma endregion Geometry

#pragma region Graphics

    class Line3D : Shape
    {
    };

    class Point3D : Shape
    {
    };

    class Circle3D : Shape
    {
    };

    class Triangle3D : Shape
    {
    };

    class Cube : Shape
    {
    };

    class Sphere : Shape
    {
    };

    class Cylinder : Shape
    {
    };

    class Capsule : Shape
    {
    };

#pragma endregion Graphics

    class Model
    {
    };

    class Mesh
    {
    };

} // namespace clyde

#endif // CLYDE_3D_H