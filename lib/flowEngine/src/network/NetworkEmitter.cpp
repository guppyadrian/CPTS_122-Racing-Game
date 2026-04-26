//
// Created by guppy on 4/25/26.
//

#include "flow/components/NetworkEmitter.hpp"

#include "flow/GameObject.hpp"
#include "flow/NetworkManager.hpp"

namespace flow
{
    void NetworkEmitter::init()
    {
    }

    void NetworkEmitter::fixedUpdate()
    {
        const auto& pos = mGameObject->mTransform.getPosition();
        const auto& rot = mGameObject->mTransform.getRotation();
        const float tPacked[3] = { pos.x, pos.y, rot.asRadians() };
        
        NetworkManager::getGlobal().getClient().emit(_eventName, tPacked);
    }
}
