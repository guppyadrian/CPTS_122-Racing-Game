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
		int mActiveParticles;

		std::vector<float> mLifetimes;
		std::vector<sf::Vector2f> mPositions;
		std::vector<sf::Vector2f> mVelocities;
		std::vector<float> mSizes;
		std::vector<sf::Color> mColors;

		float mStartLifetime;
		sf::Vector2f mStartVelocity;
		sf::Vector2f mAcceleration;
		float mDrag;
		float mStartSize;
		float mEndSize;
		sf::Color mStartColor;
		sf::Color mEndColor;

		void resetParticle(int i);

	public:

		ParticleSystem();
		~ParticleSystem();

		const sf::Drawable& getDrawable() override;

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override {};

		// Getters and setters
		float getParticleCount() const { return mParticleCount; }
		void setParticleCount(float v) { mParticleCount = v; }

		float getStartLifetime() const { return mStartLifetime; }
		void setStartLifetime(float v) { mStartLifetime = v; }

		sf::Vector2f getStartVelocity() const { return mStartVelocity; }
		void setStartVelocity(const sf::Vector2f& v) { mStartVelocity = v; }

		sf::Vector2f getAcceleration() const { return mAcceleration; }
		void setAcceleration(const sf::Vector2f& v) { mAcceleration = v; }

		float getDrag() const { return mDrag; }
		void setDrag(float v) { mDrag = v; }

		float getStartSize() const { return mStartSize; }
		void setStartSize(float v) { mStartSize = v; }

		float getEndSize() const { return mEndSize; }
		void setEndSize(float v) { mEndSize = v; }

		sf::Color getStartColor() const { return mStartColor; }
		void setStartColor(const sf::Color& c) { mStartColor = c; }

		sf::Color getEndColor() const { return mEndColor; }
		void setEndColor(const sf::Color& c) { mEndColor = c; }
	};
}