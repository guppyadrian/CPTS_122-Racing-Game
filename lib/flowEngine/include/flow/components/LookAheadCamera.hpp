#pragma once

#include <algorithm>

#include "flow/components/Camera.hpp"
#include "flow/Renderer.hpp"
#include "flow/Interpolation.hpp"

namespace flow
{
	class LookAheadCamera : public Camera
	{
		sf::Vector2f mLastTransformPos;
		sf::Vector2f mTarget;

		float mLookAheadFactor = 60.f;
		float mTargetLerp = 20.f;
		float mCameraLerp = 5.f;
	public:

		LookAheadCamera(sf::View view) : Camera(view) {};
		~LookAheadCamera() = default;

		// --- Getters ---
		float lookAheadFactor() const { return mLookAheadFactor; }
		float targetLerp() const { return mTargetLerp; }
		float cameraLerp() const { return mCameraLerp; }

		// --- Setters ---
		void lookAheadFactor(float value) { mLookAheadFactor = value; }
		void targetLerp(float value) { mTargetLerp = value; }
		void cameraLerp(float value) { mCameraLerp = value; }

		void init() override
		{
			reset();
		};

		void reset()
		{
			mTarget = mLastTransformPos = mGameObject->mTransform.getPosition();
			mView.setCenter(mTarget);
		};

		void update(float dt) override
		{
			// lag spike detection
			if (dt > 0.05)
			{
				reset();
				return;
			}

			sf::Vector2f transformPos = mGameObject->mTransform.getPosition();
			sf::Vector2f lookAhead = transformPos - mLastTransformPos;
			lookAhead *= mLookAheadFactor;

			// exponential decay to calculate framerate independant interpolation alpha
			float targetAlpha = 1.0f - std::exp(-mTargetLerp * dt);
			float cameraAlpha = 1.0f - std::exp(-mCameraLerp * dt);

			// lerp target towards the look ahead point
			mTarget = interp::lerp(mTarget, transformPos + lookAhead, std::clamp(targetAlpha, 0.f, 1.f));

			// lerp the camera towards its target
			mView.setCenter(interp::lerp(mView.getCenter(), mTarget, std::clamp(cameraAlpha, 0.f , 1.f)));
			Renderer::getGlobalRenderer().setView(mView);


			mLastTransformPos = transformPos;
		};
	};
}