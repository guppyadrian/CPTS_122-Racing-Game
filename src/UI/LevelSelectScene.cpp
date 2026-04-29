//
// Created by guppy on 4/28/26.
//

#include "UI/LevelSelectScene.hpp"

#include "LevelLoader.hpp"
#include "flow/Renderer.hpp"
#include "flow/SceneManager.hpp"

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
    {
        flow::SceneManager::getGlobal().loadScene(LevelLoader().readFile(_nextLevelPath));
        flow::SceneManager::getGlobal().switchScene(_nextLevelPath);
    }
    
}

void LevelSelectScene::draw()
{
    _window.setView(_window.getDefaultView()); // idk bro
    
    _window.clear();
    sf::Text text(*_font, _levels[_levelSelected]);
    text.setPosition({_window.getSize().x / 2.f, _window.getSize().y - 150.f});
    text.setCharacterSize(100);
    const sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.getCenter());
    
    _window.draw(text);
    _window.draw(_thumbnails[_levelSelected]);
    
    _window.display();
}

void LevelSelectScene::onEnter()
{
    _font = std::make_unique<sf::Font>();
    if (!_font->openFromFile("assets/Pixel-Regular.ttf")) { // Load a font
        throw std::runtime_error("Could not load font: assets/Pixel-Regular.ttf");
    }
    
    // load levels
    _levels.clear();
    std::filesystem::path folder = "assets/levels";
    
    for (const auto& lvlPath : std::filesystem::directory_iterator(folder))
    {
        if (!lvlPath.is_regular_file()) continue;
        _levels.push_back(lvlPath.path().stem().string());
        
        const auto lvl = LevelLoader().readFile(_levels.back());
        _thumbnailTextures.push_back(std::make_unique<sf::Texture>(flow::Renderer::getGlobalRenderer().generateThumbnail(*lvl)));
        auto& sprite = _thumbnails.emplace_back(*_thumbnailTextures.back());
        sprite.setScale({0.7f, 0.7f});
        sprite.setOrigin(sprite.getLocalBounds().getCenter());
        sprite.setPosition({_window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 100.0f});
    }
    
    UIScene::onEnter();
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
