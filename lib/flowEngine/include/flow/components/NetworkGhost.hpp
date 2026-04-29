//
// Created by guppy on 4/25/26.
//

#pragma once
#include <string>

#include "flow/Component.hpp"
#include "flow/components/ParticleSystem.hpp"

namespace flow
{
    class NetworkGhost final : public Component
    {
    private:
        int _id;
        ParticleSystem* _jetFlame;
    public:
        explicit NetworkGhost(const int id) : _id(id), _jetFlame(nullptr) {}
        
        // implement Component interface
        void init() override;
        void update(float dt) override;
        void fixedUpdate() override {};
    };
} // flow