#include "clyde/clyde.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    std::unique_ptr<clyde::Window> window = std::unique_ptr<clyde::Window>(clyde::Window::Create({"Sample", screenWidth, screenHeight}));

    while (true)
    {
        window->OnUpdate();
    }

    return 0;
}
