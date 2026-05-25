#pragma once

#include "Element.h"

class Player
{
public:
    Player();

    void setPosition(vec2f position_);
    vec2f getPosition() { return position; }

    void calcHandLayout(sf::RenderWindow& window);

    void calcChipsLayout(sf::RenderWindow& window);

    void setBetAmount(int betAmount_){ betAmount = betAmount_; }
    int getBetAmount(){ return betAmount; }

    int getHandValue();

    std::vector<std::shared_ptr<Element>> chips;
    std::vector<std::shared_ptr<Element>> hand;

private:

    vec2f position;
    int betAmount;
};