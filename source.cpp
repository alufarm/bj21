



#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct Timer{
	Timer(): time(std::chrono::high_resolution_clock::now()){}
	
	double elapsed(){
		double elapsed = (std::chrono::high_resolution_clock::now() - time).count();
		time = std::chrono::high_resolution_clock::now();
		return elapsed / 1e9;
	}
	
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> time;
};

template<typename T>
void print(T value){
	std::cout << value << std::endl;
}

void print(sf::FloatRect fRect){
	print(std::to_string(fRect.left) + ", " + std::to_string(fRect.top) + ", " + std::to_string(fRect.width) + ", " + std::to_string(fRect.height));
}

void print(sf::Vector2i iVect){
	print(std::to_string(iVect.x) + ", " + std::to_string(iVect.y));
}

void print(sf::Vector2f fVect){
	print(std::to_string(fVect.x) + ", " + std::to_string(fVect.y));
}

void print(sf::Color color){
	print(std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b));
}

bool checkCollision(sf::FloatRect fRect, sf::Vector2i point){
	if(point.x > fRect.left && point.x < fRect.left + fRect.width
		&& point.y > fRect.top && point.y < fRect.top + fRect.height){
		return true;
	}
	return false;
}


float interpolate(float a, float b, float p){
	return a + (b-a) * p;
}

enum STATE{
	START_STATE,
	DISTRIB_STATE
};

struct Transform{

	Transform() : mPosition(sf::Vector2f(0, 0)), mSize(sf::Vector2f(0, 0)) {}

	void SetPosition(sf::Vector2f position)
	{
		mPosition = position;
	}

	void SetSize(sf::Vector2f size)
	{
		mSize = size;
	}

	sf::Vector2f GetPosition()
	{
		return mPosition;
	}

	sf::Vector2f GetSize()
	{
		return mSize;
	}

private:

	sf::Vector2f mPosition;
	sf::Vector2f mSize;
};

struct Card{
	int mType;
	int mValue;

	Card(sf::Vector2f position, sf::Vector2f size, std::string text, sf::Font font, float type, float value) : mType(type), mValue(value)
	{
		mTransform.SetPosition(position);
		mTransform.SetSize(size);

		mText.setFont(font);
		mText.setCharacterSize(20);
		mText.setString(text);
		mText.setFillColor(sf::Color::Black);
		mText.setPosition(position);
	}

	void Draw(sf::RenderWindow& window)
	{
		mShape.setPosition(mTransform.GetPosition());
		mShape.setSize(mTransform.GetSize());
		mText.setPosition(mTransform.GetPosition());

		window.draw(mShape);
	}

	Transform mTransform;

private:

	sf::RectangleShape mShape;
	sf::Text mText;
};

class Player{
public:
	Player() : betAmount(0), tag("") {}
	
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
		hand.push_back(card);
	}

	void AdjustCards(float width, float height){
		float centerX = width / 2;
		sf::Vector2f cardSize = hand.back()->mTransform.GetSize();
		float offsetX = 20;
		float offsetY = 20;
		float distance = cardSize.x + offsetX;
		float handWidth = hand.size() * distance;
		float handX = centerX - handWidth / 2;
		float handY = height - cardSize.y - offsetY;
		float i = 0;
		for(auto& it : hand)
		{
			it->mTransform.SetPosition(sf::Vector2(handX + i * distance, handY));
			i++;
		}
	}

	void Draw(sf::RenderWindow& window){
		for(auto& it : hand){
			it->Draw(window);
		}
	}

private:
	float betAmount;
	std::string tag;
	std::vector<Card*> hand;
};

std::string ToPresisionString(float value, int digits){
	
 	std::ostringstream oss;
	oss << std::fixed << std::setprecision(digits) << value;
	return oss.str();
}

bool BoxVsPoint(sf::FloatRect& rect, sf::Vector2i& point){
	if(point.x > rect.left && point.x < rect.left + rect.width &&
           point.y > rect.top && point.y < rect.top + rect.height){
		return true;
	}
	return false;
}

