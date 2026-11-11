// Demo slice Minggu 2.
// Diadaptasi dari src/Game.cpp pada commit c254188.
// Fokus: window, game loop, state machine, meja, wall collision, pocket.
// Menu penuh, cue, scoreboard, dan fisika bola minggu 3+ sengaja dihilangkan.

#include "Game.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace {
constexpr float kWindowWidth = 1080.f;
constexpr float kWindowHeight = 660.f;
constexpr float kBallSpeed = 320.f;
constexpr float kRespawnDelay = 0.8f;

const sf::Color kInk(15, 23, 42);
const sf::Color kYellow(234, 179, 8);
const sf::Color kOrange(217, 119, 6);
const sf::Color kPaper(248, 250, 252);
const sf::Color kMuted(148, 163, 184);
const sf::Color kCyan(56, 189, 248);
} // namespace

Game::Game()
    : window(sf::VideoMode(1080, 660),
             "8-Ball Pool Arena - Proyek DPBO (C++ & SFML)",
             sf::Style::Titlebar | sf::Style::Close),
      state(GameState::MainMenu),
      fontLoaded(false),
      table(130.f, 125.f, 860.f, 430.f),
      demoBall(0, 130.f + 860.f * 0.25f, 125.f + 430.f * 0.5f,
               sf::Color(248, 250, 252), 14.f),
      cueBallStartPos(130.f + 860.f * 0.25f, 125.f + 430.f * 0.5f),
      pottedCount(0),
      respawnTimer(0.f),
      menuButtonSize(260.f, 64.f) {
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    loadSystemFont();

    menuButton.setSize(menuButtonSize);
    menuButton.setPosition((kWindowWidth - menuButtonSize.x) * 0.5f, 400.f);
    menuButton.setFillColor(kYellow);
    menuButton.setOutlineThickness(4.f);
    menuButton.setOutlineColor(kInk);
}

void Game::loadSystemFont() {
    std::vector<std::string> fontPaths = {
        "assets/fonts/font.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            break;
        }
    }
}

void Game::startMatch() {
    pottedCount = 0;
    respawnTimer = 0.f;
    resetBall();
    state = GameState::Playing;
}

void Game::resetBall() {
    demoBall.reset(cueBallStartPos.x, cueBallStartPos.y);
    respawnTimer = 0.f;
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else if (state == GameState::MainMenu &&
                       (event.key.code == sf::Keyboard::Enter ||
                        event.key.code == sf::Keyboard::Space)) {
                startMatch();
            } else if (state == GameState::Playing) {
                if (event.key.code == sf::Keyboard::R) {
                    resetBall();
                } else if (event.key.code == sf::Keyboard::G) {
                    state = GameState::GameOver;
                } else if (event.key.code == sf::Keyboard::Space) {
                    demoBall.velocity = {0.f, 0.f};
                } else {
                    // Satu tekanan panah = satu dorongan. Arah diambil dari
                    // event, bukan isKeyPressed, agar tetap bekerja saat key
                    // sudah dilepas sebelum frame berikutnya.
                    sf::Vector2f dir(0.f, 0.f);
                    switch (event.key.code) {
                        case sf::Keyboard::Left:
                        case sf::Keyboard::A:
                            dir.x = -1.f;
                            break;
                        case sf::Keyboard::Right:
                        case sf::Keyboard::D:
                            dir.x = 1.f;
                            break;
                        case sf::Keyboard::Up:
                        case sf::Keyboard::W:
                            dir.y = -1.f;
                            break;
                        case sf::Keyboard::Down:
                        case sf::Keyboard::S:
                            dir.y = 1.f;
                            break;
                        default:
                            break;
                    }
                    if (dir.x != 0.f || dir.y != 0.f) {
                        demoBall.velocity = dir * kBallSpeed;
                    }
                }
            } else if (state == GameState::GameOver &&
                       event.key.code == sf::Keyboard::R) {
                startMatch();
            }
        }

        if (state == GameState::MainMenu &&
            event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                               static_cast<float>(event.mouseButton.y));
            if (menuButton.getGlobalBounds().contains(mouse)) {
                startMatch();
            }
        }
    }
}

void Game::update() {
    float dt = std::min(frameClock.restart().asSeconds(), 0.05f);

    if (state != GameState::Playing) {
        return;
    }

    if (respawnTimer > 0.f) {
        respawnTimer -= dt;
        if (respawnTimer <= 0.f) {
            resetBall();
        }
        return;
    }

    if (!demoBall.active) {
        return;
    }

    demoBall.shape.move(demoBall.velocity * dt);
    table.handleWallCollision(demoBall);

    if (table.isBallPotted(demoBall)) {
        demoBall.active = false;
        demoBall.velocity = {0.f, 0.f};
        ++pottedCount;
        respawnTimer = kRespawnDelay;
    }
}

void Game::drawText(const std::string& text, unsigned size, float x, float y,
                    sf::Color color, bool bold) {
    if (!fontLoaded) {
        return;
    }
    sf::Text label;
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    if (bold) {
        label.setStyle(sf::Text::Bold);
    }
    label.setPosition(x, y);
    window.draw(label);
}

