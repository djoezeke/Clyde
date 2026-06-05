/**
 * @file clyde.cpp
 * @brief Clyde Graphics library.
 * @details All public API, types, macros, and configuration.
 * @author Sackey Ezekiel  Etrue (djoezeke)
 * @version 0.1.0
 * @see https://www.github.com/djoezeke/clyde
 * @copyright Copyright (c) 202 Sackey Ezekiel Etrue
 *
 * Developed by Sackey Ezekiel Etrue and every direct or indirect contributors to the GitHub.
 * See LICENSE for copyright and licensing details (standard MIT License).
 *
 * SECTIONS: Index of this file
 *
 *      Details: Details Namespace
 *
 *          [SECTION] Details : Encoding
 *
 *
 *          [SECTION] Details : Exceptions
 *
 *
 * CONTRIBUTORS:
 *
 * TODO:
 *
 * HELP:
 *    - See links below.
 *    - Read top of clyde.h for more details and comments.
 *
 *  Has only had a few tests run, may have issues.
 *
 *  If having issues compiling/linking/running raise an issue (https://github.com/djoezeke/clyde/issues).
 *  Please post data https://github.com/djoezeke/clyde/discussions if you cannot find a solution data resources above.
 *
 * RESOURCES:
 * - Homepage ................... https://github.com/djoezeke/clyde
 * - Releases & changelog ....... https://github.com/djoezeke/clyde/releases
 * - Issues & support ........... https://github.com/djoezeke/clyde/issues
 *
 */

#include "clyde/clyde.h"

// clang-format off

#if defined(_MSC_VER)
    #define GL_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #define GL_DEBUG_BREAK() __builtin_trap()
#else
    #define GL_DEBUG_BREAK() abort()
#endif

#ifdef CLYDE_DEBUG
    #define GL_CALL(x) x
