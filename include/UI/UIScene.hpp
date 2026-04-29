//
// Created by guppy on 4/27/26.
//

#pragma once
#include <memory>

#include "flow/Scene.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class UIScene : public flow::Scene
{
protected:
    sf::RenderWindow& _window;
    std::unique_ptr<sf::Font> _font;
public:
    explicit UIScene(const std::string& sceneUUID, sf::RenderWindow& window) : Scene(sceneUUID), _window(window) {}

    void initialize() override {}
    void update(float dt) override {}
    void draw() override {}
    void onEnter() override { _window.setView(_window.getDefaultView()); _window.pollEvent(); }
    void onExit() override {}
protected:
    static sf::Vector2f getInputVector(const sf::Keyboard::Key key)
    {
        switch (key)
        {
            case sf::Keyboard::Key::Right:
            case sf::Keyboard::Key::D:
                return {1.0f, 0.0f};
            case sf::Keyboard::Key::A:
            case sf::Keyboard::Key::Left:
                return {-1.0f, 0.0f};
            case sf::Keyboard::Key::Space:
            case sf::Keyboard::Key::Enter:
                return {0.0f, 1.0f};
            case sf::Keyboard::Key::Escape:
                return {0.0f, -1.0f};
            default:
                return {0.0f, 0.0f};
        }
    }
    
    void loadFont()
    {
        _font = std::make_unique<sf::Font>();
        if (!_font->openFromFile("assets/Pixel-Regular.ttf")) { // Load a font
            throw std::runtime_error("Could not load font: assets/Pixel-Regular.ttf");
        }
    }
    
    // position is relative to center
    void drawText(const std::string& textInput, const sf::Vector2f position = {0.0f, 0.0f}, const int size = 70)
    {
        if (_font == nullptr) loadFont();
        sf::Text text(*_font, textInput);
        text.setCharacterSize(size);
        text.setOrigin(text.getLocalBounds().getCenter());
        text.setPosition((sf::Vector2f(_window.getSize()) / 2.0f) + position);
        _window.draw(text);
    }
};
