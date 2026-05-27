#pragma once

#include "Element.h"

class Dealer
{
public:
    Dealer();
    virtual ~Dealer() = default;

    void setPosition(vec2f position_);
    vec2f getPosition();

    void calcHandLayout(sf::RenderWindow& window);

    int getHandValue();

    void takeCard(std::vector<std::shared_ptr<Element>>& deck);

    bool isBlackjack();

    std::vector<std::shared_ptr<Element>> hand;

protected:
    
    vec2f position;
};