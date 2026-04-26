//
// Created by guppy on 4/25/26.
//

#pragma once
#include <string>

#include "flow/Component.hpp"

namespace flow
{
    class NetworkGhost final : Component
    {
    private:
        std::string _eventName;
    public:
        explicit NetworkGhost(const std::string& eventName);
        
        // implement Component interface
        void init() override {};
        void update(float dt) override {};
        void fixedUpdate() override {};
    };
} // flow