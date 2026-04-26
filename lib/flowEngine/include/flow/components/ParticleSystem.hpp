#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "flow/Renderable.hpp"

namespace flow
{
	class ParticleSystem : public Renderable
	{
		sf::VertexArray mVertexArray;

		int mParticleCount;
		std::vector<float> mLifetimes;
		std::vector<sf::Vector2f> mPositions;
		std::vector<sf::Vector2f> mVelocities;
		std::vector<float> mSizes;
		std::vector<sf::Color> mColors;

		float mStartLifetime;
		sf::Vector2f mStartVelocity;
		float mDrag;
		float mStartSize;
		float mEndSize;
		sf::Color mStartColor;
		sf::Color mEndColor;
	public:

		ParticleSystem();

		const sf::Drawable& getDrawable() override;

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override {};
	};
}