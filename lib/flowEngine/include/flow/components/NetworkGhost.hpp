//
// Created by guppy on 4/25/26.
//

#pragma once
#include <string>

#include "flow/Component.hpp"

namespace flow
{
    class NetworkGhost final : public Component
    {
    private:
        int _id;
    public:
        explicit NetworkGhost(const int id) : _id(id) {}
        
        // implement Component interface
        void init() override {};
        void update(float dt) override {};
        void fixedUpdate() override {};
    };
} // flow