#include "clyde/clyde.h"
#include <GLAD/glad.h>
#include <iostream>

class DemoLayer : public clyde::Layer
{
public:
    DemoLayer() {};

    virtual ~DemoLayer() {};

    virtual void OnAttach() override
    {
        clyde::EnableGLDebugging();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::cout << "OnAttach\n";
    };

    virtual void OnDetach() override
    {
        std::cout << "OnDetach\n";
    };

    virtual void OnEvent(clyde::Event &event) override
    {
        clyde::EventDispatcher dispatcher(event);

        dispatcher.Dispatch<clyde::MousePressed>(
            [&](clyde::MousePressed &e)
            {
                std::cout << e.code << "\n";
                return false;
            });

        dispatcher.Dispatch<clyde::MouseReleased>(
            [&](clyde::MouseReleased &e)
            {
                std::cout << e.code << "\n";
                return false;
            });
    };

    virtual void OnUpdate(float ts) override
    {
        glClearColor(0.1f, 1.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };
};

class DemoApplication : public clyde::Application
{
public:
    DemoApplication()
        : Application()
    {
        auto layer = std::make_shared<DemoLayer>();
        PushLayer(layer);
    };

    ~DemoApplication() {
    };
};

int main(int argc, char const *argv[])
{
    std::unique_ptr<clyde::Application> app = std::make_unique<DemoApplication>();
    app->Run();

    return 0;
};
