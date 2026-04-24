#include "WallGenerator.hpp"
#include <flow/components/SpriteRenderer.hpp>
#include <flow/components/Rigidbody.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>        // for sf::Image
#include <SFML/System/Vector2.hpp>       // for sf::Vector2f / Vector2u (avoid transitive-include warning)
#include <iostream>

flow::GameObject WallGenerator::GenerateWall(int length, float angle, sf::Color color)
{
	auto gameObject = flow::GameObject();

	// generate an image texture
	sf::Image image(sf::Vector2u(length, 1), color);
	sf::Texture texture;
	if (!texture.loadFromImage(image))
	{
		std::cerr << "Failed to load texture from image\n";
	}

	auto srComponent = std::make_unique<flow::SpriteRenderer>(texture); // create a sprite renderer component
	gameObject.addComponent(std::move(srComponent)); // move the component into the object

	// Create a rigidbody component ---
	auto rbComponent = std::make_unique<flow::Rigidbody>(); // create a rigidbody

	// --- Configure the rigidBody's parameters ---
	// Note: You can have multiple collision shapes on a single body!
	b2BodyId bodyId = rbComponent->getBodyId();
	b2Body_SetType(bodyId, b2_staticBody); // static body
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.1f;
	shapeDef.material.friction = 0.f;
	shapeDef.material.restitution = 0.f;

	// --- get the sprite (we added the SpriteRenderer just above) ---
	auto& sprite = gameObject.getComponent<flow::SpriteRenderer>()->getSprite();
	// --- local bounds = actual texture size in pixels ---
	sf::FloatRect local = sprite.getLocalBounds();

	// --- apply the GameObject transform scale ---
	sf::Vector2f scale = gameObject.mTransform.getScale();

	// --- Box2D box expects half-width and half-height ---
	sf::Vector2f halfExtents(local.size.x * scale.x * 0.5f, local.size.y * scale.y * 0.5f);
	std::cout << "Half extents: " << halfExtents.x << ", " << halfExtents.y << std::endl;
	b2Polygon box = b2MakeBox(halfExtents.x, halfExtents.y);

	// Attach it to the existing bodyId
	b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

	gameObject.addComponent(std::move(rbComponent)); // move the component into the object
	gameObject.mTransform.setRotationDeg(angle);

	return gameObject;
}

flow::GameObject WallGenerator::GenerateWall(int radius, float angle, float arcLength, int subdivisions, sf::Color color)
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

	// draw line segments into the image while building verts
	auto toImgX = [&](float vx) { return static_cast<int>(std::lround(cx + vx)); };
	auto toImgY = [&](float vy) { return static_cast<int>(std::lround(cy + vy)); };

	std::vector<sf::Vector2f> points;
	points.reserve(count);
	for (int i = 0; i < count; ++i)
	{
		float t = static_cast<float>(i) / static_cast<float>(count - 1);
		float theta = angle + arcLength * t; // angle and arcLength are in radians
		float x = std::cos(theta) * static_cast<float>(radius);
		float y = std::sin(theta) * static_cast<float>(radius);
		points.emplace_back(x, y);
		verts.push_back({ x, y });
	}

	// rasterize segments onto the image
	sf::Color drawColor = sf::Color::White;
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
			// safe check
			if (ix >= 0 && ix < (int)image.getSize().x && iy >= 0 && iy < (int)image.getSize().y)
			{
				image.setPixel({ ix, iy }, drawColor);
			}
		}
	}

	// create texture from image
	sf::Texture texture;
	if (!texture.loadFromImage(image))
	{
		std::cerr << "Failed to load texture from image\n";
	}

	auto srComponent = std::make_unique<flow::SpriteRenderer>(texture); // create a sprite renderer component
	gameObject.addComponent(std::move(srComponent)); // move the component into the object

	// Create a rigidbody component ---
	auto rbComponent = std::make_unique<flow::Rigidbody>(); // create a rigidbody

	// --- Configure the rigidBody's parameters ---
	// A wall should be static
	b2BodyId bodyId = rbComponent->getBodyId();
	b2Body_SetType(bodyId, b2_staticBody);

	b2SurfaceMaterial mat[1]; 
	mat[0] = b2DefaultSurfaceMaterial();
	mat[0].friction = 0.0f;
	mat[0].restitution = 0.0f;

	// Create a chain shape from the sampled vertices
	b2ChainDef chain = b2DefaultChainDef();
	chain.points = verts.data();
	chain.count = (int)verts.size();
	chain.materialCount = 1;
	chain.materials = mat;
	b2ChainId shapeId = b2CreateChain(bodyId, &chain);

	gameObject.addComponent(std::move(rbComponent)); // move the component into the object

	return gameObject;
}
