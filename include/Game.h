#pragma once

#include "Player.h"

class Game : public App
{
private:

    vec2f windowSize = (vec2f)getWindow().getSize();

    std::vector<std::shared_ptr<Element>> chips;
    int chipsCount = 5;
    vec2f chipsSize = vec2f(50, 50);

    std::vector<std::shared_ptr<Element>> cards;
    int cardsCount = 52;
    vec2f cardsSize = vec2f(50, 100);

    std::shared_ptr<Element> backCard = std::make_shared<Element>();
    vec2f backCardPosition = vec2f(20, windowSize.y / 2 - cardsSize.y);

    std::shared_ptr<Element> betButton = std::make_shared<Element>(); 
    std::shared_ptr<Element> hitButton = std::make_shared<Element>();
    std::shared_ptr<Element> standButton = std::make_shared<Element>();
    std::shared_ptr<Element> restartButton = std::make_shared<Element>();

    std::shared_ptr<Element> moneyLabel = std::make_shared<Element>();
    int money = 100;

    std::vector<std::shared_ptr<Element>> elems;

    Player player;
    Player dealer;

    std::vector<std::string> values = {"A","2","3","4","5","6","7","8","9","10","VALET","LADY","KING"};
    std::vector<std::string> types = {"DIAMONDS", "SPADE", "HEARTS", "CLUBS"}; 

    void start() override
    {
        for(int i = 0; i < chipsCount; i++)
        {
            std::shared_ptr<Element> chip = std::make_shared<Element>();
            chip->setValue(i * 20 + 20);
            chip->setInnerText(std::to_string(chip->getValue()));
            chip->setSize(chipsSize);
            chip->setPosition(vec2f((windowSize.x - chipsCount * chipsSize.x) / 2 + i * chipsSize.x, windowSize.y - chipsSize.y));
            chip->setColor(getRandomColor());
            chips.push_back(chip);
            elems.push_back(chip);
        }

        for(int i = 0; i < cardsCount / 13 && i < types.size(); i++)
        {
            for(int j = 0; j < cardsCount / 4 && j < values.size(); j++)
            {
                std::shared_ptr<Element> card = std::make_shared<Element>(); 
                card->setSize(cardsSize); 
                card->setInnerText(values[j] + "\n" + types[i]);
                card->setPosition(vec2f(j * cardsSize.x, i * cardsSize.y));
                card->setColor(getRandomColor());
                card->setValue(j + 1 > 9 ? 10 : j + 1);
                print(card->getValue());
                cards.push_back(card);
                elems.push_back(card);
            }
        }

        backCard->setSize(cardsSize);
        backCard->setPosition(backCardPosition);
        backCard->setColor(sf::Color::Red);
        elems.push_back(backCard);

        betButton->setSize(vec2f(100, 100/3));
        betButton->setInnerText("BET");
        betButton->setPosition((windowSize - betButton->getSize()) / 2.0f);
        betButton->setColor(sf::Color::Red);
        elems.push_back(betButton);

        hitButton->setSize(vec2f(100, 100/3));
        hitButton->setInnerText("HIT");
        hitButton->setPosition((windowSize - hitButton->getSize()) / 2.0f - vec2f(hitButton->getSize().x, 0));
        hitButton->setColor(sf::Color::Red);
        elems.push_back(hitButton);

        standButton->setSize(vec2f(100, 100/3));
        standButton->setInnerText("STAND");
        standButton->setPosition((windowSize - standButton->getSize()) / 2.0f + vec2f(standButton->getSize().x, 0));
        standButton->setColor(sf::Color::Red);
        elems.push_back(standButton);

        restartButton->setSize(vec2f(100, 100/3));
        restartButton->setInnerText("RESTART");
        restartButton->setPosition((windowSize - restartButton->getSize()) / 2.0f);
        restartButton->setColor(sf::Color::Green);
        elems.push_back(restartButton);

        moneyLabel->setSize(vec2f(100, 100 / 3));
        moneyLabel->setColor(sf::Color(255, 255, 255, 0));
        moneyLabel->setInnerText(std::to_string(money));
        moneyLabel->setValue(money);
        moneyLabel->setFontSize(24);
        elems.push_back(moneyLabel);

        player.setPosition((vec2f)getWindow().getSize() / 2.0f + vec2f(0, getWindow().getSize().y / 4.0f));
        dealer.setPosition((vec2f)getWindow().getSize() / 2.0f - vec2f(0, getWindow().getSize().y / 4.0f));
        
        restart();
    }

    void update() override
    {
        auto& window = getWindow();

        for(auto&& it : elems)
        {
            if(it->hidden())continue;
            it->draw(window);
        }

        for(auto&& it : player.chips)
        {
            if(it->hidden())continue;
            it->draw(window);
        }

        for(auto&& it : dealer.hand)
        {
            if(it->hidden())continue;
            if(it->isCardHidden())
            {
                backCard->setPosition(it->getPosition());
                backCard->draw(window);
                backCard->setPosition(backCardPosition);
                continue;
            }
            it->draw(window);
        }

        for(auto&& it : player.hand)
        {
            if(it->hidden())continue;
            it->draw(window);
        }
    }

