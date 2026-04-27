#include <algorithm>


#include "flow/components/ParticleSystem.hpp"
#include "flow/Interpolation.hpp"
#include "flow/Renderer.hpp"

namespace flow
{
	ParticleSystem::ParticleSystem()
	: mParticleCount(0)
    , mActiveParticleCount(0)
	, mEmitAccumulator(0.f)
    , mEmit(false)
	, mStartRandomVelocity(0.f)
	, mStartLifetime(1.0f)
	, mStartVelocity(0.f, 0.f)
	, mAcceleration(0.f, 0.f)
	, mDrag(0.f)
	, mStartSize(1.f)
	, mEndSize(1.f)
	, mStartColor(sf::Color::White)
	, mEndColor(sf::Color::Transparent)
	{
		mVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

		Renderer::getGlobalRenderer().addRenderable(this);

		std::random_device rd;
		mRandGen = std::mt19937(rd());
	}

	ParticleSystem::~ParticleSystem()
	{
		Renderer::getGlobalRenderer().removeRenderable(this);
	}

	void ParticleSystem::init()
	{
		//initialize particle arrays
		mActiveParticles.resize(mParticleCount);
		mLifetimes.resize(mParticleCount);
		mPositions.resize(mParticleCount);
		mVelocities.resize(mParticleCount);
		mSizes.resize(mParticleCount);
		mColors.resize(mParticleCount);

		// starting position is (0,0) and activate/reset all particles
		for (int i = 0; i < mParticleCount; i++)
		{
			resetParticle(i);
		}

		// reflect that all particles are active after init
		mActiveParticleCount = mParticleCount;
	}

	inline void ParticleSystem::resetParticle(int i)
	{
		static 	std::uniform_real_distribution<float> dist(0.f, 360.f);

		mActiveParticles[i] = true;
		mLifetimes[i] = mStartLifetime;
		mPositions[i] = { 0,0 };
		mVelocities[i] = mStartVelocity + sf::Vector2f(mStartRandomVelocity, 0.f).rotatedBy(sf::degrees(dist(mRandGen)));
		mSizes[i] = mStartSize;
		mColors[i] = mStartColor;
	}

	void ParticleSystem::emitOne()
	{
		for (int i = 0; i < mParticleCount; i++)
		{
			if (mActiveParticles[i] == false)
			{
				// found an inactive particle to emit
				mActiveParticleCount++;
				resetParticle(i);
				return;
			}
		}
	}

	void ParticleSystem::deactivateParticle(int i)
	{
		mActiveParticles[i] = false;
	}

	void ParticleSystem::update(float dt)
	{
		mEmitAccumulator += dt;

		if (mParticleCount > 0 && mEmitAccumulator > (mStartLifetime / mParticleCount))
		{
			if (mEmit)
				emitOne();
			mEmitAccumulator -= (mStartLifetime / mParticleCount);
		}

		if (mActiveParticleCount <= 0 || mParticleCount == 0)
			return;

		// temporary verticies list
		std::vector<sf::Vertex> vertices;
		vertices.reserve(mActiveParticleCount * 6);

		int packedActive = 0;
		for (int i = 0; i < mParticleCount; ++i)
		{
			if (!mActiveParticles[i])
				continue;

			// update lifetime
			mLifetimes[i] = std::max(0.0f, mLifetimes[i] - dt);

			if (mLifetimes[i] <= 0.0f)
			{
				deactivateParticle(i);
				// particle deactivated and skipped
				continue;
			}

			// update velocity & position
			mVelocities[i] += mAcceleration * dt;
			mVelocities[i] *= std::clamp((1.f - mDrag * dt), 0.f, 1.f);
			mPositions[i] += mVelocities[i] * dt;

			// interpolation
			float lifeRatio = (mStartLifetime > 0.0f) ? (mLifetimes[i] / mStartLifetime) : 0.0f;
			mSizes[i] = mStartSize * lifeRatio;
			mColors[i] = interp::lerp(mStartColor, mEndColor, 1 - lifeRatio);

			// build quad (two triangles) centered at position
			float half = mSizes[i] * 0.5f;
			const sf::Vector2f& p = mPositions[i];

			sf::Vector2f tl = { p.x - half, p.y - half };
			sf::Vector2f tr = { p.x + half, p.y - half };
			sf::Vector2f br = { p.x + half, p.y + half };
			sf::Vector2f bl = { p.x - half, p.y + half };

			vertices.emplace_back(tl, mColors[i]);
			vertices.emplace_back(tr, mColors[i]);
			vertices.emplace_back(br, mColors[i]);

			vertices.emplace_back(tl, mColors[i]);
			vertices.emplace_back(br, mColors[i]);
			vertices.emplace_back(bl, mColors[i]);

			packedActive++;
		}

		// update mActiveParticleCount to reflect any deactivations that occurred during update
		mActiveParticleCount = packedActive;

		// move vertices into mVertexArray
		mVertexArray.clear();
		if (!vertices.empty())
		{
			mVertexArray.resize(static_cast<std::size_t>(vertices.size()));
			for (std::size_t vi = 0; vi < vertices.size(); ++vi)
				mVertexArray[vi] = vertices[vi];
		}
	}

	const sf::Drawable& ParticleSystem::getDrawable()
	{
		return mVertexArray;
	}
}