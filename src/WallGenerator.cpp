#include "WallGenerator.hpp"
#include <flow/components/SpriteRenderer.hpp>
#include <flow/components/Rigidbody.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>        // for sf::Image
#include <SFML/System/Vector2.hpp>       // for sf::Vector2f / Vector2u (avoid transitive-include warning)
#include <iostream>

flow::GameObject WallGenerator::GenerateWall(sf::Vector2f pos, int length, float angle, sf::Color color)
{
	auto gameObject = flow::GameObject();

	// generate an image texture
	sf::Image image(sf::Vector2u(length, 1), color);
	sf::Texture texture;
	if (!texture.loadFromImage(image))
	{
		std::cerr << "Failed to load texture from image\n";
	}

	auto& sr = gameObject.addComponent<flow::SpriteRenderer>(texture);

	// Create a rigidbody component ---
	auto& rbComponent = gameObject.addComponent<flow::Rigidbody>(); // create a rigidbody

	// --- Configure the rigidBody's parameters ---
	// Note: You can have multiple collision shapes on a single body!
	b2BodyId bodyId = rbComponent.getBodyId();
	b2Body_SetType(bodyId, b2_staticBody); // static body
	b2SurfaceMaterial mat[1];
	mat[0] = b2DefaultSurfaceMaterial();
	mat[0].friction = 0.f;
	mat[0].restitution = 0.f;

	// --- get the sprite (we added the SpriteRenderer just above) ---
	auto& sprite = gameObject.getComponent<flow::SpriteRenderer>()->getSprite();
	// --- local bounds = actual texture size in pixels ---
	sf::FloatRect local = sprite.getLocalBounds();

	// --- Box2D box expects half-width and half-height ---
	b2ChainDef chain = b2DefaultChainDef();
	b2Vec2 verts[4] = { {-local.size.x/2,0}, {local.size.x/2,0}, {0,0}, {0,0} };
	chain.points = verts;
	chain.count = 4;
	chain.isLoop = true;
	chain.materials = mat;
	chain.materialCount = 1;

	// Attach it to the existing bodyId
	b2ChainId shapeId = b2CreateChain(bodyId, &chain);

	gameObject.mTransform.setRotationDeg(angle);
	gameObject.mTransform.setPosition(pos);

	return gameObject;
}

flow::GameObject WallGenerator::GenerateWall(sf::Vector2f pos, int radius, float angle, float arcLength, int subdivisions, sf::Color color)
{
	using namespace std;

	// create a gameobject
	auto gameObject = flow::GameObject();

	// prepare an image large enough to contain the arc (centered)
	int texSize = radius * 2 + 4; // padding
	sf::Image image(sf::Vector2u(texSize, texSize), color);
	// make transparent background
	for (unsigned int y = 0; y < image.getSize().y; ++y)
		for (unsigned int x = 0; x < image.getSize().x; ++x)
			image.setPixel({ x, y }, sf::Color(0, 0, 0, 0));

	float cx = texSize / 2.0f;
	float cy = texSize / 2.0f;

	// sample vertices along the arc
	int count = max(2, subdivisions + 1);
	std::vector<b2Vec2> verts;
	verts.reserve(count);

	auto toImgX = [&](float vx) { return static_cast<int>(std::lround(cx + vx)); };
	auto toImgY = [&](float vy) { return static_cast<int>(std::lround(cy + vy)); };

	std::vector<sf::Vector2f> points;
	points.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		float t = static_cast<float>(i) / static_cast<float>(count - 1);
		float theta = angle + arcLength * t;
		float x = std::cos(theta) * static_cast<float>(radius);
		float y = std::sin(theta) * static_cast<float>(radius);
		points.emplace_back(x, y);
		verts.push_back({ x, y });
	}

	// Reverse winding if normals face the wrong way.
	// Box2D chain normals point to the LEFT of travel direction.
	// Uncomment to flip which side receives collision:
	// std::reverse(verts.begin(), verts.end());

	for (int i = 0; i < (int)points.size() - 1; ++i)
	{
		auto a = points[i];
		auto b = points[i + 1];
		float dx = b.x - a.x;
		float dy = b.y - a.y;
		float len = std::hypot(dx, dy);
		int steps = std::max(1, static_cast<int>(std::ceil(len)));
		for (int s = 0; s <= steps; ++s)
		{
			float u = steps == 0 ? 0.f : static_cast<float>(s) / steps;
			float px = a.x + dx * u;
			float py = a.y + dy * u;
			int ix = toImgX(px);
			int iy = toImgY(py);
			if (ix >= 0 && ix < (int)image.getSize().x && iy >= 0 && iy < (int)image.getSize().y)
			{
				image.setPixel(sf::Vector2u(static_cast<unsigned>(ix), static_cast<unsigned>(iy)), color);
			}
		}
	}

	// create texture from image
	sf::Texture texture;
	if (!texture.loadFromImage(image))
	{
		std::cerr << "Failed to load texture from image\n";
	}

	auto& sr = gameObject.addComponent<flow::SpriteRenderer>(texture);

	// Create a rigidbody component
	auto& rbComponent = gameObject.addComponent<flow::Rigidbody>();

	b2BodyId bodyId = rbComponent.getBodyId();
	b2Body_SetType(bodyId, b2_staticBody);

	b2SurfaceMaterial mat[1];
	mat[0] = b2DefaultSurfaceMaterial();
	mat[0].friction = 0.0f;
	mat[0].restitution = 0.0f;

	// Ghost vertices: extend linearly beyond each endpoint so Box2D
	// computes correct normals at the chain's start and end
	auto extendPoint = [](b2Vec2 endpoint, b2Vec2 neighbor) -> b2Vec2 {
		return {
			endpoint.x + (endpoint.x - neighbor.x),
			endpoint.y + (endpoint.y - neighbor.y)
		};
		};

	std::vector<b2Vec2> doubleVerts = verts;
	for (int i = (int)verts.size() - 1; i >= 0; --i)
		doubleVerts.push_back(verts[i]);

	b2ChainDef chain = b2DefaultChainDef();
	chain.points = doubleVerts.data();
	chain.count = (int)doubleVerts.size();
	chain.isLoop = true;  // close the loop between last and first point
	chain.materialCount = 1;
	chain.materials = mat;

	b2ChainId shapeId = b2CreateChain(bodyId, &chain);

	gameObject.mTransform.setPosition(pos);

	return gameObject;
}
