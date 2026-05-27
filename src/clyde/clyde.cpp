#include "clyde/clyde.h"

// Provide stb_image implementation here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Provide stb_image_write implementation here
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

// Provide stb_truetype implementation here
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>

namespace clyde
{

    // ---- PIMPL Decls for unique_ptr ----
    struct Texture::Impl
    {
        unsigned int id{0};
        int width{0}, height{0};
    };

    struct Image::Impl
    {
        Impl(const std::string &path) : texture(path) {}
        Impl(Texture &&tex) : texture(std::move(tex)) {}
        Texture texture;
    };

    struct Font::Impl
    {
        Texture texture;
        stbtt_packedchar chardata[96];
    };

    struct Window::Impl
    {
        GLFWwindow *window{nullptr};
        int width{0}, height{0};
    };

    // ---- Keyboard ----

    bool Keyboard::isKeyPressed(Keyboard::Key key)
    {
        GLFWwindow *w = glfwGetCurrentContext();
        if (!w)
            return false;
        return glfwGetKey(w, static_cast<int>(key)) == GLFW_PRESS;
    }

    // ---- Mouse ----
    bool Mouse::isButtonPressed(Button button)
    {
        GLFWwindow *w = glfwGetCurrentContext();
        if (!w)
            return false;
        return glfwGetMouseButton(w, static_cast<int>(button)) == GLFW_PRESS;
    }

    Vec2i Mouse::getPosition()
    {
        GLFWwindow *w = glfwGetCurrentContext();
        if (!w)
            return {0, 0};
        double x, y;
        glfwGetCursorPos(w, &x, &y);
        return {(int)x, (int)y};
    }

    void Mouse::setPosition(Vec2i position)
    {
        GLFWwindow *w = glfwGetCurrentContext();
        if (!w)
            return;
        glfwSetCursorPos(w, position.x, position.y);
    }

    // ---- Text ----
    Text::Text(const std::string &str, const Font &font, int size)
        : m_text(str), m_font(&font), m_size(size)
    {
    }

