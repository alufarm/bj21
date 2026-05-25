#include "Player.h"

Player::Player() : position(vec2f(0, 0)), betAmount(0) {}

void Player::setPosition(vec2f position_) { position = position_; }

void Player::calcHandLayout(sf::RenderWindow& window)
{
    if(hand.empty()) { return; }
    vec2f cardSize = hand.back()->getSize();
    vec2f layoutPosition = vec2f(
        position.x - hand.size() * cardSize.x / 2.0f,
        position.y
    );
    
    for(int i = 0; i < hand.size(); i++)
    {
        hand[i]->setPosition(vec2f(layoutPosition.x + i * (cardSize.x), layoutPosition.y));
        hand[i]->setHidden(false);
    }
}

void Player::calcChipsLayout(sf::RenderWindow& window)
{
    if (chips.empty()) 
    {
        return; 
    }

    vec2f chipSize = chips.back()->getSize();
    vec2f layoutPosition = ((vec2f)window.getSize() - chipSize) / 2.0f;
    layoutPosition.y += chipSize.y * 2;

    for(int i = 0; i < chips.size(); i++)
    {
        chips[i]->setPosition(vec2f(layoutPosition.x, layoutPosition.y - i * (chipSize.y / 4)));
    }
}

int Player::getHandValue()
{
    int value = 0;

    for(const auto& card : hand)
    {
        value += card->getValue();
    }

    return value;
}