#else
    #define GL_CALL(x)                                                         \
        x;                                                                     \
        if (const char *errStr = glCheckError(#x, __FILE__, __LINE__)) \
        {                                                                      \
            std::cerr << errStr << std::endl;                                  \
            GL_DEBUG_BREAK();                                                  \
        }
#endif // CLYDE_DEBUG

// clang-format on

// Helper function to decode the string
inline const char *glCheckError(const char *function, const char *file, int line)
{
    // Get the last error
    GLenum errCode = glGetError();
    if (errCode == GL_NO_ERROR)
        return nullptr;

    std::string files = file;
    std::string funcs = function;
    std::string error = "Unknown error";
    std::string description = "No description";

    switch (errCode)
    {
    case GL_INVALID_ENUM:
        error = "GL_INVALID_ENUM";
        description = "An unacceptable value has been specified for an enumerated argument.";
        break;
    case GL_INVALID_VALUE:
        error = "GL_INVALID_VALUE";
        description = "A numeric argument is out of range.";
        break;
    case GL_INVALID_OPERATION:
        error = "GL_INVALID_OPERATION";
        description = "The specified operation is not allowed in the current state.";
        break;
    case GL_OUT_OF_MEMORY:
        error = "GL_OUT_OF_MEMORY";
        description = "There is not enough memory left to execute the command.";
        break;
#ifdef GL_STACK_OVERFLOW
    case GL_STACK_OVERFLOW:
        error = "GL_STACK_OVERFLOW";
        description = "This command would cause a stack overflow.";
        break; // Legacy
#endif
#ifdef GL_STACK_UNDERFLOW
    case GL_STACK_UNDERFLOW:
        error = "GL_STACK_UNDERFLOW";
        description = "This command would cause a stack underflow.";
        break; // Legacy
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "GL_INVALID_FRAMEBUFFER_OPERATION";
        description = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
        break; // 3.0+
#endif
    }

    std::ostringstream s;
    s << "[OpenGL Error] ( " << error << " ) "
      << files.substr(files.find_last_of("\\/") + 1) << "(" << line << ")."
      << "\nFunction: " << funcs
      << "\nDescription: " << description << "\n"
      << std::endl;

    return s.str().c_str();
};

#if !defined(CLYDE_OPENGL_11) && \
    !defined(CLYDE_OPENGL_21) && \
    !defined(CLYDE_OPENGL_33) && \
    !defined(CLYDE_OPENGL_43)

// OpenGL Version 3.3
#define CLYDE_OPENGL_33

#endif // CLYDE_OPENGL_*

#if defined(CLYDE_OPENGL_43)
// OpenGL Version 3.3
#define CLYDE_OPENGL_33
#endif

#pragma region glad

#include <GLAD/glad.h>

#pragma endregion glad

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>

#pragma region clyde

namespace clyde
{

#pragma region System

    //-----------------------------------------------------------------------------
    // [Class] Time
    //-----------------------------------------------------------------------------

    // Time::asSeconds() moved to inline in clyde.h
    constexpr int32_t clyde::Time::asMilliseconds() const { return static_cast<int32_t>(m_microseconds.count() / 1000); };
    constexpr int64_t clyde::Time::asMicroseconds() const { return static_cast<int64_t>(m_microseconds.count()); };

    //-----------------------------------------------------------------------------
    // [Class] Clock
    //-----------------------------------------------------------------------------

    clyde::Time clyde::Clock::getElapsedTime() const {};
    bool clyde::Clock::isRunning() const {};
    void clyde::Clock::start() {};
    void clyde::Clock::stop() {};
    clyde::Time clyde::Clock::restart() {};
    clyde::Time clyde::Clock::reset() {};

#pragma endregion System

#pragma region Geometry

    //-----------------------------------------------------------------------------
    // [Class] Angle
    //-----------------------------------------------------------------------------

    constexpr float clyde::Angle::asDegrees() const { return radians * 180.0f / 3.14159265359f; };

    constexpr float clyde::Angle::asRadians() const { return radians; };

    constexpr Angle degrees(float angle)
    {
        Angle a;
        a.radians = angle * 3.14159265359f / 180.0f;
        return a;
    };

    constexpr Angle radians(float angle)
    {
        Angle a;
        a.radians = angle;
        return a;
    };

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Vectors
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Vec2
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Vec3
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Vec3
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Geometry : Matrices
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Mat
    //-----------------------------------------------------------------------------

#pragma endregion Geometry

#pragma region Graphics

    //-----------------------------------------------------------------------------
    // [Class] Shader
    //-----------------------------------------------------------------------------

    static std::string ReadFileAsString(const std::string &filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            LOG_ERROR("Could not open file '%s'", filepath);
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // [Class] Texture
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Font
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Image
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Text
    //-----------------------------------------------------------------------------

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

#pragma region OpenGL43

#if defined(CLYDE_RENDERER_OPENGL43)

#endif // CLYDE_RENDERER_OPENGL43

#pragma endregion OpenGL43

#pragma endregion Renderer

#pragma region Platform

    Window *Window::s_Instance = nullptr;
    // Mouse *Mouse::s_Instance = nullptr;
    // Keyboard *Keyboard::s_Instance = nullptr;

#pragma region Windows

#if defined(CLYDE_PLATFORM_WINDOWS)

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Input
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Mouse
    //-----------------------------------------------------------------------------

    Mouse *Mouse::s_Instance = new WindowsMouse();

    bool WindowsMouse::IsButtonPressedImpl(Button button)
    {
        auto window = static_cast<GLFWwindow *>(Window::Get().GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    };

    std::pair<float, float> WindowsMouse::GetPositionImpl()
    {
        auto window = static_cast<GLFWwindow *>(Window::Get().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {(float)xpos, (float)ypos};
    };

    //-----------------------------------------------------------------------------
    // [Class] Keyboard
    //-----------------------------------------------------------------------------

    Keyboard *Keyboard::s_Instance = new WindowsKeyboard();

    bool WindowsKeyboard::IsKeyPressedImpl(Keyboard::Key key)
    {
        auto window = static_cast<GLFWwindow *>(Window::Get().GetNativeWindow());
        auto state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    };

    //-----------------------------------------------------------------------------
    // [SECTION] Windows : Window
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [Class] Window
    //-----------------------------------------------------------------------------

    // Window *Window::s_Instance = nullptr;

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char *description) {
        // LOG_ERROR("GLFW Error (%s): {1}", error, description);
    };

    Window *Window::Create(const WindowProps &props)
    {
        WindowsWindow *w = new WindowsWindow(props);
        s_Instance = w;
        return w;
    };

    Window *Window::Create(const std::string &title, uint32_t width, uint32_t height)
    {
        WindowsWindow *w = new WindowsWindow({title, width, height});
        s_Instance = w;
        return w;
    };

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        Init(props);
    };

    WindowsWindow::~WindowsWindow()
    {
        Free();
    };

    void WindowsWindow::Init(const WindowProps &props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        LOG_INFO("OpenGL Info:\n");
        LOG_INFO("  Vendor: %s\n", glGetString(GL_VENDOR));
        LOG_INFO("  Renderer: %s\n", glGetString(GL_RENDERER));
        LOG_INFO("  Version: %s\n", glGetString(GL_VERSION));

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
                                  {
                                      WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                                      data.Width = width;
                                      data.Height = height;

                                      WindowResize event(width, height);
                                      data.EventCallback(event); });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
                                   {
                                       WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                                       WindowClose event;
                                       data.EventCallback(event); });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
                    KeyPressed event(key);
					data.EventCallback(event);
                    break;
				}
				case GLFW_RELEASE:
				{
                    KeyReleased event(key);
                    data.EventCallback(event);
                    break;
                }
				case GLFW_REPEAT:
                {
                    KeyPressed event(key);
                    data.EventCallback(event);
                    break;
                }
            } });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, uint32_t keycode)
                            { WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window); 
			KeyTyped event(keycode);
			data.EventCallback(event); });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
                                   {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
                    MousePressed event(button);
                    data.EventCallback(event);
                    break;
				}
				case GLFW_RELEASE:
				{
                    MouseReleased event(button);
					data.EventCallback(event);
					break;
				}
			} });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
                              { WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window); 			
                                MouseScrolled event((float)xOffset, (float)yOffset);
			data.EventCallback(event); });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos)
                                 { WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window); 
                                			MouseMoved event((float)xPos, (float)yPos);
			data.EventCallback(event); });
    };

    void WindowsWindow::Free()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
        }
        glfwTerminate();
    };

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    };

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    };

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    };