    void mousePressed() override
    {
        vec2f mousePos(App::mouse.x, App::mouse.y);

        for(auto&& it : elems)
        {
            if(!it->active()) continue;
            if(isPointInRect(vec2f(App::mouse.x, App::mouse.y), it->getPosition(), it->getSize()))
            {
                if(it == betButton)
                {
                    betButtonCallback();
                }
                if(it == hitButton)
                {
                    hitCallback();
                }
                if(it == restartButton)
                {
                    restart();
                }

                for (auto&& tableChip : chips) {
                    if (it == tableChip) {
                        chipOnClickCallback(tableChip);
                        return;
                    }
                }
            }
        }

        int topClicked = -1;
        int count = 0;
        for(auto&& it : player.chips)
        {
            if(!it->active()) continue;
            if(isPointInRect(vec2f(App::mouse.x, App::mouse.y), it->getPosition(), it->getSize()))
            {
                topClicked = count;
            }
            count++;
        }

        if(topClicked != -1)
        {
            playerOnClickChipsCallback(player.chips[topClicked]);
            return;
        }
    }

    void restart()
    {
        // Set all elements to inactive and hidden
        for(auto&& it : elems)
        {
            it->setActive(false);
            it->setHidden(true);
        }

        // Show interactable chips
        for(auto&& it : chips)
        {
            it->setActive(true);
            it->setHidden(false);
        }

        // Return cards to deck and shuffle
        for(auto&& it : player.hand)        
        {
            cards.push_back(it);
        }
        for(auto&& it : dealer.hand)        
        {
            cards.push_back(it);
        }
        std::shuffle(cards.begin(), cards.end(), std::default_random_engine(std::random_device{}()));

        // Clear player and dealer hands and player chips
        player.chips.clear();
        player.hand.clear();
        dealer.hand.clear();

        // Show bet button, back card and money label
        betButton->setActive(true);
        betButton->setHidden(false);

        backCard->setActive(true);
        backCard->setHidden(false);

        moneyLabel->setActive(true);
        moneyLabel->setHidden(false);
    }

    void chipOnClickCallback(std::shared_ptr<Element> element)
    {
        std::shared_ptr<Element> plChip = std::make_shared<Element>(*element);
        player.chips.push_back(plChip);
        player.calcChipsLayout(getWindow());
        money -= plChip->getValue();
        player.setBetAmount(player.getBetAmount() + plChip->getValue());
        moneyLabel->setInnerText(std::to_string(money));
    }

    void playerOnClickChipsCallback(std::shared_ptr<Element> element)
    {
        player.chips.erase(std::remove(player.chips.begin(), player.chips.end(), element), player.chips.end());
        player.calcChipsLayout(getWindow());
        money += element->getValue();
        player.setBetAmount(player.getBetAmount() - element->getValue());
        moneyLabel->setInnerText(std::to_string(money));
    }

    void betButtonCallback()
    {
        if(player.getBetAmount() == 0) return;
        for(auto&& it : chips)
        {
            it->setActive(false);
            it->setHidden(true);
        }
        for(auto&& it : player.chips)
        {
            it->setActive(false);
        }
        betButton->setActive(false);
        betButton->setHidden(true);

        dealer.hand.push_back(std::move(cards.back()));
        cards.pop_back();
        dealer.hand.push_back(std::move(cards.back()));
        //dealer.hand.back()->setCardHidden(true);
        cards.pop_back();

        player.hand.push_back(std::move(cards.back()));
        cards.pop_back();
        player.hand.push_back(std::move(cards.back()));
        cards.pop_back();

        dealer.calcHandLayout(getWindow());
        player.calcHandLayout(getWindow());

        hitButton->setActive(true);
        hitButton->setHidden(false);

        standButton->setActive(true);
        standButton->setHidden(false);
    }

    void hitCallback()
    {
        // Move card from deck to player hand
        player.hand.push_back(std::move(cards.back()));
        cards.pop_back();

        // Recalculate player hand layout
        player.calcHandLayout(getWindow());

        money -= player.getBetAmount();
        // Double the bet amount and add chips to player then recalculate chips layout
        player.setBetAmount(player.getBetAmount() * 2);
    
        int plChipsSize = player.chips.size();
        for(int i = 0; i < plChipsSize; i++)
        {
            std::shared_ptr<Element> plChip = std::make_shared<Element>(*player.chips[i]);
            player.chips.push_back(plChip);
        }
        player.calcChipsLayout(getWindow());

        // Move card from deck to dealer hand
        dealer.hand.push_back(std::move(cards.back()));
        cards.pop_back();
        dealer.calcHandLayout(getWindow());

        int playerValue = player.getHandValue();
        int dealerValue = dealer.getHandValue();
        print(dealerValue);
        print(playerValue);

        // Win conditions
        if(playerValue == 21 || dealerValue > 21)
        {
            hitButton->setActive(false);
            standButton->setActive(false);

            restartButton->setActive(true);
            restartButton->setHidden(false);

            money += player.getBetAmount() * 2;
            print(money);
            print("win");
        }
        // Lose conditions
        else if(dealerValue == 21 || playerValue > 21)
        {
            hitButton->setActive(false);
            standButton->setActive(false);

            restartButton->setActive(true);
            restartButton->setHidden(false);

            print("lose");
        }

        player.setBetAmount(0);
        
        moneyLabel->setInnerText(std::to_string(money));
    }
};