#include "flow/components/Camera.hpp"
#include "flow/Renderer.hpp"

namespace flow
{
	void Camera::update(float dt)
	{
		mView.setCenter(mGameObject->mTransform.getPosition());
		Renderer::getGlobalRenderer().setView(mView);
	}
}