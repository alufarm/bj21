#ifndef PLAYER_H
#define PLAYER_H

#include "Utility.h"
#include "Card.h"

class Player{
public:
	Player() : betAmount(0), tag(""), points(0), offsetX(20) {}
	
	void SetBet(float value) {
		betAmount = value;
	}

	void SetTag(std::string name) {
		tag = name;
	}

	std::string GetTag(){
		return tag;
	}

	void AddToHand(Card* card){
		card->Show();
		hand.push_back(card);
		
		float sum = 0;
		for(auto& it : hand){
			sum += it->mValue;
		}
		points = sum;
	}

	void AdjustCards(){
		sf::Vector2f cardSize = hand.back()->GetSize();
		float distance = cardSize.x + offsetX;
		float handWidth = hand.size() * distance - offsetX;
		float handX = handPosition.x - handWidth / 2;
		float i = 0;
		for(auto& it : hand)
		{
			it->SetPosition(sf::Vector2(handX + i * distance, handPosition.y));
			i++;
		}
		print(handPosition);
	}

	void SetHandPosition(sf::Vector2f position){
		handPosition = position;
	}
	
	sf::Vector2f GetHandPosition(){
		return handPosition;
	}
	
	float GetHandWidth(){
		float width = hand.size() * (hand.back()->GetSize().x + offsetX) - offsetX;
		return width;
	}

	void Draw(sf::RenderWindow& window){
		for(auto& it : hand){
			it->Draw(window);
		}
	}
	
	float GetPoints(){
		return points;
	}
	
	void ClearHand(){
		hand.clear();
	}
	
private:
	
	float points;
	float offsetX;
	sf::Vector2f handPosition;
	float betAmount;
	std::string tag;
	std::vector<Card*> hand;
};

#endif