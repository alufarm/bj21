
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
	START_STATE
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

class Button{
public:
	Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font) : 
		position(position), size(size), 
		isActive(false), shape(new sf::RectangleShape()), 
		text(new sf::Text()) 
	{
		text->setFont(font);
		text->setCharacterSize(20);
		text->setFillColor(sf::Color::Black);
		text->setPosition(position);
	}
	
	void SetPosition(sf::Vector2f position) {
		position = position;
		text->setPosition(position);
	}

	sf::Vector2f GetPosition(){
		return position;
	}

	void SetSize(sf::Vector2f size) {
		size = size;
	}

	sf::Vector2f GetSize(){
		return size;
	}

	sf::RectangleShape* GetShape(){

		shape->setSize(sf::Vector2f(size.x, size.y));
		shape->setPosition(sf::Vector2f(position.x, position.y));
		return shape;
	}
	
	void SetText(std::string newText){
		text->setString(newText);
	}

	sf::Text* GetText(){
		return text;
	}

 	bool isActive;
private:
	sf::Vector2f size;
	sf::Vector2f position;
	sf::RectangleShape* shape;

	sf::Text* text;
};

int main(){
	STATE state = STATE::START_STATE;
	Player* player = new Player();
	Player* dealer = new Player();
	std::vector<Player*> players;
	players.push_back(player);
	players.push_back(dealer);
	
	int current_player_id = 0;
	float scoreValue = 327.328;

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

	int width = 800, height = 600;

	// BUTTONS
	Button* button_bet_1 = new Button(sf::Vector2f(width/2, height/2+60), sf::Vector2f(40, 40), font);
	button_bet_1->isActive = true;
	button_bet_1->SetText("10%");
	
	Button* button_bet_2 = new Button(sf::Vector2f(width/2+60, height/2+60), sf::Vector2f(40, 40), font);
	button_bet_2->isActive = true;
	button_bet_2->SetText("40%");
	//

	std::vector<Button*> buttons;
	buttons.push_back(button_bet_1);
	buttons.push_back(button_bet_2);

	sf::RenderWindow window(sf::VideoMode(width, height), "21");
	
	sf::Vector2i mouseMove;
	bool mouseLeftDown;

	sf::Vector2f cardSize(110, 180);
	sf::RectangleShape card(cardSize);
	card.setPosition(width / 2 - cardSize.x / 2, height / 2 - cardSize.y / 2);
	card.setSize(cardSize);
	bool cardEnter = false;
	sf::FloatRect cardCollision = card.getGlobalBounds();
	
	float riseAmount = 50;
	
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
			//print(frames);
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
				
				card.setPosition(width / 2 - cardSize.x / 2, height / 2 - cardSize.y / 2);
				card.setSize(cardSize);
				
				if(cardEnter){
					cardCollision = card.getGlobalBounds();
					cardCollision.height+=50;
				}
				else
					cardCollision = card.getGlobalBounds();
			}
			
			if(event.type == sf::Event::MouseMoved){
				mouseMove.x = event.mouseMove.x;
				mouseMove.y = event.mouseMove.y;
			}
			
			if(event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left){
					mouseLeftDown = true;
				}
			}
			if(event.type == sf::Event::MouseButtonReleased){
				if (event.mouseButton.button == sf::Mouse::Left){
					mouseLeftDown = false;
				}
			}
			
		}

		window.clear(sf::Color::Cyan);		

		switch(state){
			case START_STATE:
				//print("START_STATE");
				
				for(auto& button : buttons) {
					sf::Vector2f position = button->GetPosition();
					sf::Vector2f size = button->GetSize();
					sf::FloatRect collider(position.x, position.y, size.x, size.y);
					if(checkCollision(collider, mouseMove)){
						button->GetShape()->setFillColor(sf::Color(50, 200, 30));
						if(mouseLeftDown){
							button->GetShape()->setFillColor(sf::Color(50, 60, 200));
							
						}
					}
					else{
						button->GetShape()->setFillColor(sf::Color(255, 255, 255));
					}
				}

				break;
					
		}



		for(auto& button : buttons){
			window.draw(*(button->GetShape()));
			window.draw(*(button->GetText()));
		}
		window.draw(scoreText);		

		window.display();
	}
	
	return 0;
}
