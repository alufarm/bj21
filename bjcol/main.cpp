#pragma once

#include "Utility/Utility.h"

enum TextAlign
{
    center
};

enum TextTransform
{
    uppercase
};

enum FontWeight
{
    bold
};

enum Position
{
    fixed
};

enum Display
{
    block
};

struct QStyle
{
    int margin;
    std::string font_family;
    TextAlign text_align;
    sf::Color color;
    TextTransform text_transform;
    int font_size;
    FontWeight font_weight;
    int padding_top;
    Position position;
    float width;
    float height;
    float bottom;
    Display display;
};

class QElement
{
public:

    QElement(QElement* parent_ = nullptr) : 
        parent(parent_),
        name("button"), innerText("button"), isActive(true), isHidden(false)
    {
        vec2f lastPosition;
        vec2f lastSize;
        if(parent)
        {
            print("HASPARENT");
            if(!parent->childs.empty())
            {
                lastPosition = parent->childs.back()->getPosition();
                lastSize = parent->childs.back()->getSize();
                lastPosition.y += lastSize.y;
            }
            else
            {
                lastPosition = parent->getPosition();
                lastSize = parent->getSize();
            }
            parent->childs.push_back(this);
        }else print("NOPARENT");
        println(lastPosition);
        println(lastSize);
        setPosition(lastPosition.x, lastPosition.y);
        setSize(lastSize.x, lastSize.y);

        text.setFont(g_standardFont);
        text.setCharacterSize(g_standardFontSize);
        text.setString(innerText);
        text.setFillColor(sf::Color::Black);
    }

    void setTexture(const sf::Texture& texture_)
    {
        sprite.setTexture(texture_);
    }

    void setTexture(const sf::Texture& texture_, const sf::IntRect& rect_)
    {
        sprite.setTexture(texture_);
        sprite.setTextureRect(rect_);
    }

    void setPosition(const float& x_, const float& y_)
    {
        shape.setPosition(x_, y_);
        sprite.setPosition(x_, y_);
        text.setPosition(x_, y_);
    }

    void setPosition(const vec2f& position_)
    {
        shape.setPosition(position_);
        sprite.setPosition(position_);
        text.setPosition(position_);
    }

    void setSize(const float& x_, const float& y_)
    {
       shape.setSize(vec2f(x_, y_));
       sprite.setScale(x_ / (float)sprite.getTextureRect().width, y_ / (float)sprite.getTextureRect().height);
    }

    void setSize(const vec2f& size_)
    {
       shape.setSize(size_);
       sprite.setScale(vec2f(size_.x / (float)sprite.getTextureRect().width, size_.y / (float)sprite.getTextureRect().height));
    }

    void setColor(const sf::Color& color_)
    {
        shape.setFillColor(color_);
    }

    void setStyle(const QStyle& style)
    {

    }

    void setTextAlignment(TextAlign textAlign)
    {
        switch(textAlign)
        {
            case TextAlign::center:
            {
                visitAll(
                [this](QElement* it)
                {
                    print(it->name);
                    vec2f position = it->getPosition();
                    vec2f size = it->getSize();
                    vec2f textSize = vec2f(it->text.getGlobalBounds().width, it->text.getGlobalBounds().height);
                    it->text.setPosition(position.x + size.x / 2 - textSize.x / 2,
                                         position.y + size.y / 2 - textSize.y / 2);
                });

                break;
            }
        }
    }

    void setInnerText(std::string innerText_)
    {
        text.setString(innerText_);
    }

    void setName(std::string name_)
    {
        name = name_;
    }

    const vec2f& getPosition() const
    {
        return shape.getPosition();
    }

    const vec2f& getSize() const
    {
        return shape.getSize();
    }

    const QElement* findElementByName(const std::string name_)
    {
        visitAll(
        [&name_](QElement* it)
        {
            if(it->name == name_)return it;
        });

        return this;
    }

    void visitAll(std::function<void(QElement*)> f)
    {
        std::stack<QElement*> stack;
        stack.push(this);

        std::stack<int> indices;
        indices.push(0);

        f(stack.top());

        while(!stack.empty())
        {
            int currIndex = indices.top();
            if(currIndex < stack.top()->childs.size())
            {
                indices.top()++;
                stack.push(stack.top()->childs[currIndex]);
                indices.push(0);

                f(stack.top());
            }
            else
            {
                stack.pop();
                indices.pop();
            }

        }
    }

    void draw(sf::RenderWindow& window_)
    {
        sf::RectangleShape dshape;
        visitAll(
        [&window_, &dshape](QElement* it)
        {
            //it->setPosition(100, 0);
            //println(it->getPosition());
            //println(it->getSize());
            println(it->name);
            
            dshape.setFillColor(sf::Color::Blue);
            dshape.setPosition(it->text.getGlobalBounds().left, it->text.getGlobalBounds().top);
            dshape.setSize(vec2f(it->text.getGlobalBounds().width, it->text.getGlobalBounds().height));

            
            window_.draw(it->shape);
            window_.draw(dshape);
            window_.draw(it->sprite);
            window_.draw(it->text);
            
        });
    }

private:

    QElement* parent;
    std::vector<QElement*> childs;

    std::string name;
    std::string innerText;

    sf::Text text;

    sf::Sprite sprite;
    sf::RectangleShape shape;

    bool isActive;
    bool isHidden;
};
