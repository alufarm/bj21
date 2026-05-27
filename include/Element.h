#pragma once

#include "App.h"

class Element
{
public:
    Element();

    void setPosition(const vec2f& position);

    void setSize(const vec2f& size);
    void setColor(const sf::Color& color);
    void setInnerText(std::string innerText);
    void setValue(int value_);
    void setActive(bool val);
    void setHidden(bool val);
    void setFontSize(int fontSize);
    void setCardHidden(bool val);

    sf::RectangleShape& getShape() { return shape; }
    int getValue() { return value; }
    vec2f getSize() { return shape.getSize(); }
    vec2f getPosition() { return shape.getPosition(); }
    bool active() { return isActive; }
    bool hidden() { return isHidden; }
    bool isCardHidden() { return cardHidden; }
    void show();
    void hide();
    
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHidden;
    bool isActive;
    bool cardHidden;
    int value;
};