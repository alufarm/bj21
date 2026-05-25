#pragma once

#include "Utility.h"

class App
{
public:
    App();
    ~App();

    void run();

    sf::RenderWindow& getWindow() { return window; }
    static sf::Font& getGlobalFont() { return font; }

    struct Mouse{ int x; int y; };
    static Mouse mouse;

private:
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void mousePressed() = 0;
    void handleEvents(sf::Event& event);

    sf::RenderWindow window;
    static sf::Font font;
    float deltaTime;
    bool isRunning;
};