#endif // CLYDE_PLATFORM_WINDOWS

#pragma endregion Windows

#pragma region Linux

#if defined(CLYDE_PLATFORM_LINUX)

    //-----------------------------------------------------------------------------
    // [SECTION] Linux : Input
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // [SECTION] Linux : Window
    //-----------------------------------------------------------------------------

#endif // CLYDE_PLATFORM_LINUX

#pragma endregion Linux

#pragma endregion Platform

#pragma region Application

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    //-----------------------------------------------------------------------------
    // [SECTION] Application : Layer
    //-----------------------------------------------------------------------------

    Layer::Layer(const std::string &name)
        : m_Name(name) {
          };

    void Layer::OnAttach() {};
    void Layer::OnDetach() {};

    void Layer::OnUpdate(float ts) {};
    void Layer::OnEvent(Event &event) {};

    inline const std::string &Layer::GetName() const { return m_Name; };

    //-----------------------------------------------------------------------------
    // [SECTION] Application : Application
    //-----------------------------------------------------------------------------

    Application *Application::s_Instance = nullptr;

    Application::Application(const std::string &name, uint32_t width, uint32_t height)
    {
        s_Instance = this;

        Init(name, width, height);
    };

    Application::~Application()
    {
        Free();

        s_Instance = nullptr;
    };

    Application &Application::Get()
    {
        return *s_Instance;
    };

    template <typename T>
    void Application::PushLayer()
    {
        static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
        auto layer = m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::make_shared<T>());
        m_LayerInsertIndex++;
        layer->OnAttach();
    };

    void Application::PushLayer(const std::shared_ptr<Layer> &layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
        layer->OnAttach();
    };

    template <typename T>
    void Application::PushOverlay()
    {
        static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
        m_Layers.emplace_back(std::make_shared<T>())->OnAttach();
    };

    void Application::PushOverlay(const std::shared_ptr<Layer> &overlay)
    {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    };

    template <typename T>
    void Application::PopLayer()
    {
        static_assert(std::is_base_of<Layer, T>::value, "Pop type is not subclass of Layer!");

        auto layer = std::make_shared<T>();
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex)
        {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    };

    void Application::PopLayer(const std::shared_ptr<Layer> &layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex)
        {
            layer->OnDetach();
            m_Layers.erase(it);
        }
    };

    template <typename T>
    void Application::PopOverlay()
    {
        static_assert(std::is_base_of<Layer, T>::value, "Pop type is not subclass of Layer!");

        auto overlay = std::make_shared<T>();
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    };

    void Application::PopOverlay(const std::shared_ptr<Layer> &overlay)
    {
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    };

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowClose>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_Layers.end(); it != m_Layers.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    };

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            float timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (auto layer : m_Layers)
                layer->OnUpdate(timestep);

            m_Window->OnUpdate();
        }
    };

    bool Application::OnWindowClose(Event &e)
    {
        m_Running = false;
        return true;
    };

    void Application::Init(const std::string &name, uint32_t width, uint32_t height)
    {
        m_Window = std::unique_ptr<Window>(Window::Create({name, width, height}));
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    };

    void Application::Free() {
    };

