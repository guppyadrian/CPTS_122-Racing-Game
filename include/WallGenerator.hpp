
#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <SFML/Graphics/Color.hpp>

class WallGenerator
{
	flow::GameObject GenerateWall(int length, float angle, sf::Color color);
	static flow::GameObject GenerateWall(int radius, float angle, float arcLength, int subdivisions, sf::Color color);
};