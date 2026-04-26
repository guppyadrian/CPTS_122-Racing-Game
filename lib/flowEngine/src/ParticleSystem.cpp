#include <algorithm>

#include "flow/components/ParticleSystem.hpp"
#include "flow/Interpolation.hpp"

namespace flow
{
	ParticleSystem::ParticleSystem()
	{
		mVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	}

	void ParticleSystem::init()
	{
		//initialize particle arrays
		mLifetimes.resize(mParticleCount);
		mPositions.resize(mParticleCount);
		mVelocities.resize(mParticleCount);
		mSizes.resize(mParticleCount);
		mColors.resize(mParticleCount);

		// starting position is (0,0)
		for (int i = 0; i < mParticleCount; i++)
		{
			mLifetimes[i] = mStartLifetime;
			mPositions[i] = { 0,0 };
			mVelocities[i] = mStartVelocity;
			mSizes[i] = mStartSize;
			mColors[i] = mStartColor;
		}
	}

	void ParticleSystem::update(float dt)
	{
		// nothing to do if there are no particles
		if (mParticleCount <= 0)
			return;

		// ensure vertex array has space for 6 vertices (2 triangles) per particle
		if (mVertexArray.getVertexCount() != mParticleCount * 6)
		{
			mVertexArray.clear();
			mVertexArray.resize(mParticleCount * 6);
		}

		for (int i = 0; i < mParticleCount; ++i)
		{
			// update lifetime
			mLifetimes[i] = std::max(0.0f, mLifetimes[i] - dt);

			// update velocity
			mVelocities[i] += mAcceleration * dt;
			mVelocities[i] *= std::clamp((1.f - mDrag * dt), 0.f, 1.f);

			// update position
			mPositions[i] += mVelocities[i] * dt;

			// compute life ratio for interpolation
			float lifeRatio = (mStartLifetime > 0.0f) ? (mLifetimes[i] / mStartLifetime) : 0.0f;

			// update size
			mSizes[i] = mStartSize * lifeRatio;

			// update color
			mColors[i] = interp::lerp(mStartColor, mEndColor, lifeRatio);

			// build quad (two triangles) centered at position
			float half = mSizes[i] * 0.5f;
			sf::Vector2f p = mPositions[i];

			sf::Vector2f tl = { p.x - half, p.y - half };
			sf::Vector2f tr = { p.x + half, p.y - half };
			sf::Vector2f br = { p.x + half, p.y + half };
			sf::Vector2f bl = { p.x - half, p.y + half };

			int base = i * 6;
			mVertexArray[base + 0] = sf::Vertex(tl, mColors[i]);
			mVertexArray[base + 1] = sf::Vertex(tr, mColors[i]);
			mVertexArray[base + 2] = sf::Vertex(br, mColors[i]);

			mVertexArray[base + 3] = sf::Vertex(tl, mColors[i]);
			mVertexArray[base + 4] = sf::Vertex(br, mColors[i]);
			mVertexArray[base + 5] = sf::Vertex(bl, mColors[i]);
		}
	}

	const sf::Drawable& ParticleSystem::getDrawable()
	{
		return mVertexArray;
	}
}