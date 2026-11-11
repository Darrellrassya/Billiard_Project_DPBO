#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Ball.hpp"
#include "Table.hpp"

enum class GameState {
    MainMenu,
    Playing,
    GameOver
};

// Slice Minggu 2: hanya window, game loop, state machine, meja, dan
// collision/pocket. Menu penuh, cue, skor, dan fisika bola belum ada.
class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update();
    void render();

    void startMatch();
    void resetBall();
    void loadSystemFont();

    void drawText(const std::string& text, unsigned size, float x, float y,
                  sf::Color color, bool bold = false);
    void drawCenteredText(const std::string& text, unsigned size, float y,
                          sf::Color color, bool bold = false);
    void drawHud();
    void drawMenuOverlay();
    void drawGameOverOverlay();

    sf::RenderWindow window;
    GameState state;
    sf::Font font;
    bool fontLoaded;

    Table table;
    Ball demoBall;
    sf::Vector2f cueBallStartPos;
    int pottedCount;
    float respawnTimer;

    sf::Vector2f menuButtonSize;
    sf::RectangleShape menuButton;

    sf::Clock frameClock;
};
