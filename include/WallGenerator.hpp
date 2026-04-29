/**
 * @file WallGenerator.hpp
 * @author Logan Rainchild
 * @brief a class for generating streight walls and ark walls
 */


#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <SFML/Graphics/Color.hpp>

class WallGenerator
{
public:
	static flow::GameObject GenerateWall(sf::Vector2f pos, int length, float angle, sf::Color color);
	static flow::GameObject GenerateWall(sf::Vector2f pos, int radius, float angle, float arcLength, int subdivisions, sf::Color color);
};