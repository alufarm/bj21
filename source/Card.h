#ifndef CARD_H
#define CARD_H

#include <SFML/Graphics.hpp>

struct Card{
	int mType;
	int mValue;

	Card(sf::Vector2f position, sf::Vector2f size, std::string text, sf::Font& font, float type, float value) 
	: mType(type), mValue(value), hide(false)
	{
		SetPosition(position);
        SetSize(size);

		mText.setFont(font);
		mText.setCharacterSize(20);
		mText.setString(text);
		mText.setFillColor(sf::Color::Black);
		mText.setPosition(position);
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(mShape);
		if(hide == false)
		{
			window.draw(mText);
		}
	}

    void SetPosition(sf::Vector2f position){
		mShape.setPosition(sf::Vector2f(int(position.x), int(position.y)));
		mText.setPosition(sf::Vector2f(int(position.x), int(position.y)));
	}

	sf::Vector2f GetPosition(){
		return mShape.getPosition();
	}

	void SetSize(sf::Vector2f size){
		mShape.setSize(size);
	}

    sf::Vector2f GetSize(){
        return mShape.getSize();
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

private:
	bool hide;
	sf::RectangleShape mShape;
	sf::Text mText;
};

#endif