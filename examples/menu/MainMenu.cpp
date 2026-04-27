#include "MainMenu.hpp"
#include "flow/Renderer.hpp"

MainMenu::MainMenu()
    : flow::Scene("main-menu"),
      _curMenuPosition(0)
{
    mGameObject = new flow::GameObject();
}

void MainMenu::initialize()
{
    // --- Fonts ---
    if (!_headerFont.openFromFile("assets/Pixel-Regular.ttf")) { }
    if (!_buttonFont.openFromFile("assets/ButtonText.TTF")) { }

    // --- Construct texts ---
    _headerText = std::make_unique<sf::Text>(_headerFont);
    _playText   = std::make_unique<sf::Text>(_buttonFont);
    _exitText   = std::make_unique<sf::Text>(_buttonFont);

    // --- Header ---
    _headerText->setString("MY GAME");
    _headerText->setCharacterSize(64);
    _headerText->setFillColor(sf::Color::White);
    sf::FloatRect headerBounds = _headerText->getLocalBounds();
    _headerText->setOrigin(sf::Vector2f(headerBounds.size.x / 2.f, headerBounds.size.y / 2.f));
    _headerText->setPosition(sf::Vector2f(400.f, 120.f));

    // --- Play Button ---
    _playButton.setSize(sf::Vector2f(240.f, 60.f));
    _playButton.setFillColor(sf::Color(50, 150, 50));
    _playButton.setOutlineColor(sf::Color::White);
    _playButton.setOutlineThickness(2.f);
    _playButton.setOrigin(sf::Vector2f(120.f, 30.f));
    _playButton.setPosition(sf::Vector2f(400.f, 300.f));

    _playText->setString("Play");
    _playText->setCharacterSize(28);
    _playText->setFillColor(sf::Color::White);
    sf::FloatRect playBounds = _playText->getLocalBounds();
    _playText->setOrigin(sf::Vector2f(playBounds.size.x / 2.f, playBounds.size.y / 2.f));
    _playText->setPosition(sf::Vector2f(400.f, 297.f));

    // --- Exit Button ---
    _exitButton.setSize(sf::Vector2f(240.f, 60.f));
    _exitButton.setFillColor(sf::Color(150, 50, 50));
    _exitButton.setOutlineColor(sf::Color::White);
    _exitButton.setOutlineThickness(2.f);
    _exitButton.setOrigin(sf::Vector2f(120.f, 30.f));
    _exitButton.setPosition(sf::Vector2f(400.f, 400.f));

    _exitText->setString("Exit");
    _exitText->setCharacterSize(28);
    _exitText->setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = _exitText->getLocalBounds();
    _exitText->setOrigin(sf::Vector2f(exitBounds.size.x / 2.f, exitBounds.size.y / 2.f));
    _exitText->setPosition(sf::Vector2f(400.f, 397.f));

    // --- Register draw (still created here for cases where initialize is called before onEnter) ---
    if (!_menuDrawable)
    {
        _menuDrawable = std::make_unique<MenuDrawable>();
        _menuDrawable->add([this]() -> const sf::Drawable* { return &_playButton; });
        _menuDrawable->add([this]() -> const sf::Drawable* { return &_exitButton; });
        _menuDrawable->add([this]() -> const sf::Drawable* { return _headerText.get(); });
        _menuDrawable->add([this]() -> const sf::Drawable* { return _playText.get(); });
        _menuDrawable->add([this]() -> const sf::Drawable* { return _exitText.get(); });
    }
}

void MainMenu::onEnter()
{
    _curMenuPosition = 0;

    // Ensure the drawable is created before registering with the renderer
    if (!_menuDrawable)
    {
        _menuDrawable = std::make_unique<MenuDrawable>();
        _menuDrawable->add([this]() -> const sf::Drawable* { return &_playButton; });
        _menuDrawable->add([this]() -> const sf::Drawable* { return &_exitButton; });
        _menuDrawable->add([this]() -> const sf::Drawable* { return _headerText.get(); });
        _menuDrawable->add([this]() -> const sf::Drawable* { return _playText.get(); });
        _menuDrawable->add([this]() -> const sf::Drawable* { return _exitText.get(); });
    }

    flow::Renderer::getGlobalRenderer().addRenderable(this);
}

void MainMenu::onExit()
{
    flow::Renderer::getGlobalRenderer().removeRenderable(this);
}

void MainMenu::update(float dt)
{
    if (_playText && _exitText)
    {
        _playButton.setOutlineColor(_curMenuPosition == 0 ? sf::Color::Yellow : sf::Color::White);
        _exitButton.setOutlineColor(_curMenuPosition == 1 ? sf::Color::Yellow : sf::Color::White);
    }
}

const sf::Drawable& MainMenu::getDrawable()
{
    return *_menuDrawable;
}