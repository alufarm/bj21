#include <iostream>
#include <string>
#include <random>
#include <SFML/Window.hpp>

#include "Timer.h"
#include "Utility.h" 
#include "Button.h"
#include "Player.h"
#include "FinishWindow.h"

enum STATE{
	START_STATE,
	DISTRIB_STATE,
	ACTION_STATE,
	DEFEAT_STATE
};

int main(){
	int width = 800, height = 600;

	STATE state = STATE::START_STATE;
	
	Player* player = new Player();
	Player* dealer = new Player();
	
	player->SetTag("player");
	dealer->SetTag("dealer");
	
	std::vector<Player*> players;
	players.push_back(player);
	players.push_back(dealer);
	
	int current_player_id = 0;
	float money = 100.0f;

	if(!font.loadFromFile("Zepter_Pro.ttf")){
		print("ERROR: font loading!");
	}
	
	std::string score = ToPresisionString(money, 2) + " $";
	sf::Text scoreText;
	scoreText.setPosition(sf::Vector2f(0, 0));
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setString(score);
	
	//scoreText.setPosition();

	std::vector<Button*> buttons;
	bool isStartInit = false;
	bool isDistribInit = false;
	bool isActionInit = false;
	bool isDefeatInit = false;

	std::vector<Card*> cards;

	sf::Vector2f deckPosition(100, 200);
	sf::Vector2f cardSize(100, 180);
	
	std::vector<std::string> types = {"Spades", "Hearts", "Diamonds", "Clubes"};

	FinishWindow finishWindow(sf::Vector2f(width/2-0.3f*width/2, height/2-0.3f*height/2), sf::Vector2f(0.3f * width, 0.3f * height));
	bool isOverlay = false;

	sf::RenderWindow window(sf::VideoMode(width, height), "21");
	
	sf::Vector2i mouseMove;
	bool mouseLeftDown;
	bool mouseLeftDownFirst;
	
	double targetDurationTime = 1.0;
	double durationAccumTime = 0;
	int frames = 0;
	
	double deltaTime = 0;
	
	Timer frameTimer;

	sf::Event event;
	while(window.isOpen()){
		deltaTime = frameTimer.elapsed();
		
		frames++;
		durationAccumTime += deltaTime;
		if(durationAccumTime > targetDurationTime){
			print(frames);
			frames = 0;
			durationAccumTime = 0;

		}


		
		while(window.pollEvent(event)){
			
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;
				
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			
			if(event.type == sf::Event::MouseMoved){
				mouseMove.x = event.mouseMove.x;
				mouseMove.y = event.mouseMove.y;
			}
			
			if(event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left){
					mouseLeftDown = true;
					mouseLeftDownFirst = true;
				}
			}
			if(event.type == sf::Event::MouseButtonReleased){
				if (event.mouseButton.button == sf::Mouse::Left){
					mouseLeftDown = false;
				}
			}
			
		}

		window.clear(sf::Color(255, 200, 85));		

		switch(state)
		{
			case START_STATE:
				//print("START_STATE");
				
				//InitState()
				if(isStartInit == false)
				{
					print("InitStart");
					//DeletePreviosState()

					buttons.clear();

					cards.clear();
					
					for(int j = 0; j < 4; j++)
					{
						for(int i = 0; i < 9; i++)
						{
							Card* card = new Card(deckPosition, cardSize, std::to_string(i+2) + "\n" + types[j], font, j, i+2);
							cards.push_back(card);
						}
						
						int lastValue = cards.back()->mValue;

						Card* cardJ = new Card(deckPosition, cardSize, "J\n" + types[j], font, j, lastValue);
						cards.push_back(cardJ);

						Card* cardQ = new Card(deckPosition, cardSize, "Q\n" + types[j], font, j, lastValue);
						cards.push_back(cardQ);

						Card* cardK = new Card(deckPosition, cardSize, "K\n" + types[j], font, j, lastValue);
						cards.push_back(cardK);

						Card* cardA = new Card(deckPosition, cardSize, "A\n" + types[j], font, j, 1);
						cards.push_back(cardA);
					}
					
					// BUTTONS
					Button* button_bet_1 = new Button(sf::Vector2f(width/2, height/2+60), sf::Vector2f(40, 40));
					button_bet_1->SetValue(0.1f);
					button_bet_1->SetText(std::to_string(int(button_bet_1->GetValue() * 100)) + "%");
					button_bet_1->SetTag("btbet");
					
					Button* button_bet_2 = new Button(sf::Vector2f(width/2+60, height/2+60), sf::Vector2f(40, 40));
					button_bet_2->SetValue(0.4f);
					button_bet_2->SetText(std::to_string(int(button_bet_2->GetValue() * 100)) + "%");
					button_bet_2->SetTag("btbet");

					buttons.push_back(button_bet_1);
					buttons.push_back(button_bet_2);
					
					//

					isStartInit = true;
				}
				break;
			case DISTRIB_STATE:
				if(isDistribInit == false)
				{
					print("InitDistrib");
					//DeletePreviosState()
					for(auto it : buttons)
					{
						delete it;
					}
					buttons.clear();

					// InitRandom()
					std::random_device rd;
    				std::mt19937 g(rd());
					//
					
					std::shuffle(cards.begin(), cards.end(), g);

					for(auto& it : players)
					{

						it->AddToHand(cards.back());
						//delete cards.back();
						cards.pop_back();
						it->AddToHand(cards.back());
						//delete cards.back();

						if(it->GetTag() == "dealer")
						{
							print("deal");
							it->SetHandPosition(sf::Vector2f(width / 2, 20));
							cards.back()->Hide();
						}
						cards.pop_back();
						
						if(it->GetTag() == "player")
						{
							it->SetHandPosition(sf::Vector2f(width / 2, height - cardSize.y - 20));
						}
						
						it->AdjustCards();
					}

					isDistribInit = true;
					state = STATE::ACTION_STATE;
				}
				break;
			case ACTION_STATE:
				if(player->GetPoints() < 21 || dealer->GetPoints() < 21){
					if(isActionInit == false)
					{
						float px = player->GetHandPosition().x + player->GetHandWidth();
						
						Button* button_hit = new Button(sf::Vector2f(px, player->GetHandPosition().y - 60), sf::Vector2f(40, 40));
						button_hit->SetValue(0);
						button_hit->SetText("HIT");
						button_hit->SetTag("hit");
						
						Button* button_stand = new Button(sf::Vector2f(px + 40 + 20, player->GetHandPosition().y - 60), sf::Vector2f(40, 40));
						button_stand->SetValue(0);
						button_stand->SetText("STAND");
						button_stand->SetTag("stand");
						
						buttons.push_back(button_hit);
						buttons.push_back(button_stand);
						
						isActionInit = true;
					}
					
					
				}
				break;
			case DEFEAT_STATE:
				if(isDefeatInit == false)
				{
					Button* restartButton = &finishWindow.button;
					buttons.push_back(restartButton);
					
					isDefeatInit = true;
				}
				break;
		}


		for(Button*& button : buttons)
		{
			sf::FloatRect rect = button->GetGlobalBounds();

			std::string tag = button->GetTag();
			button->OnCollisionExit();
			if(isOverlay == false){
				
				
				
				if(BoxVsPoint(rect, mouseMove))
				{
					if(mouseLeftDownFirst){
						
						if(tag == "btbet")
						{
							money -= money * button->GetValue();
							state = STATE::DISTRIB_STATE;
							score = ToPresisionString(money, 2) + " $";
							scoreText.setString(score);
						}
						
						if(tag == "hit")
						{
							if(cards.size() != 0){
								player->AddToHand(cards.back());
								cards.pop_back();
								print(player->GetPoints());
								player->AdjustCards();
							}
							
							if(player->GetPoints() > 21)
							{
								isOverlay = true;
								state = STATE::DEFEAT_STATE;
							}
						}
						
						if(tag == "stand")
						{
							
						}
						

						
					}
					button->OnCollisionEnter();
				}
			}
			else{
				if(BoxVsPoint(rect, mouseMove))
				{
					if(mouseLeftDownFirst){
						if(tag == "restart")
						{
							print("restart");
							isStartInit = false;
							isDistribInit = false;
							isActionInit = false;
							isDefeatInit = false;
							
							state = STATE::START_STATE;
							isOverlay = false;
							
							for(auto& it : players){
								it->ClearHand();
							}
						}
					}
					
				}
				
			}
			

		}

		for(Button*& button : buttons)
		{
			if(button->GetActive())
			{
				button->Draw(window);
			}
		}
		
		for(auto& it : players)
		{
			it->Draw(window);
		}
		
		if(state == STATE::DISTRIB_STATE || state == STATE::ACTION_STATE)
		{
			if(cards.size() != 0){
				cards.back()->SetPosition(sf::Vector2f(50, 100));
				cards.back()->Hide();
				cards.back()->Draw(window);
			}
		}
		
		
		window.draw(scoreText);
		if(isOverlay)finishWindow.Draw(window);
		window.display();

		mouseLeftDownFirst = false;
	}
	
	return 0;
}