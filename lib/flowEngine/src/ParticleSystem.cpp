#include "flow/components/ParticleSystem.hpp"

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
			mVelocities[i] = mStartVelocity;
			mSizes[i] = mStartSize;

		}
	}

	void ParticleSystem::update(float dt)
	{
		// update the particle lifetimes

		// update the particle velocities

		// update the particle positions

		// update the particle sizes

		// update the particle colors
	}

	const sf::Drawable& ParticleSystem::getDrawable()
	{
		return mVertexArray;
	}
}