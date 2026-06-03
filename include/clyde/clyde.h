/**
 * @file clyde.h
 * @brief Clyde Graphics library.
 * @author Sackey Ezekiel Etrue (djoezeke)
 * @version 0.1.0
 * @copyright Copyright (c) 202 Sackey Ezekiel Etrue
 *
 * This file contains the main declarations for the Clyde library,
 * including classes for windows, events, graphics primitives, and more.
 */

#ifndef CLYDE_CLYDE_H

/**
 * SECTIONS: Index of this file
 *
 *  [SECTION] Include Mess
 *  [SECTION] Configurations
 *  [SECTION] Compiler & Platform
 *  [SECTION] API Imports/Exports
 *
 *  Forward: Forward Declarations
 *
 *      [SECTION] Details Forward
 *      [SECTION] Literals Forward
 *
 */

#define CLYDE_CLYDE_H

#include <GLAD/glad.h>
struct GLFWwindow;

// clang-format off

/**
 * @defgroup version version Information
 * @brief Macros for library versioning.
 * @{
 */

/**
 * @def CLYDE_VERSION_MAJOR
 * @brief Major version number of the library.
 * @note If this were version 1.2.3, this value would be 1.
 * @since This macro stream available since 0.1.0 .
 */
#ifndef CLYDE_VERSION_MAJOR
    #define CLYDE_VERSION_MAJOR 0
#endif // CLYDE_VERSION_MAJOR

/**
 * @def CLYDE_VERSION_MINOR
 * @brief Minor version number of the library.
 * @note If this were version 1.2.3, this value would be 2.
 * @since This macro stream available since 0.1.0 .
 */
#ifndef CLYDE_VERSION_MINOR
    #define CLYDE_VERSION_MINOR 1
#endif // CLYDE_VERSION_MINOR

/**
 * @def CLYDE_VERSION_PATCH
 * @brief Patch version number of the library.
 * @note If this were version 1.2.3, this value would be 3.
 * @since This macro stream available since 0.1.0 .
 */
#ifndef CLYDE_VERSION_PATCH
    #define CLYDE_VERSION_PATCH 0
#endif // CLYDE_VERSION_PATCH

/**
 * @def CLYDE_VERSION
 * @brief Library version string in the format @c "X.Y.Z",
 * where @c X stream the major version number, @c Y stream a minor version
 * number, and @c Z stream the patch version number.
 * @sa MyGetVersion
 */
#ifndef CLYDE_VERSION
    #define CLYDE_VERSION "0.1.0"
#endif // CLYDE_VERSION

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Configurations
//-----------------------------------------------------------------------------

/**
 * @defgroup configuration Library Configurations.
 * @brief Preprocessor macros for configuring library functionality.
 * @{
 */

/**
 * @brief Configure file with user config.
 */
#ifdef CLYDE_CONFIG
    #include CLYDE_CONFIG
#endif // CLYDE_CONFIG

#define CLYDE_FONT_TTF
#define CLYDE_FONT_FNT
#define CLYDE_FONT_BDF

#define CLYDE_IMAGE_PNG
#define CLYDE_IMAGE_BMP
#define CLYDE_IMAGE_TGA
#define CLYDE_IMAGE_JPG

// OpenGL Version 1.1
#define CLYDE_OPENGL_11 1

// OpenGL Version 2.1
#define CLYDE_OPENGL_21 2

// OpenGL Version 3.3
#define CLYDE_OPENGL_33 3

// OpenGL Version 4.3
#define CLYDE_OPENGL_43 4

/** @} */

//-----------------------------------------------------------------------------
// [SECTION] Include Mess
//-----------------------------------------------------------------------------

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>
#include <memory>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <queue>
#include <variant>

#pragma region Compiler

/**
 * @defgroup platform Platform Definitions
 * @{
 */

/**
 * @brief   Checks if the compiler stream of given brand.
 * @param   name Platform, like `APPLE`.
 * @retval  true   It is.
 * @retval  false  It isn't.
 */
#define CLYDE_PLATFORM_IS(name) CLYDE_PLATFORM_##name

/**
 * @brief  Returns the current platform name.
 * @return  platform name.
 */
