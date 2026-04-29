//
// Created by guppy on 4/29/26.
//

#include "UI/LobbyScene.hpp"

#include <fstream>

#include "LevelLoader.hpp"
#include "Multiplayer.hpp"
#include "flow/NetworkManager.hpp"
#include "flow/Renderer.hpp"
#include "flow/SceneManager.hpp"
#include "network/NetworkClient.hpp"
#include "UI/LevelSelectScene.hpp"
#include "UI/MenuScene.hpp"

LobbyScene::LobbyScene(sf::RenderWindow& window, const State state) : UIScene("multiplayer-lobby", window), _state(state)
{
    float y = _window.getSize().y / 2.0f;
    _buttons.add("menu/selectmap.png", {  400, y}); // host
    _buttons.add("menu/start.png", {1000, y}); // join
    _buttons.add("menu/quitButton.png", {1600, y}, {0.5f, 0.5f}); // quit
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
    
    Multiplayer::getInstance().reset();
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
            _playerCount++;
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
            socket->on("disconnect", [this]() mutable {_playerCount--;});
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
    
    client.on<long long>("start-time", [](const long long startTime)
    {
        Multiplayer::getInstance().startTime = startTime;
    });
    
    client.on<std::string>("start-game", [](const std::string &levelPath)
    {
        asio::post(flow::NetworkManager::getGlobal().io(), [levelPath]()
        {
            std::cout << "level path: " << levelPath << std::endl;
            flow::SceneManager::getGlobal().loadScene(LevelLoader().readFile(levelPath));
            flow::SceneManager::getGlobal().switchScene(levelPath, false);
        });
        flow::NetworkManager::getGlobal().io().restart();
    });
    
    client.on<int>("handshakePlayerID", [](const int id)
    {
        Multiplayer::getInstance().id = id;
    });
    
    client.on("end-game", []()
    {
        asio::post(flow::NetworkManager::getGlobal().io(), []()
        {
            flow::SceneManager::getGlobal().switchScene("multiplayer-lobby");
        });
        flow::NetworkManager::getGlobal().io().restart();
    });
    
    client.on("connect_error", [&client, this]() mutable
    {
        client.connect(_lastIP, 25550); 
    });
    
    client.on("disconnect", [this]() mutable
    {
        _connected = false;
        if (_state == State::Hosting) return;
        asio::post(flow::NetworkManager::getGlobal().io(), []()
        {
            flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(flow::Renderer::getGlobalRenderer().getWindow()));
            flow::SceneManager::getGlobal().switchScene("menu");
        });
        flow::NetworkManager::getGlobal().io().restart();
    });
    
    if (_state == State::Hosting)
    {
        client.connect("localhost", 25550);
        _lastIP = "localhost";
    } 
    else
    {
        std::ifstream infile("assets/ip.txt");
        if (!infile.is_open()) throw std::runtime_error("assets/ip.txt doesn't exist!");
        std::string line;
        std::getline(infile, line); // TODO: works?
        client.connect(line, 25550);
        _lastIP = line;
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
        drawText("Connecting.. Press ESC to cancel");
    } 
    else if (_state == State::Joining) 
    {
        drawText("Connected!");
        drawText("press ESC to leave", {0, 100}, 40);
    } 
    else
        _buttons.draw(_window);
}

void LobbyScene::onEnter()
{
    UIScene::onEnter();
    if (!_hasInitialized)
    {
        initialize();
    }
    Multiplayer::getInstance().inMultiplayer = true;
    Multiplayer::getInstance().endEmitted = false;
}

void LobbyScene::onExit()
{
    
}

void LobbyScene::handleInput(const sf::Vector2f inputVector)
{
    if (inputVector.y > 0) // accept
    {
        // start game
        if (_state != State::Hosting) return;
        
        
        switch (_buttons.selected())
        {
            case 0: //select map
                flow::SceneManager::getGlobal().loadScene(std::make_unique<LevelSelectScene>(_window));
                flow::SceneManager::getGlobal().switchScene("level-select", false);
                break;
            case 1:
            {
                auto& server = flow::NetworkManager::getGlobal().getServer();
                long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                server.emit("start-time", now + 5000);
                server.emit("start-game", Multiplayer::getInstance().trackSelected);
                break;
            }
            case 2: // quit
                flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(_window));
                flow::SceneManager::getGlobal().switchScene("menu");
                break;
        }
        
    }
    else if (inputVector.y < 0) // deny
    {
        flow::SceneManager::getGlobal().loadScene(std::make_unique<MenuScene>(_window));
        flow::SceneManager::getGlobal().switchScene("menu");
    }
    if (inputVector.x > 0) _buttons.next();
    else if (inputVector.x < 0) _buttons.prev();
}
