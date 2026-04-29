//
// Created by guppy on 4/25/26.
//

#include "flow/components/NetworkGhostManager.hpp"

#include "flow/GameObject.hpp"
#include "flow/LevelScene.hpp"
#include "flow/NetworkManager.hpp"
#include "flow/SceneManager.hpp"
#include "flow/components/SpriteRenderer.hpp"
#include "flow/components/ParticleSystem.hpp"
#include "flow/components/AudioSource.hpp"
#include "flow/components/Rigidbody.hpp"
#include "network/BufferParser.hpp"
#include "network/Shared.hpp"
#include <string>

namespace flow
{
    NetworkGhostManager::NetworkGhostManager(const std::string& eventName)
    : _eventName(eventName)
    {
        NetworkManager::getGlobal().getClient().on<int>("handshakePlayerID", [this](const int id)
        {
            _id = id;
        });
        
        NetworkManager::getGlobal().getClient().on(eventName, [this](const network::ByteBuffer& buffer)
        {
            try
            {
                auto& curScene = dynamic_cast<LevelScene&>(SceneManager::getGlobal().getCurrentScene());
                network::BufferParser parser(buffer);

                for (int i = 0; i < buffer.size() / 16; i++) // 16 is size of player: int id, floats xyz
                {
                    int id = parser.read<int>();
                    float transform[3] = {
                        parser.read<float>(),
                        parser.read<float>(),
                        parser.read<float>()
                    };

                    if (id == _id) continue;

                    if (!_ghosts.contains(id))
                    {
                        GameObject ghost; // such bad practice to hard code this in, but too bad!
                        ghost.mTransform.setScale({ 0.02f, 0.02f });

                        ghost.addComponent<SpriteRenderer>("assets/Ghost" + std::to_string(id % 8 + 1) + ".png");
                        _ghosts[id] = &ghost.addComponent<NetworkGhost>(id);

                        // narrow beam particle system
                        auto& ps2 = ghost.addComponent<flow::ParticleSystem>();
                        ps2.setStartPosition({ 0.f, 200.f });
                        ps2.setParticleCount(50);
                        ps2.setStartRandomVelocity(1000.f);
                        ps2.setStartVelocity({ 0.f, 30000.f });
                        ps2.setStartColor(sf::Color(255, 200, 220, 128));
                        ps2.setEndColor(sf::Color(255, 40, 200, 32));
                        ps2.setStartSize(200);
                        ps2.setEndSize(50);
                        ps2.setStartLifetime(0.12f);

                        // wide thrust particle system
                        auto& ps1 = ghost.addComponent<flow::ParticleSystem>();
                        ps1.setStartPosition({ 0.f, 300.f });
                        ps1.setParticleCount(500);
                        ps1.setStartRandomVelocity(1500.f);
                        ps1.setStartVelocity({ 0.f, 2000.f });
                        ps1.setStartColor(sf::Color(250, 250, 250, 200));
                        ps1.setEndColor(sf::Color(25, 100, 250, 150));
                        ps1.setStartSize(50);
                        ps1.setEndSize(25);
                        ps1.setStartLifetime(0.3f);
                        ps1.startEmit();

                        auto& thrustAudio = ghost.addComponent<flow::audio::AudioSource>("assets/sfx/thrustLoop.mp3");
                        thrustAudio.setVolume(67.f);
                        thrustAudio.loop(true);
                        thrustAudio.play();

                        //ghost.addComponent<flow::Rigidbody>();

                        curScene.AddGameObject(std::move(ghost));
                    }

                    _ghosts[id]->mGameObject->mTransform.setPosition({ transform[0], transform[1] });
                    _ghosts[id]->mGameObject->mTransform.setRotationRad(transform[2]);
                }
            }
            catch (std::bad_cast ec)
            {
                return;
            }
            
        });

        NetworkManager::getGlobal().getClient().emit("handshakePlayerReady", '\0');
    }

    NetworkGhostManager::~NetworkGhostManager()
    {
        NetworkManager::getGlobal().getClient().off(_eventName);
    }
}
