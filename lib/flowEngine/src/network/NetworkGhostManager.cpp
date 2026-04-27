//
// Created by guppy on 4/25/26.
//

#include "flow/components/NetworkGhostManager.hpp"

#include "flow/GameObject.hpp"
#include "flow/LevelScene.hpp"
#include "flow/NetworkManager.hpp"
#include "flow/SceneManager.hpp"
#include "flow/components/SpriteRenderer.hpp"
#include "network/BufferParser.hpp"
#include "network/Shared.hpp"

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
            std::cout << "MY ID: " << _id << std::endl;
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
                    ghost.mTransform.setScale({0.02f, 0.02f});

                    ghost.addComponent<SpriteRenderer>("assets/player.png");
                    _ghosts[id] = &ghost.addComponent<NetworkGhost>(id);
                    
                    curScene.AddGameObject(std::move(ghost));
                }

                _ghosts[id]->mGameObject->mTransform.setPosition({transform[0], transform[1]});
                _ghosts[id]->mGameObject->mTransform.setRotationRad(transform[2]);
            }
        });

        NetworkManager::getGlobal().getClient().emit("handshakePlayerReady", '\0');
    }
}