void Game::drawCenteredText(const std::string& text, unsigned size, float y,
                            sf::Color color, bool bold) {
    if (!fontLoaded) {
        return;
    }
    sf::Text label;
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    if (bold) {
        label.setStyle(sf::Text::Bold);
    }
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width * 0.5f,
                    bounds.top + bounds.height * 0.5f);
    label.setPosition(kWindowWidth * 0.5f, y);
    window.draw(label);
}

void Game::drawHud() {
    if (state == GameState::MainMenu) {
        return;
    }

    sf::RectangleShape info({330.f, 62.f});
    info.setPosition(20.f, 20.f);
    info.setFillColor(sf::Color(15, 23, 42, 235));
    info.setOutlineThickness(3.f);
    info.setOutlineColor(kYellow);
    window.draw(info);

    drawText("MINGGU 02 / 07", 11, 34.f, 31.f, kYellow, true);
    drawText("Window 1080 x 660  -  60 FPS", 15, 34.f, 49.f, kPaper, true);

    sf::RectangleShape score({190.f, 62.f});
    score.setPosition(870.f, 20.f);
    score.setFillColor(sf::Color(15, 23, 42, 235));
    score.setOutlineThickness(3.f);
    score.setOutlineColor(kOrange);
    window.draw(score);

    drawText("POTTED", 11, 886.f, 31.f, kMuted, true);
    drawText(std::to_string(pottedCount), 22, 886.f, 46.f, kPaper, true);

    if (respawnTimer > 0.f) {
        drawText("BOLA MASUK LUBANG", 13, 690.f, 32.f, kOrange, true);
    }

    sf::RectangleShape bar({1040.f, 38.f});
    bar.setPosition(20.f, 606.f);
    bar.setFillColor(sf::Color(15, 23, 42, 235));
    bar.setOutlineThickness(3.f);
    bar.setOutlineColor(kCyan);
    window.draw(bar);

    drawText("PANAH/WASD  luncurkan   |   SPACE  stop   |   R  reset   |   "
             "G  game over   |   ESC  keluar",
             13, 36.f, 615.f, kPaper, false);
}

void Game::drawMenuOverlay() {
    sf::RectangleShape veil({kWindowWidth, kWindowHeight});
    veil.setFillColor(sf::Color(15, 23, 42, 175));
    window.draw(veil);

    sf::RectangleShape panel({760.f, 400.f});
    panel.setPosition((kWindowWidth - 760.f) * 0.5f, 110.f);
    panel.setFillColor(sf::Color(15, 23, 42, 225));
    panel.setOutlineThickness(4.f);
    panel.setOutlineColor(kYellow);
    window.draw(panel);

    drawCenteredText("MINGGU 02 / 07", 13, 150.f, kYellow, true);
    drawCenteredText("WINDOW & MEJA", 52, 205.f, kPaper, true);
    drawCenteredText("Demo slice kode utama - SFML 2.6", 16, 255.f, kCyan, false);
    drawCenteredText(
        "Window 1080 x 660  |  game loop  |  meja 4 lapis  |  6 pocket",
        13, 285.f, kMuted, false);

    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    bool hover = menuButton.getGlobalBounds().contains(
        static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    menuButton.setFillColor(hover ? sf::Color(253, 230, 138) : kYellow);
    window.draw(menuButton);
    drawCenteredText("MAIN SEKARANG", 22, 432.f, kInk, true);

    drawCenteredText("atau tekan ENTER  -  ESC untuk keluar", 13, 500.f, kMuted,
                     false);
}

void Game::drawGameOverOverlay() {
    sf::RectangleShape veil({kWindowWidth, kWindowHeight});
    veil.setFillColor(sf::Color(15, 23, 42, 215));
    window.draw(veil);

    sf::RectangleShape panel({620.f, 260.f});
    panel.setPosition((kWindowWidth - 620.f) * 0.5f, 190.f);
    panel.setFillColor(sf::Color(15, 23, 42, 245));
    panel.setOutlineThickness(4.f);
    panel.setOutlineColor(kOrange);
    window.draw(panel);

    drawCenteredText("STATE: GAME OVER", 13, 230.f, kOrange, true);
    drawCenteredText("GAME OVER", 52, 290.f, kPaper, true);
    drawCenteredText("Simulasi transisi state dari Playing.", 15, 345.f, kMuted,
                     false);
    drawCenteredText("R  main lagi   -   ESC  keluar", 16, 395.f, kCyan, true);
}

void Game::render() {
    window.clear(sf::Color(15, 23, 42));

    table.draw(window);
    if (demoBall.active) {
        demoBall.draw(window, fontLoaded ? &font : nullptr, fontLoaded);
    }

    drawHud();

    if (state == GameState::MainMenu) {
        drawMenuOverlay();
    } else if (state == GameState::GameOver) {
        drawGameOverOverlay();
    }

    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}
