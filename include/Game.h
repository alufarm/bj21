#pragma once

#include "Player.h"

struct Transform
{
    vec2f position;
    vec2f size;
};

class Animation
{
public:

    Animation(std::shared_ptr<Element> target_) : target(target_) 
    {

    }

    void play()
    {
        isPlaying = true;
    }

    void update(float dt)
    {
       // print("Animate cycle");
        if (!isPlaying)
            return;
       
        if (currentFrame == transforms.size())
        {
            isPlaying = false;
            currentFrame = 0;
            accumulator = 0;
            return;
        }

        if(accumulator >= timePoints[currentFrame])
        {
            target->setPosition(transforms[currentFrame].position);
            ++currentFrame;
        }

        accumulator += dt;

        //print("after Animate cycle");
    }

    Transform getFrame()
    {
        return transforms[currentFrame];
    }

    void addFrame(float timePoint, Transform transform)
    {
        transforms.push_back(transform);
        timePoints.push_back(timePoint);
    }

private:
    bool isPlaying = false;
    int currentFrame = 0;
    std::vector<Transform> transforms;
    std::vector<float> timePoints;
    float duration = 0;
    float accumulator = 0;
    std::shared_ptr<Element> target;
};

class Game : public App
{
private:
    enum GameState
    {
        PlayerTurn,

    };

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

    std::vector<std::shared_ptr<Element>> elems;

    Player player;
    Dealer dealer;

