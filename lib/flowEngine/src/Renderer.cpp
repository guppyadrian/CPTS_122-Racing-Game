#include "flow/Renderer.hpp"
#include <iostream>
#include "flow/Shaders.hpp"
#include <SFML/Graphics/BlendMode.hpp>

namespace flow
{
    Renderer::Renderer()
    {
        if (!brightShader.loadFromMemory(shaders::brightShaderSource, sf::Shader::Type::Fragment))
        {
            // extract bright shader failed to load
        }
        if (!blurShader.loadFromMemory(shaders::blurShaderSource, sf::Shader::Type::Fragment))
        {
            // blur shader faild to load
        }
        if (!cromeAbShader.loadFromMemory(shaders::chromaticAberrationShaderSource, sf::Shader::Type::Fragment))
        {

        }
        if (!scanLines.loadFromMemory(shaders::scanLinesShaderSource, sf::Shader::Type::Fragment))
        {

        }
        if (!crtDistortion.loadFromMemory(shaders::crtDistortionShaderSource, sf::Shader::Type::Fragment))
        {

        }

        brightPass.setSmooth(true);
        blurPing.setSmooth(true);
        blurPong.setSmooth(true);
        chromaticAberration.setSmooth(true);
    }

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

        mainScene.clear();
        brightPass.clear();
        blurPing.clear();
        blurPong.clear();
        chromaticAberration.clear();

        // draw all SpriteRenderers
        for (auto& sprite : mActiveSprites)
        {
            sf::Transform tf = sprite->mGameObject->mTransform.getTransform();
            //tf.scale(sf::Vector2f(0.01f, 0.01f));
            mainScene.draw(sprite->getSprite(), tf);
        }

        // Draw to the brightness pass
        brightShader.setUniform("texture", mainScene.getTexture());
        brightShader.setUniform("threshold", 0.4f);
        brightPass.draw(sf::Sprite(mainScene.getTexture()), &brightShader);
        brightPass.display();

        // setup the blur pass
        sf::RenderTexture* input = &brightPass;
        sf::RenderTexture* output = &blurPing;

        bool pingPong = true;
        int iterations = 8; // (H+V) * 4

        // blur the brightness texture to produce the bloom effect
        for (int i = 0; i < iterations; i++) {
            output->clear(sf::Color::Transparent);

            // Set uniforms
            blurShader.setUniform("texture", input->getTexture());
            sf::Vector2f direction = pingPong ? sf::Vector2f(1.f / mWindowRef->getSize().x * ((int)(i/2) + 1), 0.f) : sf::Vector2f(0.f, 1.f / mWindowRef->getSize().y * ((int)(i / 2) +1));
            blurShader.setUniform("offset", direction);

            // Draw input onto output using the shader
            output->draw(sf::Sprite(input->getTexture()), &blurShader);
            output->display();

            // SWAP: Current output becomes the next input
            if (i == 0) {
                input = &blurPing;
                output = &blurPong;
            }
            else {
                std::swap(input, output);
            }

            pingPong = !pingPong;
        }

        // Composite in the bloom
        sf::RenderStates states;
        states.blendMode = sf::BlendAdd; // Add the glow to the scene
        mainScene.draw(sf::Sprite(input->getTexture()), states);
        mainScene.display();

        // composite chromatic abberation over the mainScene
        cromeAbShader.setUniform("texture", mainScene.getTexture());
        cromeAbShader.setUniform("samples", (mCromeAbOffset.lengthSquared() == 0.f) ? 2.f : 20.f);
        cromeAbShader.setUniform("offset", mCromeAbOffset);
        chromaticAberration.draw(sf::Sprite(input->getTexture()), &cromeAbShader);
        chromaticAberration.display();

        //mWindowRef->draw(sf::Sprite(chromaticAberration.getTexture()));

        //draw the scan lines
        scanLines.setUniform("texture", chromaticAberration.getTexture());
        scanLines.setUniform("spacing",  2.f / chromaticAberration.getSize().y );
        mainScene.clear();
        mainScene.draw(sf::Sprite(chromaticAberration.getTexture()), &scanLines);
        mainScene.display();

        // distort the screen
        crtDistortion.setUniform("texture", mainScene.getTexture());
        chromaticAberration.clear();
        chromaticAberration.draw(sf::Sprite(mainScene.getTexture()), &crtDistortion);
        chromaticAberration.display();

        mWindowRef->draw(sf::Sprite(chromaticAberration.getTexture())); // Output the final composite

    }

    void Renderer::setView(const sf::View& view)
    {
        mWindowRef->setView(view);
    }
}