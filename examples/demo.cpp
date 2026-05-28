#include "clyde/clyde.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    clyde::Window window = clyde::Window(screenWidth, screenHeight, "Sample");

    // clyde::Texture texture = clyde::Texture("face.png");
    // clyde::Image image = clyde::Image("parrot.png");
    // clyde::Font font = clyde::Font("tuffy.ttf");
    // clyde::Text text = clyde::Text("Hello, Clyde!", font, 20);

    while (window.IsOpen())
    {
        // window.PollEvents();
        // window.Clear(clyde::Color(50, 50, 50));

        if (clyde::Keyboard::isKeyPressed(clyde::Keyboard::Key::Escape))
        {
            window.Close();
        }

        // window.Draw(texture, {100, 100});
        // window.Draw(image, {300, 100});
        // window.Draw(text, {200, 200});
        // window.Display();
    }

    return 0;
}
