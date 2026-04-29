//
// Created by guppy on 4/27/26.
//

#pragma once
#include <memory>

#include "flow/Scene.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Sprite.hpp"

class UIScene : public flow::Scene
{
protected:
    sf::RenderWindow& _window;
public:
    explicit UIScene(const std::string& sceneUUID, sf::RenderWindow& window) : Scene(sceneUUID), _window(window) {}

    void initialize() override {}
    void update(float dt) override {}
    void draw() override {}
    void onEnter() override { _window.setView(_window.getDefaultView()); }
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
                return {0.0f, 1.0f};
            default:
                return {0.0f, 0.0f};
        }
    }
};
