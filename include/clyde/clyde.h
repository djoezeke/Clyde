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

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include <chrono>
#include <memory>
#include <queue>
#include <variant>

struct GLFWwindow;

namespace clyde
{

#pragma region System

    //-----------------------------------------------------------------------------
    // [SECTION] System : Time
    //-----------------------------------------------------------------------------

    class Time
    {
    public:
        constexpr Time() = default;

        [[nodiscard]] constexpr float asSeconds() const { return static_cast<float>(m_microseconds.count()) / 1e6f; }
        [[nodiscard]] constexpr std::int32_t asMilliseconds() const;
        [[nodiscard]] constexpr std::int64_t asMicroseconds() const;

    public:
        std::chrono::microseconds m_microseconds{};
    };

    class Clock
    {
    public:
        [[nodiscard]] Time getElapsedTime() const;
        [[nodiscard]] bool isRunning() const;
        void start();
        void stop();
        Time restart();
        Time reset();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
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
     * @brief A 2D vector.
     * @tparam T The type of the vector components.
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
         * @brief Constructor with radius and angle.
         * @param r The radius.
         * @param phi The angle in radians.
         */
        Vec2(T r, Angle phi);

        /**
         * @brief Get the area of the vector.
         * @return The area.
         */
        inline constexpr auto area() const;

        /**
         * @brief Get the magnitude of the vector.
         * @return The magnitude.
         */
        inline constexpr auto mag() const;

        /**
         * @brief Get the normalized vector.
         * @return The normalized vector.
         */

        inline constexpr Vec2 norm() const;
        /**
         * @brief Get the perpendicular vector.
         * @return The perpendicular vector.
         */

        inline constexpr Vec2 perp() const;
        /**
         * @brief Get the ceiling vector.
         * @return The ceiling vector.
         */

        inline constexpr Vec2 ceil() const;
        /**
         * @brief Get the maximum vector.
         * @param v The vector to compare.
         * @return The maximum vector.
         */

        inline constexpr Vec2 max(const Vec2 &v) const;
        /**
         * @brief Get the minimum vector.
         * @param v The vector to compare.
         * @return The minimum vector.
         */

        inline constexpr Vec2 min(const Vec2 &v) const;
        /**
         * @brief Get the dot product.
         * @param v The vector to dot.
         * @return The dot product.
         */

        inline constexpr Vec2 dot(const Vec2 &v) const;
        /**
         * @brief Get the cross product.
         * @param v The vector to cross.
         * @return The cross product.
         */

        inline constexpr Vec2 cross(const Vec2 &v) const;
        /**
         * @brief Get the Cartesian coordinates.
         * @return The Cartesian coordinates.
         */

        inline constexpr Vec2 cart() const;
        /**
         * @brief Get the polar coordinates.
         * @return The polar coordinates.
         */

        inline constexpr Vec2 polar() const;
        /**
         * @brief Get the clamped vector.
         * @param v1 The lower bound.
         * @param v2 The upper bound.
         * @return The clamped vector.
         */

        inline constexpr Vec2 clamp(const Vec2 &v1, const Vec2 &v2) const;
        /**
         * @brief Get the linear interpolation.
         * @param v1 The start vector.
         * @param t The interpolation factor.
         * @return The interpolated vector.
         */

        inline constexpr Vec2 lerp(const Vec2 &v1, const double t) const;

        /**
         * @brief Get the reflection.
         * @return The reflection.
         */
        inline constexpr Vec2 reflect() const;

        /**
         * @brief Get the length of the vector.
         * @return The length.
         */
        [[nodiscard]] T length() const;

        /**
         * @brief Get the length squared.
         * @return The length squared.
         */
        [[nodiscard]] constexpr T lengthSquared() const;

        /**
         * @brief Get the angle to another vector.
         * @param rhs The other vector.
         * @return The angle.
         */
        [[nodiscard]] Angle angleTo(Vec2 rhs) const;

        /**
         * @brief Get the angle.
         * @return The angle.
         */
        [[nodiscard]] Angle angle() const;

        /**
         * @brief Get the vector rotated by an angle.
         * @param phi The angle to rotate.
         * @return The rotated vector.
         */
        [[nodiscard]] Vec2 rotatedBy(Angle phi) const;

        /**
         * @brief Get the vector projected onto an axis.
         * @param axis The axis.
         * @return The projected vector.
         */
        [[nodiscard]] Vec2 projectedOnto(Vec2 axis) const;

