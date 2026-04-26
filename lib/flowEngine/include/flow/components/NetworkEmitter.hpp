//
// Created by guppy on 4/25/26.
//

#pragma once
#include <string>

#include "flow/Component.hpp"

namespace flow
{
    class NetworkEmitter final : public Component
    {
    private:
        std::string _eventName;
    public:
        explicit NetworkEmitter(const std::string& eventName) : _eventName(eventName) {}

        // implement Component interface
        void init() override;
        void update(float dt) override {};
        void fixedUpdate() override;
    };
} // flow