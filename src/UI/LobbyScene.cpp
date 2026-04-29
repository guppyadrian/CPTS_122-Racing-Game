//
// Created by guppy on 4/29/26.
//

#include "UI/LobbyScene.hpp"

#include <fstream>

#include "LevelLoader.hpp"
#include "Multiplayer.hpp"
#include "flow/NetworkManager.hpp"
#include "flow/SceneManager.hpp"
#include "network/NetworkClient.hpp"
#include "UI/MenuScene.hpp"

LobbyScene::LobbyScene(sf::RenderWindow& window, const State state) : UIScene("multiplayer-lobby", window), _state(state)
{
    _buttons.add("menu/playButton.png", {  0, 0}); // host
    _buttons.add("menu/playButton.png", {200, 0}); // join
    _buttons.add("menu/quitButton.png", {400, 0}); // quit
}

LobbyScene::~LobbyScene()
{
    if (_state == State::Hosting)
    {
        auto& server = flow::NetworkManager::getGlobal().getServer();
        server.stop();
    }
    auto& client = flow::NetworkManager::getGlobal().getClient();
    client.disconnect();
}

void LobbyScene::initialize()
{
    _hasInitialized = true;
    
    loadFont();
    
    if (_state == State::Hosting)
    {
        auto& server = flow::NetworkManager::getGlobal().getServer();
        server.listen(25550);
        server.onConnection = [this, &server](gp::network::Socket socket) mutable // TODO: handledisconnection
        {
            int id = _idAccumulator++;
            socket->on("handshakePlayerReady", [id, socket]()
            {
                socket->emit("handshakePlayerID", id);
            });
            socket->on<std::array<float, 3>>("plyr", [&server, id](std::array<float, 3> position)
            {
                gp::network::ByteBuffer buffer; // TODO: all players should be combined into one packet btw
                
                // turn everytting into bytes yk?
                const auto idBytes = reinterpret_cast<const uint8_t*>(&id);
                buffer.insert(buffer.end(), idBytes, idBytes + sizeof(id));
                const auto dataBytes = reinterpret_cast<const uint8_t*>(position.data());
                buffer.insert(buffer.end(), dataBytes, dataBytes + position.size() * sizeof(float));
                
                server.emit("plyr", buffer);
            });
        };
    }
    
    // ok so even if you are hosting, for simplicity, you also join as a client to ur own server
    auto& client = flow::NetworkManager::getGlobal().getClient();
    
    client.on("connection", [this, &client]() mutable // TODO: trigger once
    {  
        _connected = true;
        client.emit("handshakePlayerReady", '\0');
        std::cout << "Connected to lobby" << std::endl;
        client.off("connection");
    });
    
    client.on<std::string>("start-game", [](const std::string &levelPath)
    {
        asio::post(flow::NetworkManager::getGlobal().io(), [levelPath]()
        {
            flow::SceneManager::getGlobal().loadScene(LevelLoader().readFile(levelPath));
            flow::SceneManager::getGlobal().switchScene(levelPath, false);
        });
        flow::NetworkManager::getGlobal().io().restart();
    });
    
    client.on<int>("handshakePlayerID", [](const int id)
    {
        Multiplayer::getInstance().id = id;
    });
    
    client.on("disconnect", [this]() mutable
    {
        _connected = false;
        asio::post(flow::NetworkManager::getGlobal().io(), [this]()
        {
            flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(_window));
            flow::SceneManager::getGlobal().switchScene("menu");
        });
        flow::NetworkManager::getGlobal().io().restart();
    });
    
    if (_state == State::Hosting)
    {
        client.connect("localhost", 25550);
    } 
    else
    {
        std::ifstream infile("assets/ip.txt");
        if (!infile.is_open()) throw std::runtime_error("assets/ip.txt doesn't exist!");
        std::string line;
        std::getline(infile, line); // TODO: works?
        client.connect(line, 25550);
    }
}

void LobbyScene::update(const float dt)
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
                handleInput(getInputVector(keyPressed));
        }
    }
    
    _buttons.update(dt);
}

void LobbyScene::draw()
{
    _window.clear();
    if (!_connected)
    {
        sf::Text text(*_font, "Connecting... Press ESC to cancel");
        text.setCharacterSize(70);
        text.setOrigin(text.getLocalBounds().getCenter());
        text.setPosition(sf::Vector2f(_window.getSize()) / 2.0f);
        _window.draw(text);
    } else
    _buttons.draw(_window);
}

void LobbyScene::onEnter()
{
    if (!_hasInitialized)
    {
        initialize();
    }
    Multiplayer::getInstance().inMultiplayer = true;
}

void LobbyScene::onExit()
{
    
}

void LobbyScene::handleInput(const sf::Vector2f inputVector)
{
    if (inputVector.y > 0) // accept
    {
        // start game
        if (_state == State::Hosting)
        {
            auto& server = flow::NetworkManager::getGlobal().getServer();
            server.emit("start-game", "rr");
        }
    }
    else if (inputVector.y < 0) // deny
    {
        flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(_window));
        flow::SceneManager::getGlobal().switchScene("menu");
        Multiplayer::getInstance().inMultiplayer = false;
    }
    if (inputVector.x > 0) _buttons.next();
    else if (inputVector.x < 0) _buttons.prev();
}
