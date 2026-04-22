#include "flow/Renderer.hpp"
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
            sf::Transform tf = sprite->mGameObject->mTransform.getTransform();
            //tf.scale(sf::Vector2f(0.01f, 0.01f));
            windowRef->draw(sprite->getSprite(), tf);
        }
    }

    void Renderer::setView(const sf::View& view)
    {
        windowRef->setView(view);
    }
}