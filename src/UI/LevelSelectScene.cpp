//
// Created by guppy on 4/28/26.
//

#include "UI/LevelSelectScene.hpp"

#include "LevelLoader.hpp"

void LevelSelectScene::update(float dt)
{
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
            break;
        }
        if (event->is<sf::Event::KeyPressed>())
        {
            const auto keyPressed = event->getIf<sf::Event::KeyPressed>()->code;
            // TODO: controller support
            
            handleInput(getInputVector(keyPressed));
        }
    }
    
    if (!_nextLevelPath.empty())
        LevelLoader().readFile(_nextLevelPath);
}

void LevelSelectScene::draw()
{
    _window.clear();
    sf::Text text(*_font, _levels[_levelSelected]);
    text.setPosition({_window.getSize().x / 2.f, _window.getSize().y - 150.f});
    text.setCharacterSize(100);
    const sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.getCenter());
    
    _window.draw(text);
    
    _window.display();
}

void LevelSelectScene::onEnter()
{
    UIScene::onEnter();
    
    _font = std::make_unique<sf::Font>();
    if (!_font->openFromFile("assets/Pixel-Regular.ttf")) { // Load a font
        throw std::runtime_error("Could not load font: assets/Pixel-Regular.ttf");
    }
    
    // load levels
    _levels.clear();
    std::filesystem::path folder = "assets/levels";
    
    for (const auto& lvl : std::filesystem::directory_iterator(folder))
    {
        if (!lvl.is_regular_file()) continue;
        _levels.push_back(lvl.path().stem().string());
    } 
}

void LevelSelectScene::handleInput(const sf::Vector2f inputVector)
{
    if (inputVector.x > 0)
    {
        _levelSelected++;
        if (_levelSelected >= _levels.size()) _levelSelected = 0;
    }
    else if (inputVector.x < 0)
    {
        _levelSelected--;
        if (_levelSelected < 0) _levelSelected = _levels.size() - 1;
    }
    else if (inputVector.y > 0)
    {
        _nextLevelPath = _levels[_levelSelected];
    }
}