    std::vector<std::string> values = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "VALET", "LADY", "KING"};
    std::vector<std::string> types = {"DIAMONDS", "SPADE", "HEARTS", "CLUBS"};

    std::shared_ptr<Animation> chipAnimation = std::make_shared<Animation>(backCard);

    std::vector<std::shared_ptr<Animation>> animations;

    void start() override
    {
        generateChips();
        generateCards();
        generateUI();

        player.setPosition((vec2f)getWindow().getSize() / 2.0f + vec2f(0, getWindow().getSize().y / 4.0f));
        dealer.setPosition((vec2f)getWindow().getSize() / 2.0f - vec2f(0, getWindow().getSize().y / 4.0f));

        player.setMoneyLabel(moneyLabel);

        player.updateMoneyLabel();

        const int totalFrames = 900;
const float totalDuration = 15.0f;
const float timeStep = totalDuration / totalFrames; // ~0.016667 секунды

for (int i = 1; i <= totalFrames; ++i)
{
    float t = i * timeStep; // Текущее время кадра
    
    // Нормализованное время от 0.0 до 1.0 для вычисления траектории
    float progress = t / totalDuration; 

    // --- Математический расчет сложной плавной траектории (S-образная дуга) ---
    float x = 0.0f;
    float y = 0.0f;

    if (progress < 0.4f) // Первая фаза: движение вправо с изгибом
    {
        float p = progress / 0.4f;
        x = p * 104.0f;
        y = (1.0f - cos(p * 3.14159f)) * 25.0f;
    }
    else if (progress < 0.7f) // Вторая фаза: резкий уход вниз
    {
        float p = (progress - 0.4f) / 0.3f;
        x = 104.0f + sin(p * 1.57079f) * 16.0f;
        y = 50.0f + p * 314.0f;
    }
    else // Третья фаза: плавный возврат налево и торможение
    {
        float p = (progress - 0.7f) / 0.3f;
        x = 120.0f - p * 119.19f;
        y = 364.0f + sin(p * 1.57079f) * 5.21f;
    }

    // Добавляем сгенерированный кадр в систему анимации
    chipAnimation->addFrame(t, Transform{vec2f(x, y), vec2f(50.0f, 50.0f)});
}

        animations.push_back(chipAnimation);
        chipAnimation->play();

        print("init");

        restart();

        print("start");
    }

    void update() override
    {
        auto &window = getWindow();
        
        for(auto&& it : animations)
        {
            
            it->update(getDeltaTime());
        }

        drawAll(window);
        drawPlayerChips(window);
        drawDealerHand(window);
        drawPlayerHand(window);
    }

    void mousePressed() override
    {
        vec2f mousePos(App::mouse.x, App::mouse.y);

        for (auto &&it : elems)
        {
            if (!it->active())
                continue;
            if (isPointInRect(vec2f(App::mouse.x, App::mouse.y), it->getPosition(), it->getSize()))
            {
                if (it == betButton)
                {
                    betButtonCallback();
                }
                if (it == hitButton)
                {
                    hitCallback();
                }
                if (it == restartButton)
                {
                    restart();
                }
                if (it == standButton)
                {
                    print("STAND");
                    standCallback();
                }

                for (auto &&tableChip : chips)
                {
                    if (it == tableChip)
                    {
                        chipOnClickCallback(tableChip);
                        return;
                    }
                }
            }
        }

        int topClicked = -1;
        int count = 0;
        for (auto &&it : player.chips)
        {
            if (!it->active())
                continue;
            if (isPointInRect(vec2f(App::mouse.x, App::mouse.y), it->getPosition(), it->getSize()))
            {
                topClicked = count;
            }
            count++;
        }

        if (topClicked != -1)
        {
            playerOnClickChipsCallback(player.chips[topClicked]);
            return;
        }
    }

    void restart()
    {
        // Set all elements to inactive and hidden
        for (auto &&it : elems)
        {
            it->hide();
        }

        // Show interactable chips
        for (auto &&it : chips)
        {
            it->show();
        }

        // Return cards to deck and shuffle
        for (auto &&it : player.hand)
        {
            cards.push_back(it);
        }
        for (auto &&it : dealer.hand)
        {
            cards.push_back(it);
        }
        std::shuffle(cards.begin(), cards.end(), std::default_random_engine(std::random_device{}()));

        // Clear player and dealer hands and player chips
        player.setBetAmount(0);
        player.chips.clear();
        player.hand.clear();
        dealer.hand.clear();

        // Show bet button, back card and money label
        betButton->show();
        backCard->show();
        moneyLabel->show();
    }

    void chipOnClickCallback(std::shared_ptr<Element> element)
    {
        player.takeBet(element->getValue());
        player.chips.push_back(deepCopy(element));
        player.calcChipsLayout(getWindow());
    }

    void playerOnClickChipsCallback(std::shared_ptr<Element> element)
    {
        player.returnBet(element->getValue());

        player.chips.erase(std::remove(player.chips.begin(), player.chips.end(), element), player.chips.end());
        player.calcChipsLayout(getWindow());
    }

    bool isStandWin()
    {
        int dealerValue = dealer.getHandValue();
        int playerValue = player.getHandValue();

        return playerValue >= dealerValue;
    }

    void win()
    {
        int dealerValue = dealer.getHandValue();
        int playerValue = player.getHandValue();

        if (player.isBlackjack())
        {
            // Get money
            player.setMoney(player.getMoney() + player.getBetAmount() * 1.5f);
            player.updateMoneyLabel();
            // Blackjack label
            hitButton->setActive(false);
            standButton->setActive(false);
            restartButton->show();
            return;
        }

        if (dealer.isBlackjack())
        {
            // Blackjack label
            hitButton->setActive(false);
            standButton->setActive(false);
            restartButton->show();
            return;
        }

        if (dealerValue > 21)
        {
            // Get money
            player.setMoney(player.getMoney() + player.getBetAmount() * 1.5f);
            player.updateMoneyLabel();
            hitButton->setActive(false);
            standButton->setActive(false);
            restartButton->show();
            return;
        }
        if (playerValue > 21)
        {
            // Bust label
            hitButton->setActive(false);
            standButton->setActive(false);
            restartButton->show();
            return;
        }
    }

    void betButtonCallback()
    {
        if (player.getBetAmount() == 0)
            return;

        hideChips();

        player.deactivateChips();

        dealer.takeCard(cards);
        dealer.takeCard(cards);
        dealer.hand.back()->setCardHidden(true);

        player.takeCard(cards);
        player.takeCard(cards);

        dealer.calcHandLayout(getWindow());
        player.calcHandLayout(getWindow());

        betButton->hide();
        hitButton->show();
        standButton->show();

        win();
    }

    void hitCallback()
    {
        // Move card from deck to player hand
        player.takeCard(cards);

        player.calcHandLayout(getWindow());

        dealer.hand.back()->setCardHidden(false);

        int dealerValue = dealer.getHandValue();
        if (dealerValue < 17)
        {
            dealer.takeCard(cards);
            dealer.calcHandLayout(getWindow());
        }

        win();
    }

    void standCallback()
    {
        int dealerValue = dealer.getHandValue();
        if (dealerValue < 17)
        {
            dealer.takeCard(cards);
            dealer.calcHandLayout(getWindow());
        }

        win();
        if (isStandWin())
        {
            player.setMoney(player.getMoney() + player.getBetAmount() * 1.5f);
            player.updateMoneyLabel();
            hitButton->setActive(false);
            standButton->setActive(false);
            restartButton->show();
        }
        else
        {
            hitButton->setActive(false);
            standButton->setActive(false);
            restartButton->show();
        }
    }

    void hideChips()
    {
        for (auto &&it : chips)
        {
            it->hide();
        }
    }

    void generateCards();
    void generateChips();
    void generateUI();

    void drawAll(sf::RenderWindow &window);
    void drawPlayerChips(sf::RenderWindow &window);
    void drawDealerHand(sf::RenderWindow &window);
    void drawPlayerHand(sf::RenderWindow &window);
};

