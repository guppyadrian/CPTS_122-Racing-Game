//
// Created by guppy on 4/28/26.
//

#include "UI/LevelSelectScene.hpp"

#include "LevelLoader.hpp"
#include "flow/Renderer.hpp"
#include "flow/SceneManager.hpp"
#include <flow/MusicManager.hpp>

#include "Multiplayer.hpp"
#include "PlayerData.hpp"
#include "UI/MenuScene.hpp"

void LevelSelectScene::initialize()
{
    _hasInitialized = true;
    
    loadFont();
    
    // show loading screen
    loadingDraw();

    PlayerData::getInstance().readSaveData();
    
    // load levels
    _levels.clear();
    _thumbnails.clear();
    _thumbnailTextures.clear();
    _levelPaths.clear();
    std::filesystem::path folder = "assets/levels";
    
    for (const auto& lvlPath : std::filesystem::directory_iterator(folder))
    {
        if (!lvlPath.is_regular_file()) continue;
        _levelPaths.push_back(lvlPath.path().stem().string());
        const auto lvl = LevelLoader().readFile(lvlPath.path().stem().string(), true);
        _levels.push_back(lvl->get_uuid());
        _thumbnailTextures.push_back(std::make_unique<sf::Texture>(flow::Renderer::getGlobalRenderer().generateThumbnail(*lvl)));
        auto& sprite = _thumbnails.emplace_back(*_thumbnailTextures.back());
        sprite.setScale({0.7f, 0.7f});
        sprite.setOrigin(sprite.getLocalBounds().getCenter());
        sprite.setPosition({_window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 100.0f});
    }
}

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
    
    if (_queueNextLevel)
    {
        if (Multiplayer::getInstance().inMultiplayer)
        {
            Multiplayer::getInstance().trackSelected = _levelPaths[_levelSelected];
            flow::SceneManager::getGlobal().switchScene("multiplayer-lobby", false);
        }
        else
        {
            flow::SceneManager::getGlobal().loadScene(LevelLoader().readFile(_levelPaths[_levelSelected]));
            flow::SceneManager::getGlobal().switchScene(_levelPaths[_levelSelected], false);
        }
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

    const long long bestTime = PlayerData::getInstance().getLevelTime(_levelPaths[_levelSelected]);
    int tSec = bestTime / 1000;
    int tMs = bestTime % 1000;

    if (bestTime == 0)
    {
        drawText("Best time: None!", {0, _window.getSize().y / -2.1f}, 40);
    }
    else
    {
        drawText("Best time: " + std::to_string(tSec) + ":" + std::to_string(tMs), {0, _window.getSize().y / -2.1f}, 40);
    }
}

void LevelSelectScene::loadingDraw()
{
    _window.clear();
    try
    {
        drawText("Loading...");
    }
    catch (std::runtime_error ec)
    {
        std::cerr << "Failed to draw text: " << ec.what() << std::endl;
    }
    _window.display();
}

void LevelSelectScene::onEnter()
{
    UIScene::onEnter();
    if (!_hasInitialized) initialize();
    
    _queueNextLevel = false;

    flow::audio::MusicManager::getGlobal().play("01 Opening Theme.mp3");
    flow::audio::MusicManager::getGlobal().setVolume(40);
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
        _queueNextLevel = true;
    }
    else if (inputVector.y < 0)
    {
        
        if (Multiplayer::getInstance().inMultiplayer)
        {
            flow::SceneManager::getGlobal().switchScene("multiplayer-lobby", false);
        } 
        else
        {
            flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(_window));
            flow::SceneManager::getGlobal().switchScene("menu", false);
        }
        
    }
}
