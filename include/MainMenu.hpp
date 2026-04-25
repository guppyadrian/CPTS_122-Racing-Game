#pragma once

#include <SFML/Graphics.hpp>

class MainMenu
{
    public:
    void drawMenu();
    
    private:
    int _menuPosition;
    //format for a new button
    //sf::RectangleShape

    sf::RectangleShape header;
    sf::RectangleShape play;
    sf::RectangleShape exit;

    sf::Text headerText;
    sf::Text playText;
    sf::Text exitText;
};