class Button {
public:
	Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font) : 
		mActive(true), mShape(new sf::RectangleShape()), 
		mText(new sf::Text()), mColor(sf::Color::White), mTag(""), mValue(0)
	{
		mTransform.SetPosition(position);
		mTransform.SetSize(size);

		mText->setFont(font);
		mText->setCharacterSize(20);
		mText->setFillColor(sf::Color::Black);
		mText->setPosition(position);
		
		SetColor(mColor);
	}

	
	void SetText(std::string value){
		mText->setString(value);
	}

	sf::Text* GetText(){
		return mText;
	}

	void Draw(sf::RenderWindow& window){

		mShape->setPosition(mTransform.GetPosition());
		mShape->setSize(mTransform.GetSize());
		mText->setPosition(mTransform.GetPosition());

		window.draw(*mShape);
		window.draw(*mText);
	}

	void OnCollisionEnter(){
		SetColor(sf::Color(255, 0, 0));
	}

	void OnCollisionExit(){
		SetColor(sf::Color::White);
	}

	void SetColor(sf::Color color){
		mShape->setFillColor(color);
	}

	sf::FloatRect GetGlobalBounds(){
		return mShape->getGlobalBounds();
	}

	void SetActive(bool value){
		mActive = value;
	}

	bool GetActive(){
		return mActive;
	}

	std::string GetTag(){
		return mTag;
	} 

	void SetTag(std::string tag){
		mTag = tag;
	}

	float GetValue(){
		return mValue;
	} 

	void SetValue(float value){
		mValue = value;
	}

	Transform mTransform;

private:
	bool mActive;

	sf::RectangleShape* mShape;
	sf::Color mColor;

	sf::Text* mText;
	std::string mTag;
	float mValue;
};


int main(){
	int width = 800, height = 600;

	STATE state = STATE::START_STATE;
	Player* player = new Player();
	Player* dealer = new Player();
	std::vector<Player*> players;
	players.push_back(player);
	players.push_back(dealer);
	
	int current_player_id = 0;
	float scoreValue = 100.0f;

	sf::Font font;
	if(!font.loadFromFile("Zepter_Pro.ttf")){
		print("ERROR: font loading!");
	}
	
	std::string score = ToPresisionString(scoreValue, 2) + " $";
	sf::Text scoreText;
	scoreText.setPosition(sf::Vector2f(0, 0));
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setString(score);
	
	//scoreText.setPosition();

	std::vector<Button*> buttons;
	bool isStartInit = false;
	bool isDistribInit = false;

	std::vector<Card*> cards;

	sf::Vector2f deckPosition(100, 200);
	sf::Vector2f cardSize(100, 180);
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			Card* card = new Card(deckPosition, cardSize, std::to_string(i+2), font, j, i+2);
			cards.push_back(card);
		}

		int lastValue = cards.back()->mValue;

		Card* cardJ = new Card(deckPosition, cardSize, "J", font, j, lastValue);
		cards.push_back(cardJ);

		Card* cardQ = new Card(deckPosition, cardSize, "Q", font, j, lastValue);
		cards.push_back(cardQ);

		Card* cardK = new Card(deckPosition, cardSize, "K", font, j, lastValue);
		cards.push_back(cardK);

		Card* cardA = new Card(deckPosition, cardSize, "A", font, j, 1);
		cards.push_back(cardA);
	}

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

		window.clear(sf::Color::Cyan);		

		switch(state)
		{
			case START_STATE:
				//print("START_STATE");

				//InitState()
				if(isStartInit == false)
				{
					print("InitStart");
					//DeletePreviosState()
					for(auto it : buttons)
					{
						delete it;
					}
					buttons.clear();

					// BUTTONS
					Button* button_bet_1 = new Button(sf::Vector2f(width/2, height/2+60), sf::Vector2f(40, 40), font);
					button_bet_1->SetValue(0.1f);
					button_bet_1->SetText(std::to_string(int(button_bet_1->GetValue() * 100)) + "%");
					button_bet_1->SetTag("btbet");
					
					Button* button_bet_2 = new Button(sf::Vector2f(width/2+60, height/2+60), sf::Vector2f(40, 40), font);
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

					std::random_device rd;
    				std::mt19937 g(rd());
					std::shuffle(cards.begin(), cards.end(), g);

					for(auto& it : players)
					{
						it->AddToHand(cards.back());
						//delete cards.back();
						cards.pop_back();
						it->AddToHand(cards.back());
						//delete cards.back();
						cards.pop_back();
						
						it->AdjustCards(width, height);
					}

					isDistribInit = true;
				}
				break;
		}


		for(Button*& button : buttons)
		{
			sf::FloatRect rect = button->GetGlobalBounds();

			std::string tag = button->GetTag();

			if(BoxVsPoint(rect, mouseMove))
			{
				if(mouseLeftDownFirst){

					if(tag == "btbet")
					{
						scoreValue -= scoreValue * button->GetValue();
						state = STATE::DISTRIB_STATE;
						score = ToPresisionString(scoreValue, 2) + " $";
						scoreText.setString(score);
					}
					

				}
				button->OnCollisionEnter();
			}
			else{
				button->OnCollisionExit();
			}
			

		}

		for(Button*& button : buttons)
		{
			if(button->GetActive())
			{
				button->Draw(window);
			}
		}

		cards.back()->mTransform.SetPosition(sf::Vector2f(50, height / 2));
		cards.back()->Draw(window);
		

		window.draw(scoreText);
		window.display();

		mouseLeftDownFirst = false;
	}
	
	return 0;
}
