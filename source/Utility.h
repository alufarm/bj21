#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics.hpp>

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

#endif