#ifndef FINISHWINDOW_H
#define FINISHWINDOW_H

#include "Button.h"

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
		button.SetSize(sf::Vector2f(mSize.x / 4, mSize.y / 4));
		button.SetPosition(sf::Vector2f(
			int(mPosition.x + mSize.x / 4 - button.GetSize().x / 2), 
			int(mPosition.y + mSize.y -  mSize.y / 4 - button.GetSize().y / 2)));
		button.SetColor(sf::Color(255, 1, 255));
		

	}

	sf::RectangleShape mShape;
	sf::Text mText;
};

#endif