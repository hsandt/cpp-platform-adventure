#include "UI/UIWidgetText.h"

#include <iostream>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

template class std::unique_ptr<sf::Text>;

// Game
#include "Components/Transform.h"

UIWidgetText::UIWidgetText() :
    mc_transform(std::make_unique<Transform>()),
    mc_text(std::make_unique<sf::Text>()),
    mp_text()
{
    // mc_text->scale({2.f, 2.f});
}

// Even empty, destructor definition in .cpp required if only class-forwarding
// some smart pointer contained types in .h
UIWidgetText::~UIWidgetText()
{
}

void UIWidgetText::render(sf::RenderTarget& renderTarget)
{
    // just for testing we load every frame, but of course we should preload
    // and store the font (it must live as text keeps a pointer to it)
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/open-sans/OpenSans-Regular.ttf"))
    {
        std::cout << "FAILED LOADING FONT" << std::endl;
        return;
    }

    mc_text->setFont(font);
    mc_text->setString(mp_text);
    mc_text->setCharacterSize(11);
    mc_text->setFillColor(sf::Color::White);

    // convert custom Transform component to SFML Transform
    sf::Transform sfTransform;
    sfTransform.translate(mc_transform->position.x, mc_transform->position.y);
    renderTarget.draw(*mc_text, sfTransform);

    // ! font will be destroyed and mc_text now points to invalid data
    // this doesn't crash only because we reset the font each frame
}