    public:
        T x{}; // x-axis component
        T y{}; // y-axis component
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

    class Pixel
    {
        enum Format
        {
            R8G8B8,
        };
        enum Mode
        {
            NORMAL,
            MASK,
            ALPHA,
            CUSTOM,
        };
    };

    class Font
    {
    public:
        enum Format
        {
            ttf,
        };

    public:
        Font() = default;
        Font(const char *path);
        Font(const std::string &path);
        Font(Format format, const unsigned char *data, int size);

        ~Font();

        unsigned int getTextureId() const;
        const void *getCharData() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    class Texture
    {
    public:
        enum Format
        {
            png,
        };

    public:
        Texture() = default;
        Texture(const char *path);
        Texture(const std::string &path);
        Texture(Format format, const unsigned char *data, int size);
        ~Texture();

        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        Texture(Texture &&) noexcept;
        Texture &operator=(Texture &&) noexcept;

        unsigned int getId() const;
        int getWidth() const;
        int getHeight() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;

        friend class Font;
    };

    class Image
    {
    public:
        enum Format
        {
            PNG,
            BMP,
            TGA,
            JPG,
            JPEG,
        };

    public:
        Image() = default;
        Image(Texture texture);
        Image(const char *file);
        Image(const std::string &path);
        Image(Format format, const unsigned char *data, int size);

        Image(Texture &&texture);
        ~Image();

        const Texture &toTexture() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    class Text
    {
    public:
        Text(const std::string &str, const Font &font, int size = 30);

        const std::string &getString() const { return m_text; }
        const Font *getFont() const { return m_font; }
        int getSize() const { return m_size; }

    private:
        std::string m_text;
        const Font *m_font;
        int m_size;
    };

    class Shape
    {
    public:
        Shape() = default;
        virtual ~Shape() = default;
    };

#pragma endregion Graphics

#pragma region Renderer

    class Renderer
    {
    };

    class Shader
    {
    };

#pragma endregion Renderer

#pragma region Windows

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Input
    //-----------------------------------------------------------------------------

    /**
     * @defgroup input Window Inputs.
     * @brief Window Input Classes.
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
        [[nodiscard]] bool isButtonPressed(Button button);

        [[nodiscard]] Vec2i getPosition();

        void setPosition(Vec2i position);
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

        static bool isKeyPressed(Key key);
    };

    /** @} group input */

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Event
    //-----------------------------------------------------------------------------

    class Event
    {
    public:
        Event() = default;
        template <typename T>
        Event(const T &data) : m_data(data) {}

        struct MouseScrolled
        {
            Mouse::Wheel wheel{};
            Vec2i position;
            float delta{};
        };

        struct MouseMoved
        {
            Vec2i position;
        };

        struct MousePressed
        {
            Mouse::Button button{};
            Vec2i position;
        };

        struct MouseReleased
        {
            Mouse::Button button{};
            Vec2i position;
        };

        struct KeyPressed
        {
            Keyboard::Key code{};
            bool alt{};
            bool shift{};
            bool system{};
            bool control{};
        };

        struct KeyReleased
        {
            Keyboard::Key code{};
            bool alt{};
            bool shift{};
            bool system{};
            bool control{};
        };

        template <typename EventType>
        [[nodiscard]] bool is() const
        {
            return std::holds_alternative<EventType>(m_data);
        }

        template <typename EventType>
        [[nodiscard]] const EventType &get() const
        {
            return std::get<EventType>(m_data);
        }

    private:
        std::variant<
            KeyPressed,
            KeyReleased,
            MouseScrolled,
            MouseMoved,
            MousePressed,
            MouseReleased>
            m_data;
    };

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Window
    //-----------------------------------------------------------------------------

    class Window
    {
    public:
        Window(int width, int height, const char *title);
        ~Window();

        void Open();
        void Close();
        bool IsOpen() const;
        bool ShouldClose() const;

        void PollEvents();
        bool PollEvent(Event &event);

        void Clear(Color color = {0, 0, 0, 255});
        void Display();

        void Draw(const Shape &shape);
        void Draw(const Image &image, Vec2f position);
        void Draw(const Text &text, Vec2f position);
        void Draw(const Texture &texture, Vec2f position);

        void PushEvent(const Event &event);

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
        std::queue<Event> m_events;
    };

#pragma endregion Windows

} // namespace clyde

#endif // CLYDE_CLYDE_H