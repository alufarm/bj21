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
	DISTRIB_STATE,
	ACTION_STATE,
	DEFEAT_STATE
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

	Card(sf::Vector2f position, sf::Vector2f size, std::string text, sf::Font& font, float type, float value) 
	: mType(type), mValue(value), hide(false)
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
		if(hide == false)
		{
			window.draw(mText);
		}
	}

	void SetText(std::string value)
	{
		mText.setString(value);
	}
	
	std::string GetText()
	{
		return std::string(mText.getString());
	}

	void Hide(){
		hide = true;
	}
	
	void Show(){
		hide = false;
	}

	Transform mTransform;

private:
	bool hide;
	sf::RectangleShape mShape;
	sf::Text mText;
};

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
		sf::Vector2f cardSize = hand.back()->mTransform.GetSize();
		float distance = cardSize.x + offsetX;
		float handWidth = hand.size() * distance - offsetX;
		float handX = handPosition.x - handWidth / 2;
		float i = 0;
		for(auto& it : hand)
		{
			it->mTransform.SetPosition(sf::Vector2(handX + i * distance, handPosition.y));
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
		float width = hand.size() * (hand.back()->mTransform.GetSize().x + offsetX) - offsetX;
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

static sf::Font font;

class Button {
public:

	Button() : mActive(true), mShape(new sf::RectangleShape()), 
		mText(new sf::Text()), mColor(sf::Color::White), mTag(""), mValue(0) 
	{
		mTransform.SetPosition(sf::Vector2f(0, 0));
		mTransform.SetSize(sf::Vector2f(20, 20));
		mText->setFont(font);
		
		mText->setCharacterSize(20);
		mText->setFillColor(sf::Color::Black);
		mText->setPosition(sf::Vector2f(0, 0));
	}

	Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font) : 
		mActive(true), mShape(new sf::RectangleShape()), 
		mText(new sf::Text()), mColor(sf::Color::White), mTag(""), mValue(0)
	{
		mTransform.SetPosition(position);
		mTransform.SetSize(size);

		mText->setCharacterSize(20);
		mText->setFont(font);
		mText->setFillColor(sf::Color::Black);
		mText->setPosition(sf::Vector2f(int(position.x), int(position.y)));
		
		
		
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
		SetColor(mColor);
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

class FinishWindow{

public:

	sf::Vector2f mPosition; 
	sf::Vector2f mSize;
	sf::Color mBgColor;
	Button button;


	FinishWindow() : mPosition(sf::Vector2f(0, 0)), mSize(sf::Vector2f(20, 20)), mBgColor(sf::Color::White)
	{
		init();
	}

	FinishWindow(sf::Vector2f position, sf::Vector2f size) : mPosition(position), mSize(size), mBgColor(sf::Color::White)
	{
		init();
	}
	
	void Draw(sf::RenderWindow& window)
	{
		window.draw(mShape);
		button.Draw(window);
		window.draw(mText);
		
	}

private:

	void init()
	{
		mShape.setPosition(mPosition);
		mShape.setSize(mSize);
		mShape.setFillColor(mBgColor);
		
		mText.setCharacterSize(20);
		mText.setFont(font);
		mText.setFillColor(sf::Color::Black);
		mText.setString("Loser");
		mText.setPosition(sf::Vector2f(
			int(mPosition.x + mSize.x / 2 - mText.getGlobalBounds().width / 2),
			int(mPosition.y + mSize.y / 4 + mSize.y / 8 - mText.getGlobalBounds().height / 2)
			)
		);
		
		button.SetText("Restart");
		button.SetTag("restart");
		button.mTransform.SetSize(sf::Vector2f(mSize.x / 4, mSize.y / 4));
		button.mTransform.SetPosition(sf::Vector2f(
			int(mPosition.x + mSize.x / 4 - button.mTransform.GetSize().x / 2), 
			int(mPosition.y + mSize.y -  mSize.y / 4 - button.mTransform.GetSize().y / 2)));
		button.SetColor(sf::Color(255, 1, 255));
		

	}

	sf::RectangleShape mShape;
	sf::Text mText;
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
						
						Button* button_hit = new Button(sf::Vector2f(px, player->GetHandPosition().y - 60), sf::Vector2f(40, 40), font);
						button_hit->SetValue(0);
						button_hit->SetText("HIT");
						button_hit->SetTag("hit");
						
						Button* button_stand = new Button(sf::Vector2f(px + 40 + 20, player->GetHandPosition().y - 60), sf::Vector2f(40, 40), font);
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
				cards.back()->mTransform.SetPosition(sf::Vector2f(50, 100));
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