#pragma endregion Application

#pragma region Utilities

    static GLDebugLevel s_GLDebugLevel = GLDebugLevel::HighAssert;

    void SetGLDebugLevel(GLDebugLevel level)
    {
        s_GLDebugLevel = level;
    };

#if defined(CLYDE_RENDERER_OPENGL43)

    void GLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            if ((int)s_GLDebugLevel > 0)
            {
                LOG_ERROR("[OpenGL Debug HIGH] %s", message);
                if (s_GLDebugLevel == GLDebugLevel::HighAssert)
                    CLYDE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
            }
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            if ((int)s_GLDebugLevel > 2)
                LOG_WARN("[OpenGL Debug MEDIUM] %s", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            if ((int)s_GLDebugLevel > 3)
                LOG_INFO("[OpenGL Debug LOW] %s", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            if ((int)s_GLDebugLevel > 4)
                LOG_TRACE("[OpenGL Debug NOTIFICATION] %s", message);
            break;
        }
    };

#endif // CLYDE_RENDERER_OPENGL43

    void EnableGLDebugging()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

#if defined(CLYDE_RENDERER_OPENGL43)
        glDebugMessageCallback(GLLogMessage, nullptr);
#endif // CLYDE_RENDERER_OPENGL43
    };

#pragma endregion Utilities

    void ensureExtensionsInit()
    {
        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            // Retrieve the context version number
            int majorVersion = 0;
            int minorVersion = 0;

            // Try the new way first
            glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
            glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

            if (glGetError() == GL_INVALID_ENUM)
            {
                // Try the old way
                const GLubyte *version = glGetString(GL_VERSION);
                if (version)
                {
                    // The beginning of the returned string is "major.minor" (this is standard)
                    majorVersion = version[0] - '0';
                    minorVersion = version[2] - '0';
                }
                else
                {
                    // Can't get the version number, assume 1.0
                    majorVersion = 1;
                    minorVersion = 0;
                }
            }

            if ((majorVersion < 1))
            {
                CLYDE_ASSERT(false, "OpenGL 1.0");
            }
        }
    }

} // namespace clyde

#pragma endregion clyde
