#include "LevelLoader.hpp"
void LevelLoader::readFile(std::string fileUUID)
{
	std::ifstream file("assets/levels/" + fileUUID + ".txt");
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << fileUUID << std::endl;
		return;
	}

	_ss.str(""); //yes both "" and clear are needed; idk why
	_ss.clear();
	_ss << file.rdbuf();

	file.close();

	std::string line;

	std::string uuid;
	std::getline(_ss, uuid);

	int lvNum;
	std::getline(_ss, line); lvNum = std::stoi(line);

	sf::Color mainColor;
	int r, g, b, a;
	std::getline(_ss, line);
	sscanf(line.c_str(), "%d%d%d%d", &r, &g, &b, &a);
	mainColor = sf::Color(r, g, b, a);

	std::string bgFile;
	std::getline(_ss, bgFile);

	sf::Vector2f playerPos;
	float playerRot;
	std::getline(_ss, line);
	sscanf(line.c_str(), "%f,%f,%f", &playerPos.x, &playerPos.y, &playerRot);

	float gravity;
	std::getline(_ss, line); gravity = std::stof(line);

	std::getline(_ss, line); //blank line
	//rest should be handled in _init() for objects

	_init(gravity, uuid, bgFile, playerPos, playerRot, mainColor);
}

void LevelLoader::_init(const float& grav, const std::string& uuid, const std::string& bgFile,
	const sf::Vector2f& playerPos, const float& playerRot, const sf::Color& color)
{
	flow::PhysicsManager::getGlobal().setGravity(sf::Vector2f(0, grav));
	auto newScene = make_unique<flow::LevelScene>(uuid);

	flow::GameObject bg = flow::GameObject();
	bg.addComponent<flow::SpriteRenderer>(std::string("assets/" + bgFile));
	newScene->AddGameObject(std::move(bg));

	//Object stuff
	std::string shapeLine;
	while (std::getline(_ss, shapeLine)) {
		if (shapeLine.empty()) continue;

		std::stringstream ss(shapeLine);
		std::string type;
		std::getline(ss, type, ';');

		if (type == "curve") {
			std::string posStr, radiusStr, angleStr, arcStr;
			std::getline(ss, posStr, ';');
			std::getline(ss, radiusStr, ';');
			std::getline(ss, angleStr, ';');
			std::getline(ss, arcStr, ';');

			float x, y;
			sscanf(posStr.c_str(), "%f,%f", &x, &y);
			float radius = std::stof(radiusStr);
			float angle = std::stof(angleStr) / 180;
			float arcLength = std::stof(arcStr) / 180;

			newScene->AddGameObject(WallGenerator::GenerateWall(
				{ x, y },
				radius,
				angle * B2_PI,
				arcLength * B2_PI,
				32,
				color
			));
		}
		else if (type == "line") {
			std::string posStr, lengthStr, angleStr;
			std::getline(ss, posStr, ';');
			std::getline(ss, lengthStr, ';');
			std::getline(ss, angleStr, ';');

			float x, y;
			sscanf(posStr.c_str(), "%f,%f", &x, &y);
			float length = std::stof(lengthStr);
			float angle = std::stof(angleStr);

			newScene->AddGameObject(WallGenerator::GenerateWall(
				{ x, y },
				length,
				angle,
				color
			));
		}
	}

	//Player stuff
	flow::GameObject player = flow::GameObject();

	player.mTransform.setPosition(playerPos);
	player.mTransform.setRotationDeg(playerRot);
	player.mTransform.setScale(sf::Vector2f(0.02f, 0.02f));

	player.addComponent<flow::SpriteRenderer>(std::string("assets/player.png"));

	auto& rbComponent = player.addComponent<flow::Rigidbody>();

	// --- Configure the rigidBody's parameters ---
	// Note: You can have multiple collision shapes on a single body!
	b2BodyId bodyId = rbComponent.getBodyId();
	b2Body_SetType(bodyId, b2_dynamicBody); // Make the body dynamic (it moves)
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 0.1f;
	shapeDef.material.friction = 0.f;
	shapeDef.material.restitution = 0.3f;

	// --- get the sprite (we added the SpriteRenderer just above) ---
	auto& sprite = player.getComponent<flow::SpriteRenderer>()->getSprite();
	// --- local bounds = actual texture size in pixels ---
	sf::FloatRect local = sprite.getLocalBounds();

	// --- apply the GameObject transform scale ---
	sf::Vector2f scale = player.mTransform.getScale();

	// --- Box2D box expects half-width and half-height ---
	float radius = std::min(local.size.x * scale.x, local.size.y * scale.y) * 0.3f; // corner radius
	sf::Vector2f halfExtents(local.size.x * scale.x * 0.5f - radius, local.size.y * scale.y * 0.5f - radius);
	std::cout << "Half extents: " << halfExtents.x << ", " << halfExtents.y << std::endl;
	b2Polygon box = b2MakeRoundedBox(halfExtents.x, halfExtents.y, radius);
	b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

	//test for bullet?
	b2Body_SetBullet(player.getComponent<flow::Rigidbody>()->getBodyId(), true);

	player.addComponent<PlayerController>();
	player.getComponent<PlayerController>()->playerStartPos = playerPos;
	player.getComponent<PlayerController>()->playerStartRot = playerRot;

	sf::View view = sf::View({ 0,0 }, { 400, 300 });
	player.addComponent<flow::Camera>(view);

	newScene->AddGameObject(std::move(player));


	// load the scene
	flow::SceneManager::getGlobal().loadScene(std::move(newScene));
	flow::SceneManager::getGlobal().switchScene(uuid);
	return;
}