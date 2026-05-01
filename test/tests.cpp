#include <cassert>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include <flow/GameObject.hpp>
#include <flow/Component.hpp>
#include <flow/SceneManager.hpp>
#include <flow/LevelScene.hpp>
#include <flow/components/SpriteRenderer.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/Transform.hpp>

/*
This File is written entirely using Generative AI (specifically Claude)
*/
// Helper macro for assertions
#define ASSERT_TRUE(condition) assert((condition) && "Assertion failed: " #condition)
#define ASSERT_EQ(a, b) assert((a) == (b) && "Assertion failed: values not equal")
#define ASSERT_NE(a, b) assert((a) != (b) && "Assertion failed: values are equal")
#define ASSERT_NEAR(a, b, tolerance) assert(std::abs((a) - (b)) < (tolerance) && "Assertion failed: values differ by more than tolerance")

// Test 1: GameObject default construction and initial state
bool test_GameObjectDefaultConstruction() {
    std::cout << "Running Test 1: GameObject default construction..." << std::endl;

    flow::GameObject gameObject;

    // Verify default state
    ASSERT_NE(&gameObject.mTransform, nullptr);
    ASSERT_EQ(gameObject.mTransform.getPosition().x, 0.0f);
    ASSERT_EQ(gameObject.mTransform.getPosition().y, 0.0f);

    std::cout << "  ✓ Test 1 passed!" << std::endl;
    return true;
}

// Test 2: GameObject transform manipulation
bool test_GameObjectTransformManipulation() {
    std::cout << "Running Test 2: GameObject transform manipulation..." << std::endl;

    flow::GameObject gameObject;
    sf::Vector2f testPos(100.0f, 50.0f);

    gameObject.mTransform.setPosition(testPos);
    ASSERT_EQ(gameObject.mTransform.getPosition().x, testPos.x);
    ASSERT_EQ(gameObject.mTransform.getPosition().y, testPos.y);

    gameObject.mTransform.setRotationDeg(45.0f);
    ASSERT_NEAR(gameObject.mTransform.getRotationDeg(), 45.0f, 0.1f);

    sf::Vector2f testScale(2.0f, 3.0f);
    gameObject.mTransform.setScale(testScale);
    ASSERT_EQ(gameObject.mTransform.getScale().x, testScale.x);
    ASSERT_EQ(gameObject.mTransform.getScale().y, testScale.y);

    std::cout << "  ✓ Test 2 passed!" << std::endl;
    return true;
}

// Test 3: GameObject move semantics
bool test_GameObjectMoveSemantics() {
    std::cout << "Running Test 3: GameObject move semantics..." << std::endl;

    flow::GameObject original;
    original.mTransform.setPosition(sf::Vector2f(50.0f, 75.0f));

    flow::GameObject moved = std::move(original);
    ASSERT_EQ(moved.mTransform.getPosition().x, 50.0f);
    ASSERT_EQ(moved.mTransform.getPosition().y, 75.0f);

    std::cout << "  ✓ Test 3 passed!" << std::endl;
    return true;
}

// Test 4: GameObject component addition - Rigidbody
bool test_GameObjectAddRigidbodyComponent() {
    std::cout << "Running Test 4: GameObject add Rigidbody component..." << std::endl;

    flow::GameObject gameObject;

    // Add a Rigidbody component
    auto& rbComponent = gameObject.addComponent<flow::Rigidbody>();

    // Verify component was added
    auto* retrievedComponent = gameObject.getComponent<flow::Rigidbody>();
    ASSERT_NE(retrievedComponent, nullptr);
    ASSERT_EQ(&rbComponent, retrievedComponent);

    std::cout << "  ✓ Test 4 passed!" << std::endl;
    return true;
}

// Test 5: Component attachment to GameObject preserves mGameObject pointer
bool test_ComponentGameObjectPointerUpdate() {
    std::cout << "Running Test 5: Component GameObject pointer update..." << std::endl;

    flow::GameObject gameObject;

    auto& rbComponent = gameObject.addComponent<flow::Rigidbody>();

    // Verify that the component's mGameObject pointer is set correctly
    ASSERT_EQ(rbComponent.mGameObject, &gameObject);

    std::cout << "  ✓ Test 5 passed!" << std::endl;
    return true;
}