#if defined(__APPLE__)
    /**
     * A preprocessor macro that stream only defined if compiling for MacOS.
     */
    #define CLYDE_PLATFORM_APPLE 1
#endif // CLYDE_PLATFORM_APPLE

#if defined(__linux__)
    /**
     * A preprocessor macro that stream only defined if compiling for Linux.
     */
    #define CLYDE_PLATFORM_LINUX 1
#endif // CLYDE_PLATFORM_LINUX

#if defined(_WIN32)
    /**
     * A preprocessor macro that stream only defined if compiling for Windows.
     */
    #define CLYDE_PLATFORM_WINDOWS 1
#endif // CLYDE_PLATFORM_WINDOWS

/** @} */

#pragma endregion Compiler

#define LOG_TRACE(...) printf(__VA_ARGS__)
#define LOG_INFO(...) printf(__VA_ARGS__)
#define LOG_WARN(...) printf(__VA_ARGS__)
#define LOG_ERROR(...) printf(__VA_ARGS__)
#define LOG_CRITICAL(...) printf(__VA_ARGS__)

#ifdef CLYDE_DEBUG
#endif // CLYDE_DEBUG

#ifndef CLYDE_ASSERT
#define CLYDE_ASSERT(x, ...)                                 \
    {                                                        \
        if (!(x))                                            \
        {                                                    \
            LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                  \
        }                                                    \
    }
#else
    #define CLYDE_ASSERT(x, ...)
#endif // CLYDE_ASSERT

#define BIT(x) (1 << x)

#define CLYDE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// clang-format on

namespace clyde
{

#pragma region System

    //-----------------------------------------------------------------------------
    // [SECTION] System : Time
    //-----------------------------------------------------------------------------

    /**
     * @class Time
     * @brief Represents a time duration with microsecond precision.
     * @details Provides methods to convert time to different units (seconds, milliseconds, microseconds).
     */
    class Time
    {
    public:
        /**
         * @brief Default constructor. Initializes to 0 microseconds.
         */
        constexpr Time() = default;

        /**
         * @brief Get time in seconds.
         * @return Time value in seconds.
         */
        [[nodiscard]] constexpr float asSeconds() const { return static_cast<float>(m_microseconds.count()) / 1e6f; }

        /**
         * @brief Get time in milliseconds.
         * @return Time value in milliseconds.
         */
        [[nodiscard]] constexpr std::int32_t asMilliseconds() const;

        /**
         * @brief Get time in microseconds.
         * @return Time value in microseconds.
         */
        [[nodiscard]] constexpr std::int64_t asMicroseconds() const;

    public:
        std::chrono::microseconds m_microseconds{}; //!< Internal time storage in microseconds
    };

    /**
     * @class Clock
     * @brief Measures elapsed time with microsecond precision.
     * @details Provides functionality to measure elapsed time, and control start/stop/restart operations.
     */
    class Clock
    {
    public:
        /**
         * @brief Default constructor.
         */
        Clock();

        /**
         * @brief Destructor.
         */
        ~Clock();

        /**
         * @brief Get the elapsed time since the clock started.
         * @return The elapsed time.
         */
        [[nodiscard]] Time getElapsedTime() const;

        /**
         * @brief Check if the clock is currently running.
         * @return true if running, false otherwise.
         */
        [[nodiscard]] bool isRunning() const;

        /**
         * @brief Start the clock.
         */
        void start();

        /**
         * @brief Stop the clock.
         */
        void stop();

        /**
         * @brief Restart the clock, returning the elapsed time before restart.
         * @return The elapsed time before restart.
         */
        Time restart();

        /**
         * @brief Reset the clock to zero.
         * @return The elapsed time before reset.
         */
        Time reset();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

    class Timer
    {
    public:
        Timer()
        {
            Reset();
        }

        void Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        float Elapsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }

