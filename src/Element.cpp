#include "Element.h"

Element::Element() : isHidden(false), isActive(true), value(0), cardHidden(false)
{
    text.setFont(App::getGlobalFont());
    text.setCharacterSize(8);
    text.setFillColor(sf::Color::Black);
}

void Element::setPosition(const vec2f& position)
{
    shape.setPosition(position);
    sf::FloatRect bounds = text.getLocalBounds();
    vec2f textPosition = vec2f(position + (shape.getSize() - vec2f(bounds.width, bounds.height)) / 2.0f - vec2f(bounds.left, bounds.top));
    text.setPosition((int)textPosition.x, (int)textPosition.y);
}

void Element::setSize(const vec2f& size) { shape.setSize(size); }
void Element::setColor(const sf::Color& color) { shape.setFillColor(color); }
void Element::setInnerText(std::string innerText) { text.setString(innerText); }
void Element::setValue(int value_) { value = value_; }
void Element::setActive(bool val) { isActive = val; }
void Element::setHidden(bool val) { isHidden = val; }
void Element::setFontSize(int fontSize) { text.setCharacterSize(fontSize); }
void Element::setCardHidden(bool val) { cardHidden = val; }

void Element::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(text);
}

void Element::show()
{
    setActive(true);
    setHidden(false);
}

void Element::hide()
{
    setActive(false);
    setHidden(true);
}