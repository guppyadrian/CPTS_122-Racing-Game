//
// Created by guppy on 4/25/26.
//

#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "NetworkGhost.hpp"
#include "flow/Component.hpp"

namespace flow
{
    class NetworkGhostManager : public Component
    {
    private:
        std::string _eventName;
        std::unordered_map<int32_t, NetworkGhost*> _ghosts;
        //std::shared_ptr<GameObject> _prefab; TODO: maybe in the future when game objects can be copied
    public:
        explicit NetworkGhostManager(const std::string& eventName);
        // format: int32_t playerID float x float y float z
        
        void init() override {}
        void update(float dt) override {}
        void fixedUpdate() override {}

    private:
        void parsePacket() {};
    };
} // flow