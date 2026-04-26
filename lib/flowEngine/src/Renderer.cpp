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
        ping.setSmooth(true);
        pong.setSmooth(true);
    }

    void Renderer::addRenderable(Renderable* renderable)
    {
        mActiveSprites.push_back(renderable);
    }

    void Renderer::removeRenderable(Renderable* renderable)
    {
        auto it = std::find(mActiveSprites.begin(), mActiveSprites.end(), renderable);
        if (it != mActiveSprites.end()) 
        {
            mActiveSprites.erase(it);
        }
    }

    void Renderer::drawAll()
    {

        mainScene.clear();
        brightPass.clear();
        ping.clear();
        pong.clear();

        // draw all SpriteRenderers
        for (auto& sprite : mActiveSprites)
        {
            sf::Transform tf = sprite->mGameObject->mTransform.getTransform();
            //tf.scale(sf::Vector2f(0.01f, 0.01f));
            mainScene.draw(sprite->getDrawable(), tf);
        }

        // Draw to the brightness pass
        brightShader.setUniform("texture", mainScene.getTexture());
        brightShader.setUniform("threshold", 0.4f);
        brightPass.draw(sf::Sprite(mainScene.getTexture()), &brightShader);
        brightPass.display();

        // setup the blur pass
        sf::RenderTexture* input = &brightPass;
        sf::RenderTexture* output = &ping;

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
                input = &ping;
                output = &pong;
            }
            else {
                std::swap(input, output);
            }

            pingPong = !pingPong;
        }

        output->draw(sf::Sprite(mainScene.getTexture()));
        output->display();

        // Composite in the bloom
        sf::RenderStates states;
        states.blendMode = sf::BlendAdd; // Add the glow to the scene
        output->draw(sf::Sprite(input->getTexture()), states);
        output->display();

        std::swap(input, output);

        // composite chromatic abberation over the mainScene
        cromeAbShader.setUniform("texture", mainScene.getTexture());
        cromeAbShader.setUniform("samples", (mCromeAbOffset.lengthSquared() == 0.f) ? 2.f : 20.f);
        cromeAbShader.setUniform("offset", mCromeAbOffset);
        output->draw(sf::Sprite(mainScene.getTexture()), &cromeAbShader);
        output->display();

        std::swap(input, output);

        //mWindowRef->draw(sf::Sprite(chromaticAberration.getTexture()));

        //draw the scan lines
        scanLines.setUniform("texture", input->getTexture());
        scanLines.setUniform("spacing",  2.f / input->getSize().y );
        output->draw(sf::Sprite(input->getTexture()), &scanLines);
        output->display();

        std::swap(input, output);

        // distort the screen
        crtDistortion.setUniform("texture", input->getTexture());
        crtDistortion.setUniform("distortion", 1.2f);
        output->draw(sf::Sprite(input->getTexture()), &crtDistortion);
        output->display();

        // flip the output fiew for the shader pass
        sf::View view = mWindowRef->getView();
        view.setSize({ view.getSize().x, -fabs(view.getSize().y) });
        mWindowRef->setView(view);

        mWindowRef->draw(sf::Sprite(output->getTexture())); // Output the final composite

    }

    void Renderer::setView(const sf::View& view)
    {
        mainScene.setView(view);
    }
}