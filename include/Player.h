#pragma once

#include "Dealer.h"

class Player : public Dealer
{
public:
    Player();

    void calcChipsLayout(sf::RenderWindow& window);

    void setBetAmount(int betAmount_){ betAmount = betAmount_; }
    int getBetAmount(){ return betAmount; }

    void takeBet(int amount);

    void returnBet(int amount);

    void setMoney(int money_){ money = money_; };
    int getMoney(){ return money; }

    void doubleChips();

    void setMoneyLabel(std::shared_ptr<Element> moneyLabel_);

    void updateMoneyLabel();

    void activateChips();

    void deactivateChips();

    std::vector<std::shared_ptr<Element>> chips;

private:

    std::shared_ptr<Element> moneyLabel;
    int betAmount;
    int money;
};

