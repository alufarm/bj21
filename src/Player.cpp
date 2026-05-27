#include "Player.h"

Player::Player() : betAmount(0), money(100), moneyLabel(nullptr) {}

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

void Player::takeBet(int amount)
{
    setBetAmount(betAmount + amount);
    setMoney(money - amount);
    updateMoneyLabel();
}

void Player::returnBet(int amount)
{
    setBetAmount(betAmount - amount);
    setMoney(money += amount);
    updateMoneyLabel();
}

void Player::doubleChips()
{
    int chipsSize = chips.size();
    for(int i = 0; i < chipsSize; ++i)
    {
        std::shared_ptr<Element> plChip = std::make_shared<Element>(*chips[i]);
        chips.push_back(plChip);
    }
}

void Player::setMoneyLabel(std::shared_ptr<Element> moneyLabel_)
{
    moneyLabel = moneyLabel_;
}

void Player::updateMoneyLabel()
{
    if(!moneyLabel)return;
    moneyLabel->setInnerText(std::to_string(money));
    moneyLabel->setValue(money);
}

void Player::activateChips()
{
    for(auto&& it : chips)
    {
        it->setActive(true);
    }
}

void Player::deactivateChips()
{
    for(auto&& it : chips)
    {
        it->setActive(false);
    }
}