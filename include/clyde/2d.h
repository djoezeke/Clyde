#ifndef CLYDE_2D_H
#define CLYDE_2D_H

#include "clyde.h"

namespace clyde
{

#pragma region Geometry

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Primitives
    //-----------------------------------------------------------------------------

    /**
     * @defgroup geometry Geometry 2D Primitives
     * @brief A collection of 2D Geometric primitives.
     * @{
     */

    template <typename T>
    class Point : Vec2<T>
    {
    };

    template <typename T>
    class Line
    {
        Vec2<T> start;
        Vec2<T> end;
    };

    template <typename T>
    class Ray
    {
        Vec2<T> origin;
        Vec2<T> direction;
    };

    template <typename T>
    class Rect
    {
        Vec2<T> pos;
        Vec2<T> size;
    };

    template <typename T>
    class Circle
    {
        Vec2<T> pos;
        T radius = T(0);
    };

    template <typename T>
    class Triangle
    {
        std::array<Vec2<T>, 3> pos;
    };

    template <typename T>
    struct Polygon
    {
        std::vector<Vec2<T>> pos;
    };

    /** @} */

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Functions
    //-----------------------------------------------------------------------------

    /**
     *
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |A           B>|    POINT     |     LINE     |     RAY      |    RECT      |   TRIANGLE   |    CIRCLE    |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |              |              |              |              |              |              |              |
     *   |  POINT       |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |              |              |              |              |              |              |              |
     *   |  LINE        |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |              |              |              |              |              |              |              |
     *   |  RAY         |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |              |              |              |              |              |              |              |
     *   |  RECT        |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |              |              |              |              |              |              |              |
     *   |  TRIANGLE    |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *   |              |              |              |              |              |              |              |
     *   |  CIRCLE      |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   |              |              |              |              |              |              |              |
     *   +--------------+--------------+--------------+--------------+--------------+--------------+--------------+
     *
     */

    /**
     * @defgroup geometry Geometry 2D Functions
     * @brief Functions to work with of 2D Geometric primitives.
     * @{
     */

    /** @} */

    #pragma endregion Geometry

#pragma region Graphics

    class Line2D : Shape
    {
    };

    class Point2D : Shape
    {
    };

    class Circle2D : Shape
    {
    };

    class Triangle2D : Shape
    {
    };

    class Rectangle2D : Shape
    {
    };

#pragma endregion Graphics


    class Sprite
    {
    };

} // namespace clyde

#endif // CLDE_2D_H