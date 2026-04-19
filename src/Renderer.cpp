#include "Renderer.hpp"

namespace flow
{

    void Renderer::addSprite(SpriteRenderer* sprite)
    {
        mActiveSprites.push_back(sprite);
    }

    void Renderer::removeSprite(SpriteRenderer* sprite) {
        auto it = std::find(mActiveSprites.begin(), mActiveSprites.end(), sprite);
        if (it != mActiveSprites.end()) {
            mActiveSprites.erase(it);
        }
    }

    void Renderer::drawAll()
    {
        for (auto& sprite : mActiveSprites)
        {
            //sprite->getSprite()
            windowRef.draw(sprite->getSprite(), sprite->mGameObject->mTransform.getTransform());
        }
    }
}