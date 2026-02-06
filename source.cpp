

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

class ICollidable{
public:
	BoxCollider box;
	virtual void OnCollisionEnter()=0;
	virtual void OnCollisionExit()=0;

	BoxCollider& GetCollider(){
		return box;
	}
};

class Button : public IDrawable, public ICollidable{
public:
	Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font) : 
		position(position), size(size), 
		isActive(false), shape(new sf::RectangleShape()), 
		text(new sf::Text()), value(0), color(sf::Color::White)
	{
		text->setFont(font);
		text->setCharacterSize(20);
		text->setFillColor(sf::Color::Black);
		text->setPosition(position);
		ResetBox();
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

	void SetValue(float val){
		value = val;
	}

	float GetValue(){
		return value;
	}

	virtual void Draw(sf::RenderWindow& window){
		window.draw(*shape);
		window.draw(*text);
	}

	virtual void OnCollisionEnter(){
		print("Enter");
		SetPosition(sf::Vector2f(90, 90));
		SetColor(sf::Color(255, 0, 0));
	}

	virtual void OnCollisionExit(){
		print("Exit");
		SetColor(sf::Color::White);
		
	}

	void SetColor(sf::Color color){
		shape->setFillColor(color);
	}

 	bool isActive;
private:

	void ResetBox(){
		box.x1 = position.x;
		box.y1 = position.y;
		box.x2 = position.x + size.x;
		box.y2 = position.y + size.y;
	}

	sf::Vector2f size;
	sf::Vector2f position;
	sf::RectangleShape* shape;
	sf::Color color;	

	sf::Text* text;
	float value;
};



int main(){
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

	int width = 800, height = 600;

	// BUTTONS
	Button* button_bet_1 = new Button(sf::Vector2f(width/2, height/2+60), sf::Vector2f(40, 40), font);
	button_bet_1->isActive = true;
	button_bet_1->SetText("10%");
	button_bet_1->SetValue(0.1f);
	
	Button* button_bet_2 = new Button(sf::Vector2f(width/2+60, height/2+60), sf::Vector2f(40, 40), font);
	button_bet_2->isActive = true;
	button_bet_2->SetText("40%");
	button_bet_2->SetValue(0.4f);
	//

	std::vector<Button*> buttons;
	buttons.push_back(button_bet_1);
	buttons.push_back(button_bet_2);

	std::vector<IDrawable*> drawableObjects;
	drawableObjects.push_back(button_bet_1);
	drawableObjects.push_back(button_bet_2);

	std::vector<ICollidable*> collidableObjects;
	collidableObjects.push_back(button_bet_1);
	collidableObjects.push_back(button_bet_2);

	sf::RenderWindow window(sf::VideoMode(width, height), "21");
	
	sf::Vector2i mouseMove;
	bool mouseLeftDown;
	bool mouseLeftDownFirst;

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

		switch(state){
			case START_STATE:
				//print("START_STATE");
				
				for(auto& button : buttons) {
					sf::Vector2f position = button->GetPosition();
					sf::Vector2f size = button->GetSize();
					sf::FloatRect collider(position.x, position.y, size.x, size.y);
					if(checkCollision(collider, mouseMove)){
						//button->GetShape()->setFillColor(sf::Color(50, 200, 30));
						if(mouseLeftDownFirst){
							scoreValue -= scoreValue * button->GetValue();
							score = ToPresisionString(scoreValue, 2) + " $";
							scoreText.setString(score);
							//button->GetShape()->setFillColor(sf::Color(50, 60, 200));
						}
					}
					else{
						//button->GetShape()->setFillColor(sf::Color(255, 255, 255));
					}
				}

				break;
			case DISTRIB_STATE:

				break;
		}


		for(ICollidable*& object : collidableObjects){
			BoxCollider box = object->GetCollider();

			if(BoxVsPoint(box, mouseMove)){
				object->OnCollisionEnter();
			}
			else{
				object->OnCollisionExit();
			}
		}

		for(IDrawable*& object : drawableObjects){
			object->Draw(window);
		}
		window.draw(scoreText);

		window.display();

		mouseLeftDownFirst = false;
	}
	
	return 0;
}
