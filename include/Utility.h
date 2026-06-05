#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <stack>
#include <random>
#include <memory>
#include "SFML/Graphics.hpp"

typedef sf::Vector2f vec2f;
typedef sf::Vector2u vec2u;
typedef sf::Color vec4;

template<typename T>
inline void print(T val)
{
    std::cout << val << std::endl;
}

inline void print(vec2f val)
{
    std::cout << "[ " << val.x << ", " << val.y << "]" << std::endl;
}

inline void print(vec2u val)
{
    std::cout << "[ " << val.x << ", " << val.y << "]" << std::endl;
}

template <typename T, typename ...Args>
inline void print(T first, Args... args)
{
    print(first);
    print(args...);
}

inline bool isPointInRect(vec2f point, vec2f position, vec2f size)
{
    return point.x > position.x && point.x < position.x + size.x &&
           point.y > position.y && point.y < position.y + size.y;
}

#include <chrono>
using namespace std::chrono;

class Timer {
public:
	Timer() : start(high_resolution_clock::now()) {}

	double elapsed() {
		double elapsed = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
		start = high_resolution_clock::now();
		return elapsed;
	}
private:
	high_resolution_clock::time_point start;
};

inline int getRandomInt(const int& min, const int& max)
{
	// 1. Seed the generator with a non-deterministic random device
    std::random_device rd; 
    // 2. Initialize the Mersenne Twister engine with the seed
    std::mt19937 gen(rd()); 
    // 3. Define the range [min, max] (inclusive)
    std::uniform_int_distribution<> distr(min, max);

	return distr(gen);
}

inline float getRandomFloat(const float& min, const float& max)
{
	// 1. Seed the generator with a non-deterministic random device
    std::random_device rd; 
    // 2. Initialize the Mersenne Twister engine with the seed
    std::mt19937 gen(rd()); 
    // 3. Define the range [min, max) (inclusive)
    std::uniform_real_distribution<float> distr(min, max);

	return distr(gen);
}

inline float getRandom()
{
	// 1. Seed the generator with a non-deterministic random device
    std::random_device rd; 
    // 2. Initialize the Mersenne Twister engine with the seed
    std::mt19937 gen(rd()); 
    // 3. Define the range [min, max) (inclusive)
    std::uniform_real_distribution<float> distr(0.0f, 1.0001f);

	return distr(gen);
}

inline sf::Color getRandomColor()
{
    int r = getRandomInt(0, 255);
    int g = getRandomInt(0, 255);
    int b = getRandomInt(0, 255);
    return sf::Color(r, g, b);
}

template<class T>
std::shared_ptr<T> deepCopy(std::shared_ptr<T> ptr)
{
    return std::make_shared<T>(*ptr);;
}