    // ---- Texture ----
    static void checkGlError(const char *where)
    {
#ifndef NDEBUG
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cerr << "GL error at " << where << ": " << std::hex << err << std::dec << "\n";
        }
#endif
    }

    Texture::Texture(const std::string &path) : m_impl(std::make_unique<Impl>())
    {
        int channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *data = stbi_load(path.c_str(), &m_impl->width, &m_impl->height, &channels, 4);
        if (!data)
        {
            std::cerr << "Failed to load image: " << path << "\n";
            return;
        }

        glGenTextures(1, &m_impl->id);
        glBindTexture(GL_TEXTURE_2D, m_impl->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_impl->width, m_impl->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    Texture::~Texture()
    {
        if (m_impl && m_impl->id)
            glDeleteTextures(1, &m_impl->id);
    }

    unsigned int Texture::getId() const { return m_impl ? m_impl->id : 0; }
    int Texture::getWidth() const { return m_impl ? m_impl->width : 0; }
    int Texture::getHeight() const { return m_impl ? m_impl->height : 0; }

    Texture::Texture(Texture &&other) noexcept = default;
    Texture &Texture::operator=(Texture &&other) noexcept = default;

    // ---- Image ----
    Image::Image(const std::string &path) : m_impl(std::make_unique<Impl>(path))
    {
    }

    Image::Image(Texture &&texture) : m_impl(std::make_unique<Impl>(std::move(texture)))
    {
    }

    Image::~Image() = default;

    const Texture &Image::toTexture() const
    {
        return m_impl->texture;
    }

    // ---- Font ----
    Font::Font(const char *path) : m_impl(std::make_unique<Impl>())
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
        {
            std::cerr << "Failed to open font file: " << path << std::endl;
            return;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size))
        {
            std::cerr << "Failed to read font file: " << path << std::endl;
            return;
        }

        const int bitmapWidth = 512;
        const int bitmapHeight = 512;
        unsigned char temp_bitmap[bitmapWidth * bitmapHeight];

        stbtt_pack_context pc;
        stbtt_PackBegin(&pc, temp_bitmap, bitmapWidth, bitmapHeight, 0, 1, nullptr);
        stbtt_PackSetOversampling(&pc, 1, 1);
        stbtt_PackFontRange(&pc, (const unsigned char *)buffer.data(), 0, 32.0f, 32, 96, m_impl->chardata);
        stbtt_PackEnd(&pc);

        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLint swizzleMask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // The texture inside Font::Impl needs to be constructed properly.
        // Since Texture manages its own Impl via unique_ptr, we can't just assign to it.
        // Let's create a temporary Texture and move from it.
        Texture fontTexture;
        fontTexture.m_impl = std::make_unique<Texture::Impl>();
        fontTexture.m_impl->id = textureId;
        fontTexture.m_impl->width = bitmapWidth;
        fontTexture.m_impl->height = bitmapHeight;

        m_impl->texture = std::move(fontTexture);
    }

    Font::~Font() = default;

    unsigned int Font::getTextureId() const { return m_impl ? m_impl->texture.getId() : 0; }
    const void *Font::getCharData() const { return m_impl ? m_impl->chardata : nullptr; }

    // ---- Window / Renderer ----
    namespace
    {
        // Simple GL resources shared between Window instances
        struct RendererState
        {
            unsigned int shader{0};
            unsigned int vao{0}, vbo{0}, ebo{0};
            int projLoc{-1}, posLoc{-1}, sizeLoc{-1}, texLoc{-1};
            bool initialized{false};
            int winW{0}, winH{0};
        } renderer;

        static unsigned int compileShader(unsigned int type, const char *src)
        {
            unsigned int s = glCreateShader(type);
            glShaderSource(s, 1, &src, nullptr);
            glCompileShader(s);
            int ok = 0;
            glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
            if (!ok)
            {
                char buf[1024];
                glGetShaderInfoLog(s, sizeof(buf), nullptr, buf);
                std::cerr << "Shader compile error: " << buf << "\n";
            }
            return s;
        }

        static void checkProgramLink(unsigned int program)
        {
            int ok = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &ok);
            if (!ok)
            {
                char buf[1024];
                glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
                std::cerr << "Program link error: " << buf << "\n";
            }
        }

        static void ensureRenderer(int winW, int winH)
        {
            if (renderer.initialized && renderer.winW == winW && renderer.winH == winH)
                return;

            if (!renderer.initialized)
            {
                const char *vs = R"GLSL(
			#version 330 core
			layout(location = 0) in vec2 aPos;
			layout(location = 1) in vec2 aUV;
			uniform vec2 uPosition;
			uniform vec2 uSize;
			uniform mat4 uProj;
			out vec2 vUV;
			void main() {
				vec2 pos = aPos * uSize + uPosition;
				gl_Position = uProj * vec4(pos, 0.0, 1.0);
				vUV = aUV;
			}
		)GLSL";

                const char *fs = R"GLSL(
			#version 330 core
			in vec2 vUV;
			out vec4 FragColor;
			uniform sampler2D uTexture;
			void main() {
				FragColor = texture(uTexture, vUV);
			}
		)GLSL";

                unsigned int v = compileShader(GL_VERTEX_SHADER, vs);
                unsigned int f = compileShader(GL_FRAGMENT_SHADER, fs);

                if (v == 0 || f == 0)
                {
                    std::cerr << "Failed to compile shaders\n";
                    if (v)
                        glDeleteShader(v);
                    if (f)
                        glDeleteShader(f);
                    return;
                }

                renderer.shader = glCreateProgram();
                glAttachShader(renderer.shader, v);
                glAttachShader(renderer.shader, f);
                glLinkProgram(renderer.shader);
                checkProgramLink(renderer.shader);
                glDeleteShader(v);
                glDeleteShader(f);

                renderer.projLoc = glGetUniformLocation(renderer.shader, "uProj");
                renderer.posLoc = glGetUniformLocation(renderer.shader, "uPosition");
                renderer.sizeLoc = glGetUniformLocation(renderer.shader, "uSize");
                renderer.texLoc = glGetUniformLocation(renderer.shader, "uTexture");

                float vertices[] = {
                    // positions   // uvs
                    0.0f, 0.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 0.0f, 1.0f};
                unsigned int indices[] = {0, 1, 2, 2, 3, 0};

                glGenVertexArrays(1, &renderer.vao);
                glGenBuffers(1, &renderer.vbo);
                glGenBuffers(1, &renderer.ebo);

                glBindVertexArray(renderer.vao);
                glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
                // Allocate larger buffer for dynamic text rendering
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 256, vertices, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

                glBindVertexArray(0);

                renderer.initialized = true;
            }

            renderer.winW = winW;
            renderer.winH = winH;
            // update projection
            glUseProgram(renderer.shader);
            float left = 0.0f, right = static_cast<float>(winW);
            float bottom = static_cast<float>(winH), top = 0.0f;
            // orthographic projection (convert to normalized device coords)
            float proj[16] = {
                2.0f / (right - left), 0, 0, 0,
                0, 2.0f / (top - bottom), 0, 0,
                0, 0, -1, 0,
                -(right + left) / (right - left), -(top + bottom) / (top - bottom), 0, 1};
            glUniformMatrix4fv(renderer.projLoc, 1, GL_FALSE, proj);
        }

        static void cleanupRenderer()
        {
            if (renderer.initialized)
            {
                glDeleteProgram(renderer.shader);
                glDeleteVertexArrays(1, &renderer.vao);
                glDeleteBuffers(1, &renderer.vbo);
                glDeleteBuffers(1, &renderer.ebo);
                renderer.initialized = false;
            }
        }

        void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            if (win)
            {
                if (action == GLFW_PRESS)
                {
                    Event::KeyPressed pressed;
                    pressed.code = static_cast<Keyboard::Key>(key);
                    pressed.alt = (mods & GLFW_MOD_ALT) != 0;
                    pressed.control = (mods & GLFW_MOD_CONTROL) != 0;
                    pressed.shift = (mods & GLFW_MOD_SHIFT) != 0;
                    pressed.system = (mods & GLFW_MOD_SUPER) != 0;
                    win->PushEvent(Event{pressed});
                }
                else if (action == GLFW_RELEASE)
                {
                    Event::KeyReleased released;
                    released.code = static_cast<Keyboard::Key>(key);
                    released.alt = (mods & GLFW_MOD_ALT) != 0;
                    released.control = (mods & GLFW_MOD_CONTROL) != 0;
                    released.shift = (mods & GLFW_MOD_SHIFT) != 0;
                    released.system = (mods & GLFW_MOD_SUPER) != 0;
                    win->PushEvent(Event{released});
                }
            }
        }

        void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
        {
            Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            if (win)
            {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                if (action == GLFW_PRESS)
                {
                    Event::MousePressed pressed;
                    pressed.button = static_cast<Mouse::Button>(button);
                    pressed.position = {(int)x, (int)y};
                    win->PushEvent(Event{pressed});
                }
                else if (action == GLFW_RELEASE)
                {
                    Event::MouseReleased released;
                    released.button = static_cast<Mouse::Button>(button);
                    released.position = {(int)x, (int)y};
                    win->PushEvent(Event{released});
                }
            }
        }

        void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
        {
            Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            if (win)
            {
                Event::MouseMoved moved;
                moved.position = {(int)xpos, (int)ypos};
                win->PushEvent(Event{moved});
            }
        }

        void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
        {
            Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            if (win)
            {
                Event::MouseScrolled scrolled;
                scrolled.delta = yoffset; // Assuming vertical scroll for now
                scrolled.wheel = Mouse::Wheel::Vertical;
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                scrolled.position = {(int)x, (int)y};
                win->PushEvent(Event{scrolled});
            }
        }

    } // anonymous namespace

    // Static GLFW initialization guard
    namespace
    {
        static int glfwInitCount = 0;

        static bool ensureGlfwInit()
        {
            if (glfwInitCount == 0)
            {
                if (!glfwInit())
                {
                    std::cerr << "Failed to initialize GLFW\n";
                    return false;
                }
            }
            ++glfwInitCount;
            return true;
        }

        static void releaseGlfwInit()
        {
            if (glfwInitCount > 0)
            {
                --glfwInitCount;
                if (glfwInitCount == 0)
                {
                    glfwTerminate();
                }
            }
        }
    }

    Window::Window(int width, int height, const char *title)
        : m_impl(std::make_unique<Impl>())
    {
        m_impl->width = width;
        m_impl->height = height;

        if (!ensureGlfwInit())
        {
            return;
        }

        // Set window hints for OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        m_impl->window = glfwCreateWindow(m_impl->width, m_impl->height, title, nullptr, nullptr);
        if (!m_impl->window)
        {
            std::cerr << "Failed to create GLFW window with OpenGL 3.3 Core\n";
            std::cerr << "GLFW Error: ";
            const char *error;
            int code = glfwGetError(&error);
            if (error)
                std::cerr << error << "\n";
            releaseGlfwInit();
            return;
        }

        glfwMakeContextCurrent(m_impl->window);
        glfwSwapInterval(1); // Enable vsync

        glfwSetWindowUserPointer(m_impl->window, this);
        glfwSetKeyCallback(m_impl->window, key_callback);
        glfwSetMouseButtonCallback(m_impl->window, mouse_button_callback);
        glfwSetCursorPosCallback(m_impl->window, cursor_position_callback);
        glfwSetScrollCallback(m_impl->window, scroll_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD\n";
            releaseGlfwInit();
            return;
        }

        glViewport(0, 0, m_impl->width, m_impl->height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        ensureRenderer(m_impl->width, m_impl->height);
    }

    Window::~Window()
    {
        if (m_impl && m_impl->window)
        {
            glfwDestroyWindow(m_impl->window);
        }
        cleanupRenderer();
        releaseGlfwInit();
    }

    void Window::Open()
    {
        if (m_impl && m_impl->window)
            glfwShowWindow(m_impl->window);
    }

    void Window::Close()
    {
        if (m_impl && m_impl->window)
            glfwSetWindowShouldClose(m_impl->window, GLFW_TRUE);
    }

    bool Window::IsOpen() const
    {
        return m_impl && m_impl->window && !glfwWindowShouldClose(m_impl->window);
    }

    bool Window::ShouldClose() const
    {
        return m_impl && m_impl->window && glfwWindowShouldClose(m_impl->window);
    }

    bool Window::PollEvent(Event &event)
    {
        if (m_events.empty())
        {
            return false;
        }

        event = m_events.front();
        m_events.pop();
        return true;
    }

    void Window::PollEvents()
    {
        if (m_impl && m_impl->window)
        {
            glfwPollEvents();
        }
    }

    void Window::Clear(Color color)
    {
        if (!m_impl || !m_impl->window)
            return;
        glfwMakeContextCurrent(m_impl->window);
        glViewport(0, 0, m_impl->width, m_impl->height);
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::Display()
    {
        if (!m_impl || !m_impl->window)
            return;
        glfwSwapBuffers(m_impl->window);
    }

    void Window::Draw(const Shape &shape)
    {
        // Not implemented
    }

    void Window::Draw(const Texture &texture, Vec2f position)
    {
        if (!m_impl || !m_impl->window || texture.getId() == 0)
            return;
        glfwMakeContextCurrent(m_impl->window);
        ensureRenderer(m_impl->width, m_impl->height);

        glUseProgram(renderer.shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        glUniform1i(renderer.texLoc, 0);
        glUniform2f(renderer.posLoc, position.x, position.y);
        glUniform2f(renderer.sizeLoc, static_cast<float>(texture.getWidth()), static_cast<float>(texture.getHeight()));

        glBindVertexArray(renderer.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Window::Draw(const Image &image, Vec2f position)
    {
        Draw(image.toTexture(), position);
    }

    void Window::Draw(const Text &text, Vec2f position)
    {
        if (!m_impl || !m_impl->window)
            return;
        glfwMakeContextCurrent(m_impl->window);
        ensureRenderer(m_impl->width, m_impl->height);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(renderer.shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, text.getFont()->getTextureId());
        glUniform1i(renderer.texLoc, 0);

        const stbtt_packedchar *chardata = (const stbtt_packedchar *)text.getFont()->getCharData();

        float x = position.x;
        float y = position.y;

        for (char c : text.getString())
        {
            if (c >= 32 && c < 128)
            {
                stbtt_aligned_quad q;
                stbtt_GetPackedQuad(chardata, 512, 512, c - 32, &x, &y, &q, 1);

                float vertices[] = {
                    // positions   // uvs
                    q.x0, q.y0, q.s0, q.t0,
                    q.x1, q.y0, q.s1, q.t0,
                    q.x1, q.y1, q.s1, q.t1,
                    q.x0, q.y1, q.s0, q.t1};

                glBindVertexArray(renderer.vao);
                glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }

    void Window::PushEvent(const Event &event)
    {
        m_events.push(event);
    }

} // namespace clyde
