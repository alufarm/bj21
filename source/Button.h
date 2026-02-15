#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

static sf::Font font;

class Button {
public:

	Button() : mActive(true), mShape(new sf::RectangleShape()), 
		mText(new sf::Text()), mTag(""), mValue(0)
	{
		SetPosition(sf::Vector2f(0, 0));
        SetSize(sf::Vector2f(40, 40));
		
		mText->setFont(font);
		mText->setCharacterSize(20);
		mText->setFillColor(sf::Color::Black);
	}

	Button(sf::Vector2f position, sf::Vector2f size) : 
		mActive(true), mShape(new sf::RectangleShape()), 
		mText(new sf::Text()), mTag(""), mValue(0)
	{
		SetPosition(position);
        SetSize(size);

		mText->setCharacterSize(20);
		mText->setFont(font);
		mText->setFillColor(sf::Color::Black);
		
		SetColor(sf::Color::White);
	}

	void SetText(std::string value){
		mText->setString(value);
	}

	sf::Text* GetText(){
		return mText;
	}

	void SetPosition(sf::Vector2f position){
		mShape->setPosition(sf::Vector2f(int(position.x), int(position.y)));
		mText->setPosition(sf::Vector2f(int(position.x), int(position.y)));
	}

	sf::Vector2f GetPosition(){
		return mShape->getPosition();
	}

	void SetSize(sf::Vector2f size){
		mShape->setSize(size);
	}

    sf::Vector2f GetSize(){
        return mShape->getSize();
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

private:
	bool mActive;

	sf::RectangleShape* mShape;

	sf::Text* mText;
	std::string mTag;
	float mValue;
};

#endif