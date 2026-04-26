//
// Created by guppy on 4/25/26.
//

#include "flow/components/NetworkGhost.hpp"

#include "flow/GameObject.hpp"
#include "flow/NetworkManager.hpp"
#include "network/BufferParser.hpp"

namespace flow
{
    NetworkGhost::NetworkGhost(const std::string& eventName) : _eventName(eventName)
    {
        NetworkManager::getGlobal().getClient().on(eventName, [this](const network::ByteBuffer& buffer)
        {
            float transform[3]{};
            network::BufferParser parser(buffer);
            transform[0] = parser.read<float>();
            transform[1] = parser.read<float>();
            transform[2] = parser.read<float>();

            mGameObject->mTransform.setPosition({transform[0], transform[1]});
            mGameObject->mTransform.setRotationRad(transform[2]);
        });
    }
}
