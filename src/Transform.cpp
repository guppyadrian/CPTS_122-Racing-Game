#include "Transform.hpp"

namespace flow
{

	void Transform::setRotationRad(float radians)
	{
		setRotation(sf::radians(radians));
	}

	void Transform::setRotationDeg(float degrees)
	{
		setRotation(sf::degrees(degrees));
	}
	float Transform::getRotationRad()
	{
		return getRotation().asRadians();
	}
	float Transform::getRotationDeg()
	{
		return getRotation().asRadians();
	}
}