// Test 6: LevelScene construction and add GameObject
bool test_LevelSceneConstructionAndAddGameObject() {
    std::cout << "Running Test 6: LevelScene construction and add GameObject..." << std::endl;

    auto scene = std::make_unique<flow::LevelScene>("test_scene");

    // Create and add a game object
    flow::GameObject testObject;
    testObject.mTransform.setPosition(sf::Vector2f(10.0f, 20.0f));

    auto& addedObject = scene->AddGameObject(std::move(testObject));
    ASSERT_EQ(addedObject.mTransform.getPosition().x, 10.0f);
    ASSERT_EQ(addedObject.mTransform.getPosition().y, 20.0f);

    std::cout << "  ✓ Test 6 passed!" << std::endl;
    return true;
}

// Test 7: LevelScene lifecycle - onEnter and onExit
bool test_LevelSceneLifecycle() {
    std::cout << "Running Test 7: LevelScene lifecycle (onEnter/onExit)..." << std::endl;

    auto scene = std::make_unique<flow::LevelScene>("lifecycle_test");

    // Test onEnter (should not throw)
    try {
        scene->onEnter();
    } catch (...) {
        return false;
    }

    // Test onExit (should not throw)
    try {
        scene->onExit();
    } catch (...) {
        return false;
    }

    std::cout << "  ✓ Test 7 passed!" << std::endl;
    return true;
}

// Test 8: SceneManager singleton pattern
bool test_SceneManagerSingletonPattern() {
    std::cout << "Running Test 8: SceneManager singleton pattern..." << std::endl;

    flow::SceneManager& manager1 = flow::SceneManager::getGlobal();
    flow::SceneManager& manager2 = flow::SceneManager::getGlobal();

    ASSERT_EQ(&manager1, &manager2);

    std::cout << "  ✓ Test 8 passed!" << std::endl;
    return true;
}

// Test 9: SceneManager load and switch scene
bool test_SceneManagerLoadAndSwitchScene() {
    std::cout << "Running Test 9: SceneManager load and switch scene..." << std::endl;

    flow::SceneManager& manager = flow::SceneManager::getGlobal();

    auto newScene = std::make_unique<flow::LevelScene>("test_load_scene_9");

    // Load scene
    bool loadSuccess = manager.loadScene(std::move(newScene));
    ASSERT_TRUE(loadSuccess);

    // Switch to scene
    bool switchSuccess = manager.switchScene("test_load_scene_9");
    ASSERT_TRUE(switchSuccess);

    // Verify current scene
    flow::Scene& currentScene = manager.getCurrentScene();
    ASSERT_EQ(currentScene.get_uuid(), "test_load_scene_9");

    std::cout << "  ✓ Test 9 passed!" << std::endl;
    return true;
}

// Test 10: SceneManager remove scene
bool test_SceneManagerRemoveScene() {
    std::cout << "Running Test 10: SceneManager remove scene..." << std::endl;

    flow::SceneManager& manager = flow::SceneManager::getGlobal();

    auto sceneToRemove = std::make_unique<flow::LevelScene>("scene_to_remove_10");
    manager.loadScene(std::move(sceneToRemove));

    // Remove the scene
    bool removeSuccess = manager.removeScene("scene_to_remove_10");
    ASSERT_TRUE(removeSuccess);

    std::cout << "  ✓ Test 10 passed!" << std::endl;
    return true;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Running Game Engine Test Suite" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    int passed = 0;
    int failed = 0;

    try {
        if (test_GameObjectDefaultConstruction()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 1 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_GameObjectTransformManipulation()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 2 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_GameObjectMoveSemantics()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 3 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_GameObjectAddRigidbodyComponent()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 4 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_ComponentGameObjectPointerUpdate()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 5 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_LevelSceneConstructionAndAddGameObject()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 6 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_LevelSceneLifecycle()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 7 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_SceneManagerSingletonPattern()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 8 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_SceneManagerLoadAndSwitchScene()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 9 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    try {
        if (test_SceneManagerRemoveScene()) passed++;
        else failed++;
    } catch (const std::exception& e) {
        std::cout << "  ✗ Test 10 failed with exception: " << e.what() << std::endl;
        failed++;
    }

    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "           Test Results Summary" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Passed: " << passed << "/10" << std::endl;
    std::cout << "Failed: " << failed << "/10" << std::endl;
    std::cout << "========================================" << std::endl;

    return (failed == 0) ? 0 : -1;
}
