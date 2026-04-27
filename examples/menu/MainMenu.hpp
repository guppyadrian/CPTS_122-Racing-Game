#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

#include <flow/Scene.hpp>

class MainMenu : public flow::Scene
{
    public:
    MainMenu();
    void initialize() override;
    void update(float dt) override;
    void onEnter() override;
    void drawMenu(sf::RenderWindow& window);
    
    private:
    int _curMenuPosition;

    sf::Font _headerFont;
    sf::Font _buttonFont;

    sf::RectangleShape _playButton;
    sf::RectangleShape _exitButton;

    std::unique_ptr<sf::Text> _headerText;
    std::unique_ptr<sf::Text> _playText;
    std::unique_ptr<sf::Text> _exitText;
};