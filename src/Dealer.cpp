#include "Dealer.h"

Dealer::Dealer()
{
}

void Dealer::setPosition(vec2f position_)
{
    position = position_;
}

vec2f Dealer::getPosition() 
{ 
    return position; 
}

void Dealer::calcHandLayout(sf::RenderWindow& window)
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

int Dealer::getHandValue()
{
    int value = 0;

    for(const auto& card : hand)
    {
        value += card->getValue();
    }

    for(const auto& card : hand)
    {
        if(card->getValue() == 11 && value > 21)
        {
            card->setValue(1);
        }
    }

    return value;
}

void Dealer::takeCard(std::vector<std::shared_ptr<Element>>& deck)
{
    hand.push_back(std::move(deck.back()));
    deck.pop_back();
}

bool Dealer::isBlackjack()
{
    return getHandValue() == 21;
}
