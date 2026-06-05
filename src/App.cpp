#include "App.h"

std::string globalFontPath = "resources/fonts/Roboto/static/Roboto-Regular.ttf"; 

sf::Font App::font;
App::Mouse App::mouse;

App::App() : window(sf::RenderWindow(sf::VideoMode(800, 600), "Blackjack")), isRunning(true), deltaTime(0)
{
    if(!font.loadFromFile(globalFontPath))
    {
        print("ERROR: font loading");
    }
}

App::~App()
{
    window.close();
}

void App::run()
{
    start();

    float accumulator = 0;
    sf::Clock deltaClock;

    sf::Event event;
    while(isRunning)
    {
        deltaTime = deltaClock.getElapsedTime().asSeconds();
        deltaClock.restart();
        accumulator += deltaTime;
        if(accumulator > 1) { accumulator = 0; }

        handleEvents(event);
        window.clear(sf::Color::White);
        update();
        window.display();
    }
}


void App::handleEvents(sf::Event& event)
{
    while(window.pollEvent(event))
    {
        if (event.type == sf::Event::Resized) 
        {
            float newWidth = event.size.width;
            float newHeight = event.size.height;
        }
        if(event.type == sf::Event::EventType::Closed)
        {
            isRunning = false;
        }
        if(event.type == sf::Event::EventType::MouseMoved)
        {
            App::mouse.x = event.mouseMove.x;
            App::mouse.y = event.mouseMove.y;

        }
        if(event.type == sf::Event::EventType::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Button::Left)
            {
                mousePressed();
            }

        }
        if(event.type == sf::Event::EventType::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Button::Left)
            {

            }
        }
    }
}