void Game::generateCards()
{
    for (int i = 0; i < cardsCount / 13 && i < types.size(); i++)
    {
        for (int j = 0; j < cardsCount / 4 && j < values.size(); j++)
        {
            std::shared_ptr<Element> card = std::make_shared<Element>();
            card->setSize(cardsSize);
            card->setInnerText(values[j] + "\n" + types[i]);
            card->setPosition(vec2f(j * cardsSize.x, i * cardsSize.y));
            card->setColor(getRandomColor());
            card->setValue(j + 1 > 9 ? 10 : j + 1);
            cards.push_back(card);
            elems.push_back(card);
        }
    }
    cards[0]->setValue(11);

    backCard->setSize(cardsSize);
    backCard->setPosition(backCardPosition);
    backCard->setColor(sf::Color::Red);
    elems.push_back(backCard);
}

void Game::generateChips()
{
    for (int i = 0; i < chipsCount; i++)
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
}

void Game::generateUI()
{
    betButton->setSize(vec2f(100, 100 / 3));
    betButton->setInnerText("BET");
    betButton->setPosition((windowSize - betButton->getSize()) / 2.0f);
    betButton->setColor(sf::Color::Red);
    elems.push_back(betButton);

    hitButton->setSize(vec2f(100, 100 / 3));
    hitButton->setInnerText("HIT");
    hitButton->setPosition((windowSize - hitButton->getSize()) / 2.0f - vec2f(hitButton->getSize().x, 0));
    hitButton->setColor(sf::Color::Red);
    elems.push_back(hitButton);

    standButton->setSize(vec2f(100, 100 / 3));
    standButton->setInnerText("STAND");
    standButton->setPosition((windowSize - standButton->getSize()) / 2.0f + vec2f(standButton->getSize().x, 0));
    standButton->setColor(sf::Color::Red);
    elems.push_back(standButton);

    restartButton->setSize(vec2f(100, 100 / 3));
    restartButton->setInnerText("RESTART");
    restartButton->setPosition((windowSize - restartButton->getSize()) / 2.0f);
    restartButton->setColor(sf::Color::Green);
    elems.push_back(restartButton);

    moneyLabel->setSize(vec2f(100, 100 / 3));
    moneyLabel->setColor(sf::Color(255, 255, 255, 0));
    moneyLabel->setFontSize(24);
    elems.push_back(moneyLabel);
}

void Game::drawAll(sf::RenderWindow &window)
{
    for (auto &&it : elems)
    {
        if (it->hidden())
            continue;
        it->draw(window);
    }
}

void Game::drawPlayerChips(sf::RenderWindow &window)
{

    for (auto &&it : player.chips)
    {
        if (it->hidden())
            continue;
        it->draw(window);
    }
}

void Game::drawDealerHand(sf::RenderWindow &window)
{
    for (auto &&it : dealer.hand)
    {
        if (it->hidden())
            continue;
        if (it->isCardHidden())
        {
            backCard->setPosition(it->getPosition());
            backCard->draw(window);
            backCard->setPosition(backCardPosition);
            continue;
        }
        it->draw(window);
    }
}

void Game::drawPlayerHand(sf::RenderWindow &window)
{
    for (auto &&it : player.hand)
    {
        if (it->hidden())
            continue;
        it->draw(window);
    }
}
