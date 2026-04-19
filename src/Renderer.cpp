#include "Renderer.hpp"

#include <iostream>

namespace flow
{

    void Renderer::addSpriteRenderer(SpriteRenderer* sprite)
    {
        mActiveSprites.push_back(sprite);
    }

    void Renderer::removeSpriteRenderer(SpriteRenderer* sprite) 
    {
        auto it = std::find(mActiveSprites.begin(), mActiveSprites.end(), sprite);
        if (it != mActiveSprites.end()) 
        {
            mActiveSprites.erase(it);
        }
    }

    void Renderer::drawAll()
    {
        for (auto& sprite : mActiveSprites)
        {
            windowRef->draw(sprite->getSprite(), sprite->mGameObject->mTransform.getTransform());
        }
    }
}