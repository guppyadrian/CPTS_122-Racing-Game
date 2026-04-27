#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

#include <flow/Scene.hpp>

#include "flow/Renderable.hpp"
#include "MenuDrawable.hpp"

class MainMenu : public flow::Scene, flow::Renderable
{
    public:
    MainMenu();
    void initialize() override;
    void init() override {}; // Dont use!!
    void update(float dt) override;
    void fixedUpdate() override {}; // Dont use!!
    void onEnter() override;
    void onExit() override;
    //void drawMenu(sf::RenderWindow& window) {};
    const sf::Drawable& getDrawable() override;


    
    private:
    int _curMenuPosition;
    std::unique_ptr<MenuDrawable> _menuDrawable;

    sf::Font _headerFont;
    sf::Font _buttonFont;

    sf::RectangleShape _playButton;
    sf::RectangleShape _exitButton;

    std::unique_ptr<sf::Text> _headerText;
    std::unique_ptr<sf::Text> _playText;
    std::unique_ptr<sf::Text> _exitText;
};