        float ElapsedMillis()
        {
            return Elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

    class ScopedTimer
    {
    public:
        ScopedTimer(const std::string &name)
            : m_Name(name) {};

        ~ScopedTimer()
        {
            float time = m_Timer.ElapsedMillis();
            std::cout << "[TIMER] " << m_Name << " - " << time << "ms\n";
        }

    private:
        std::string m_Name;
        Timer m_Timer;
    };

#pragma endregion System

#pragma region Geometry

    /**
     * @class Angle
     * @brief Represents an angle value.
     */
    class Angle
    {
    public:
        /**
         * @brief Default constructor. Initializes angle to 0.
         */
        constexpr Angle() = default;

        /**
         * @brief Get the angle as degrees.
         * @return The angle in degrees.
         */
        [[nodiscard]] constexpr float asDegrees() const;

        /**
         * @brief Get the angle as radians.
         * @return The angle in radians.
         */
        [[nodiscard]] constexpr float asRadians() const;

    public:
        float radians{}; //!< Angle value in radians.
    };

    /**
     * @brief Create an Angle from a degree value.
     * @param angle The angle in degrees.
     * @return The corresponding Angle object.
     */
    [[nodiscard]] constexpr Angle degrees(float angle);

    /**
     * @brief Create an Angle from a radian value.
     * @param angle The angle in radians.
     * @return The corresponding Angle object.
     */
    [[nodiscard]] constexpr Angle radians(float angle);

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Vectors
    //-----------------------------------------------------------------------------

    /**
     * @defgroup vector Vector Structures
     * @brief A collection of Geometric Vector structures.
     * @{
     */

    /**
     * @class Vec2
     * @brief A 2D vector template class.
     * @tparam T The type of the vector components (e.g., float, int, double).
     * @details Provides 2D vector operations including normalization, rotation, dot product, and more.
     */
    template <typename T>
    class Vec2
    {
    public:
        /**
         * @brief Default constructor. Initializes vector to (0, 0).
         */
        constexpr Vec2() = default;

        /**
         * @brief Constructor with x and y components.
         * @param x The x-component.
         * @param y The y-component.
         */
        constexpr Vec2(T x, T y) : x(x), y(y) {}

        /**
         * @brief Constructor with radius and angle (polar coordinates).
         * @param r The radius.
         * @param phi The angle in radians.
         */
        Vec2(T r, Angle phi);

        /**
         * @brief Calculate the area of the vector (x * y).
         * @return The area.
         */
        inline constexpr auto area() const;

        /**
         * @brief Get the magnitude squared (length^2).
         * @return The magnitude squared.
         */
        inline constexpr auto mag() const;

        /**
         * @brief Get the normalized vector (unit vector).
         * @return The normalized vector.
         */
        inline constexpr Vec2 norm() const;

        /**
         * @brief Get the perpendicular vector (rotated 90 degrees).
         * @return The perpendicular vector.
         */
        inline constexpr Vec2 perp() const;

        /**
         * @brief Get the ceiling vector (rounded up).
         * @return The ceiling vector.
         */
        inline constexpr Vec2 ceil() const;

        /**
         * @brief Get the component-wise maximum.
         * @param v The vector to compare with.
         * @return The maximum vector.
         */
        inline constexpr Vec2 max(const Vec2 &v) const;

        /**
         * @brief Get the component-wise minimum.
         * @param v The vector to compare with.
         * @return The minimum vector.
         */
        inline constexpr Vec2 min(const Vec2 &v) const;

        /**
         * @brief Calculate dot product (scalar).
         * @param v The vector to dot with.
         * @return The dot product.
         */
        inline constexpr Vec2 dot(const Vec2 &v) const;

        /**
         * @brief Calculate cross product (scalar in 2D).
         * @param v The vector to cross with.
         * @return The cross product.
         */
        inline constexpr Vec2 cross(const Vec2 &v) const;

        /**
         * @brief Convert to Cartesian coordinates.
         * @return The Cartesian coordinates.
         */
        inline constexpr Vec2 cart() const;

        /**
         * @brief Convert to polar coordinates.
         * @return The polar coordinates (r, theta).
         */
        inline constexpr Vec2 polar() const;

        /**
         * @brief Clamp vector components to range [v1, v2].
         * @param v1 The lower bound.
         * @param v2 The upper bound.
         * @return The clamped vector.
         */
        inline constexpr Vec2 clamp(const Vec2 &v1, const Vec2 &v2) const;

        /**
         * @brief Linear interpolation between this vector and another.
         * @param v1 The end vector.
         * @param t The interpolation factor (0.0 to 1.0).
         * @return The interpolated vector.
         */
        inline constexpr Vec2 lerp(const Vec2 &v1, const double t) const;

        /**
         * @brief Reflect this vector.
         * @return The reflected vector.
         */
        inline constexpr Vec2 reflect() const;

        /**
         * @brief Get the length (magnitude) of the vector.
         * @return The length.
         */
        [[nodiscard]] T length() const;

        /**
         * @brief Get the length squared (faster than length).
         * @return The length squared.
         */
        [[nodiscard]] constexpr T lengthSquared() const;

        /**
         * @brief Get the angle to another vector.
         * @param rhs The other vector.
         * @return The angle between vectors in radians.
         */
        [[nodiscard]] Angle angleTo(Vec2 rhs) const;

        /**
         * @brief Get the angle of this vector from x-axis.
         * @return The angle in radians.
         */
        [[nodiscard]] Angle angle() const;

        /**
         * @brief Get the vector rotated by an angle.
         * @param phi The angle to rotate in radians.
         * @return The rotated vector.
         */
        [[nodiscard]] Vec2 rotatedBy(Angle phi) const;

        /**
         * @brief Project this vector onto an axis.
         * @param axis The axis to project onto.
         * @return The projected vector.
         */
        [[nodiscard]] Vec2 projectedOnto(Vec2 axis) const;

    public:
        T x{}; //!< x-axis component
        T y{}; //!< y-axis component
    };

    template <typename T>
    constexpr Vec2<T> &operator==(Vec2<T> &left, Vec2<T> right);

    template <typename T>
    constexpr Vec2<T> &operator!=(Vec2<T> &left, Vec2<T> right);

    template <typename T>
    constexpr Vec2<T> &operator+=(Vec2<T> &left, Vec2<T> right);

    template <typename T>
    constexpr Vec2<T> &operator-=(Vec2<T> &left, Vec2<T> right);

    template <typename T>
    constexpr Vec2<T> &operator+(Vec2<T> &left, Vec2<T> right);

    template <typename T>
    constexpr Vec2<T> &operator-(Vec2<T> &left, Vec2<T> right);

    using Vec2d = Vec2<double>;
    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;

    template <typename T>
    class Vec3
    {
    public:
        constexpr Vec3() = default;
        constexpr Vec3(T x, T y, T z);

        inline constexpr Vec3 norm() const;
        inline constexpr Vec3 dot(const Vec3 &v) const;
        inline constexpr Vec3 cross(const Vec3 &v) const;

        [[nodiscard]] T length() const;
        [[nodiscard]] constexpr T lengthSquared() const;

    public:
        T x{}; // x-axis component
        T y{}; // y-axis component
        T z{}; // z-axis component
    };

    template <typename T>
    constexpr Vec3<T> &operator==(Vec3<T> &left, Vec3<T> right);

    template <typename T>
    constexpr Vec3<T> &operator!=(Vec3<T> &left, Vec3<T> right);

    template <typename T>
    constexpr Vec3<T> &operator+=(Vec3<T> &left, Vec3<T> right);

    template <typename T>
    constexpr Vec3<T> &operator-=(Vec3<T> &left, Vec3<T> right);

    template <typename T>
    constexpr Vec3<T> &operator+(Vec3<T> &left, Vec3<T> right);

    template <typename T>
    constexpr Vec3<T> &operator-(Vec3<T> &left, Vec3<T> right);

    using Vec3d = Vec3<double>;
    using Vec3f = Vec3<float>;
    using Vec3i = Vec3<int>;

    // using Point = Vec2<int>;

    template <typename T>
    class Vec4
    {
    public:
        constexpr Vec4() = default;
        constexpr Vec4(T x, T y, T z, T w);

        inline constexpr Vec4 norm() const;
        inline constexpr Vec4 dot(const Vec4 &v) const;
        inline constexpr Vec4 cross(const Vec4 &v) const;

        [[nodiscard]] T length() const;
        [[nodiscard]] constexpr T lengthSquared() const;

    public:
        T x{}; // x-axis component
        T y{}; // y-axis component
        T z{}; // z-axis component
        T w{}; // z-axis component
    };

    /** @} group vector */

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Matrices
    //-----------------------------------------------------------------------------

    /**
     * @defgroup matrix Matrix Structures
     * @brief A collection of Geometric Matrix structures.
     * @{
     */

    template <std::size_t Columns, std::size_t Rows>
    struct Mat
    {
        float mat[Columns][Rows];
    };

    using Mat2x2 = Mat<2, 2>;
    using Mat3x3 = Mat<3, 3>;
    using Mat4x4 = Mat<4, 4>;

    using Mat2 = Mat2x2;
    using Mat3 = Mat3x3;
    using Mat4 = Mat4x4;

    /** @} group matrix */

#pragma endregion Geometry

#pragma region Graphics

    //-----------------------------------------------------------------------------
    // [SECTION] Graphics :
    //-----------------------------------------------------------------------------

    // Simple color
    class Color
    {
    public:
        std::uint8_t r{}, g{}, b{}, a{255};
        constexpr Color() = default;
        constexpr Color(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a = 255)
            : r(_r), g(_g), b(_b), a(_a) {}
    };

#pragma endregion Graphics

#pragma region Renderer

#pragma region OpenGL10

#if defined(CLYDE_RENDERER_OPENGL10)

#endif // CLYDE_RENDERER_OPENGL10

#pragma endregion OpenGL10

#pragma region OpenGL33

#if defined(CLYDE_RENDERER_OPENGL33)

#endif // CLYDE_RENDERER_OPENGL33

#pragma endregion OpenGL33

#pragma endregion Renderer

#pragma region Platform

    //-----------------------------------------------------------------------------
    // [SECTION] Platform : Input
    //-----------------------------------------------------------------------------

    /**
     * @defgroup input Platform Inputs.
     * @brief Platform Input Classes.
     * @{
     */

    class Mouse
    {
    public:
        enum class Button
        {
            Left,   // left mouse button
            Right,  // right mouse button
            Middle, // middle (wheel) mouse button
        };

        enum class Wheel
        {
            Vertical,  // Vertical mouse wheel
            Horizontal // horizontal mouse wheel
        };

    public:
        [[nodiscard]] bool IsButtonPressed(Button button)
        {
            return s_Instance->IsButtonPressedImpl(button);
        };

        std::pair<float, float> GetPosition()
        {
            return s_Instance->GetPositionImpl();
        };

    protected:
        virtual bool IsButtonPressedImpl(Button button) = 0;
        virtual std::pair<float, float> GetPositionImpl() = 0;

    private:
        static Mouse *s_Instance;
    };

    // Keyboard helper
    class Keyboard
    {
    public:
        enum class Key
        {
            Unknown = -1,
            Space = 32,
            Apostrophe = 39,
            Comma = 44,
            Minus = 45,
            Period = 46,
            Slash = 47,
            D0 = 48,
            D1 = 49,
            D2 = 50,
            D3 = 51,
            D4 = 52,
            D5 = 53,
            D6 = 54,
            D7 = 55,
            D8 = 56,
            D9 = 57,
            Semicolon = 59,
            Equal = 61,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LeftBracket = 91,
            Backslash = 92,
            RightBracket = 93,
            GraveAccent = 96,
            World1 = 161,
            World2 = 162,
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,
            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,
            PageUp = 266,
            PageDown = 267,
            Home = 268,
            End = 269,
            CapsLock = 280,
            ScrollLock = 281,
            NumLock = 282,
            PrintScreen = 283,
            Pause = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,
            Kp0 = 320,
            Kp1 = 321,
            Kp2 = 322,
            Kp3 = 323,
            Kp4 = 324,
            Kp5 = 325,
            Kp6 = 326,
            Kp7 = 327,
            Kp8 = 328,
            Kp9 = 329,
            KpDecimal = 330,
            KpDivide = 331,
            KpMultiply = 332,
            KpSubtract = 333,
            KpAdd = 334,
            KpEnter = 335,
            KpEqual = 336,
            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,
            LeftSuper = 343,
            RightShift = 344,
            RightControl = 345,
            RightAlt = 346,
            RightSuper = 347,
            Menu = 348,
            Count
        };

        static bool IsKeyPressed(Key key)
        {
            return s_Instance->IsKeyPressedImpl(key);
        };

    protected:
        virtual bool IsKeyPressedImpl(Key key) = 0;

    private:
        static Keyboard *s_Instance;
    };

    /** @} group input */

#pragma region Event

#define EVENT(type)                                                    \
    static Type GetStaticType() { return Type::type; };                \
    virtual Type GetType() const override { return GetStaticType(); }; \
    virtual const char *GetName() const override { return #type; };

    //-----------------------------------------------------------------------------
    // [SECTION] Event : Event
    //-----------------------------------------------------------------------------

    class Event
    {
    public:
        enum class Type
        {
            Unknown = -1,

            WindowClose,
            WindowResize,

            AppUpdate,
            AppRender,

            KeyTyped,
            KeyPressed,
            KeyReleased,

            MouseMoved,
            MousePressed,
            MouseReleased,
            MouseScrolled,
        };

    public:
        virtual Type GetType() const = 0;
        virtual const char *GetName() const = 0;

    public:
        bool Handled = false;
    };

    struct WindowClose : public Event
    {
        WindowClose() {};

        EVENT(WindowClose)
    };

    struct WindowResize : public Event
    {
        WindowResize(uint32_t width, uint32_t height)
            : width(width), height(height) {};

        EVENT(WindowResize)

        uint32_t width;
        uint32_t height;
    };

    struct AppUpdate : public Event
    {
        AppUpdate() {};

        EVENT(AppUpdate)
    };

    struct AppRender : public Event
    {
        AppRender() {};

        EVENT(AppRender)
    };

    struct KeyTyped : public Event
    {
        KeyTyped(Keyboard::Key key)
            : key(key), code(static_cast<int>(key)) {};

        KeyTyped(int code)
            : key(static_cast<Keyboard::Key>(code)), code(code) {};

        EVENT(KeyTyped)

        Keyboard::Key key{};
        int code;
    };

    struct KeyPressed : public Event
    {
        KeyPressed(Keyboard::Key key)
            : key(key), code(static_cast<int>(key)) {};

        KeyPressed(int code)
            : key(static_cast<Keyboard::Key>(code)), code(code) {};

        EVENT(KeyPressed)

        Keyboard::Key key{};
        int code;
    };

    struct KeyReleased : public Event
    {
        KeyReleased(Keyboard::Key key)
            : key(key), code(static_cast<int>(key)) {};

        KeyReleased(int code)
            : key(static_cast<Keyboard::Key>(code)), code(code) {};

        EVENT(KeyReleased)

        Keyboard::Key key{};
        int code;
    };

    struct MouseMoved : public Event
    {
        MouseMoved(float x, float y)
            : x(x), y(y) {};

        EVENT(MouseMoved)

        float x, y;
    };

    struct MousePressed : public Event
    {
        MousePressed(int button)
            : button(static_cast<Mouse::Button>(button)), code(button) {};

        MousePressed(Mouse::Button button)
            : button(button), code(static_cast<int>(button)) {};

        EVENT(MousePressed)

        Mouse::Button button{};
        int code;
    };

    struct MouseScrolled : public Event
    {
        MouseScrolled(float xOffset, float yOffset)
            : XOffset(xOffset), YOffset(yOffset) {};

        EVENT(MouseScrolled)

        Mouse::Wheel wheel{};
        float XOffset, YOffset;
    };

    struct MouseReleased : public Event
    {
        MouseReleased(int button)
            : button(static_cast<Mouse::Button>(button)), code(button) {};

        MouseReleased(Mouse::Button button)
            : button(button), code(static_cast<int>(button)) {};

        EVENT(MouseReleased)

        Mouse::Button button{};
        int code;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event &event)
            : m_Event(event) {}

        template <typename T, typename F>
        bool Dispatch(const F &func)
        {
            if (m_Event.GetType() == T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<T &>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event &m_Event;
    };

#pragma endregion Event

    //-----------------------------------------------------------------------------
    // [SECTION] Platform : Window
    //-----------------------------------------------------------------------------

    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProps(const std::string &title = "Clde OpenGL ",
                    uint32_t width = 1280,
                    uint32_t height = 720)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event &)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void *GetNativeWindow() const = 0;

        static Window *Create(const WindowProps &props = WindowProps());

        inline static Window &Get() { return *s_Instance; }

    private:
        static Window *s_Instance;
    };

#pragma region Windows

#if defined(CLYDE_PLATFORM_WINDOWS)

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Window
    //-----------------------------------------------------------------------------

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps &props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline uint32_t GetWidth() const override { return m_Data.Width; }
        inline uint32_t GetHeight() const override { return m_Data.Height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void *GetNativeWindow() const { return m_Window; }

    private:
        virtual void Init(const WindowProps &props);
        virtual void Free();

    private:
        struct GLFWwindow *m_Window;

        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Input
    //-----------------------------------------------------------------------------

    /**
     * @defgroup input Window Inputs.
     * @brief Window Input Classes.
     * @{
     */

    class WindowsMouse : public Mouse
    {
    protected:
        virtual bool IsButtonPressedImpl(Button button) override;
        virtual std::pair<float, float> GetPositionImpl() override;
    };

    class WindowsKeyboard : public Keyboard
    {
    protected:
        virtual bool IsKeyPressedImpl(Key key) override;
    };

    /** @} group input */

#endif // CLYDE_PLATFORM_WINDOWS

#pragma endregion Windows

#pragma region Linux

#if defined(CLYDE_PLATFORM_LINUX)

    //-----------------------------------------------------------------------------
    // [SECTION] Linux : Window
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Linux : Input
    //-----------------------------------------------------------------------------

    /**
     * @defgroup input Linux Inputs.
     * @brief Linux Input Classes.
     * @{
     */

    /** @} group input */

#endif // CLYDE_PLATFORM_LINUX

#pragma endregion Linux

#pragma endregion Platform

#pragma region Application

#ifndef CLYDE_NO_APPLICATION

    //-----------------------------------------------------------------------------
    // [SECTION] Application : Layer
    //-----------------------------------------------------------------------------

    class Layer
    {
    public:
        Layer(const std::string &name = "Layer");

        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate(float ts) {}
        virtual void OnEvent(Event &event) {}

        inline const std::string &GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

    //-----------------------------------------------------------------------------
    // [SECTION] Application : Application
    //-----------------------------------------------------------------------------

    class Application
    {
    public:
        Application(const std::string &name = "Clde", uint32_t width = 1280, uint32_t height = 720);
        virtual ~Application();

        void Run();

        void OnEvent(Event &e);

        template <typename T>
        void PushLayer();

        void PushLayer(const std::shared_ptr<Layer> &layer);

        template <typename T>
        void PushOverlay();

        void PushOverlay(const std::shared_ptr<Layer> &Overlay);

        template <typename T>
        void PopLayer();

        void PopLayer(const std::shared_ptr<Layer> &layer);

        template <typename T>
        void PopOverlay();

        void PopOverlay(const std::shared_ptr<Layer> &Overlay);

        static Application &Get();

    private:
        bool OnWindowClose(Event &e);

        void Init();
        void Free();

    private:
        bool m_Running = true;
        float m_TimeStep = 0.0f;
        float m_FrameTime = 0.0f;
        float m_LastFrameTime = 0.0f;
        std::unique_ptr<Window> m_Window;

        uint32_t m_LayerInsertIndex = 0;
        std::vector<std::shared_ptr<Layer>> m_Layers;

    private:
        static Application *s_Instance;
    };

#endif // CLYDE_NO_APPLICATION

#pragma endregion Application

#pragma region Utilities

    enum class GLDebugLevel
    {
        None = 0,
        HighAssert = 1,
        High = 2,
        Medium = 3,
        Low = 4,
        Notification = 5
    };

    void EnableGLDebugging();
    void SetGLDebugLevel(GLDebugLevel level);

#pragma endregion Utilities

} // namespace clyde

#endif // CLYDE_CLYDE_H