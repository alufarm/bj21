


#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
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

struct RectShape{
	
	void SetPosition(sf::Vector2f position){
		shape.setPosition(position);
		collision = shape.getGlobalBounds();
	}
	
	void SetSize(sf::Vector2f size){
		shape.setSize(size);
	}
	
	sf::FloatRect GetGlobalBounds(){
		return shape.getGlobalBounds();
	}
	
	bool CheckCollision(sf::Vector2i point){
		if(point.x > collision.left && point.x < collision.left + collision.width
			&& point.y > collision.top && point.y < collision.top + collision.height){
			return true;
		}
		return false;
	}
	
private:
	sf::RectangleShape shape;
	sf::FloatRect collision;
	bool onCollision;
};

struct Image{
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	
	void Load(std::string path){
		if (!(image.loadFromFile(path)))
			std::cout << "Cannot load image";
		texture.loadFromImage(image);
		sprite.setTexture(texture);  
	}
};

enum STATE{
	START_STATE,
	DISTRIB_STATE
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

private:
	float betAmount;
	std::string tag;
};

std::string ToPresisionString(float value, int digits){
	
 	std::ostringstream oss;
	oss << std::fixed << std::setprecision(digits) << value;
	return oss.str();
}

class IDrawable{
public:
	virtual void Draw(sf::RenderWindow&)=0;
};

struct BoxCollider{
	float x1, y1;
	float x2, y2;
};

void print(BoxCollider& box){
	print(std::to_string(box.x1) + ", " + std::to_string(box.y1) + ", " + std::to_string(box.x2) + ", " + std::to_string(box.y2));
}

struct PointCollider{
	float x, y;
};

bool BoxVsPoint(BoxCollider& box, sf::Vector2i& point){
	if(point.x > box.x1 && point.x < box.x2 &&
           point.y > box.y1 && point.y < box.y2){
		return true;
	}
	return false;
}

bool BoxVsPoint(sf::FloatRect& rect, sf::Vector2i& point){
	if(point.x > rect.left && point.x < rect.left + rect.width &&
           point.y > rect.top && point.y < rect.top + rect.height){
		return true;
	}
	return false;
}

class ICollidable{
public:

	virtual void OnCollisionEnter()=0;
	virtual void OnCollisionExit()=0;
};

class Button {
public:
	Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font) : 
		mPosition(position), mSize(size), 
		mActive(true), mShape(new sf::RectangleShape()), 
		mText(new sf::Text()), mColor(sf::Color::White)
	{
		mText->setFont(font);
		mText->setCharacterSize(20);
		mText->setFillColor(sf::Color::Black);
		mText->setPosition(position);
		
		SetPosition(mPosition);
		SetSize(mSize);
		SetColor(mColor);
	}
	
	void SetPosition(sf::Vector2f pos) {
		mPosition = pos;
		mShape->setPosition(mPosition);
		mText->setPosition(mPosition);
	}

	sf::Vector2f GetPosition(){
		return mPosition;
	}

	void SetSize(sf::Vector2f value) {
		mSize = value;
		mShape->setSize(mSize);
	}

	sf::Vector2f GetSize(){
		return mSize;
	}
	
	void SetText(std::string value){
		mText->setString(value);
	}

	sf::Text* GetText(){
		return mText;
	}

	void Draw(sf::RenderWindow& window){

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

private:
	bool mActive;

	sf::Vector2f mSize;
	sf::Vector2f mPosition;
	sf::RectangleShape* mShape;
	sf::Color mColor;

	sf::Text* mText;
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

	std::string score = ToPresisionString(scoreValue, 2) + " $";

	sf::Font font;
	if(!font.loadFromFile("Zepter_Pro.ttf")){
		print("ERROR: font loading!");
	}
	
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setString(score);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Black);
	//scoreText.setPosition();

	std::string betContent = "Your bet: ";
	sf::Text betText;
	betText.setFont(font);
	betText.setString(betContent);
	betText.setCharacterSize(20);
	betText.setFillColor(sf::Color::Black);
	betText.setPosition(sf::Vector2f(width/2, 0));

	// BUTTONS
	Button* button_bet_1 = new Button(sf::Vector2f(width/2, height/2+60), sf::Vector2f(40, 40), font);
	button_bet_1->SetText("10%");
	
	Button* button_bet_2 = new Button(sf::Vector2f(width/2+60, height/2+60), sf::Vector2f(40, 40), font);
	button_bet_2->SetText("40%");
	//

	std::vector<Button*> buttons;
	buttons.push_back(button_bet_1);
	buttons.push_back(button_bet_2);

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

				break;
			case DISTRIB_STATE:

				break;
		}


		for(Button*& button : buttons)
		{
			if(button->GetActive())
			{
				sf::FloatRect rect = button->GetGlobalBounds();

				if(BoxVsPoint(rect, mouseMove))
				{
					button->OnCollisionEnter();
					if(mouseLeftDownFirst)
					{
						for(Button*& button : buttons)
						{
							button->SetActive(false);
						}
						break;
					}
				}
				else{
					button->OnCollisionExit();
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
		window.draw(scoreText);
		//sh->Draw(window);

		window.display();

		mouseLeftDownFirst = false;
	}
	
	return 0;
}
