#include <LEDTableApps/snake.hpp>

#include <algorithm>
#include <sstream>

#include <LEDTableEngine/core/baseController.hpp>
#include <LEDTableEngine/core/messageDisplay.hpp>

using namespace led;
Snake::Snake() {
}

Snake::~Snake() {
}

void Snake::initialize(BaseController *ctrl) {
    BaseApplication::initialize(ctrl);
    m_colorPalette = {
        {    0,   0,   0 },        // Background
        {  210, 245,  60 },        // Food
        {  255,   0,   0 },        // Death
        {  255, 255, 255 },        // Text
        {   70, 240, 240 },        // Snake 1
        {  230,  25, 75  },        // Snake 2
        {  255, 250, 200 },        // Snake 3
        {  255,   0, 255 },        // Snake 4
    };

    fillPalette(m_colorPalette);

    m_generator = std::default_random_engine(m_ctrl->getTimeMs());
    m_posDist   = std::uniform_int_distribution<int>(0, m_ctrl->getSize() - 1);

    m_foodPos.clear();
    m_snakes.clear();

    for (size_t i = 0; i < m_ctrl->getPlayerCount(); i++) {
        SnakeData s;
        s.currMoveDir = Pointi(0, 1);
        s.nextMoveDir = Pointi(0, 1);
        s.snake.push_front(Pointi(3 + m_posDist(m_generator) % (ctrl->getWidth() - 6),
                                  3 + m_posDist(m_generator) % (ctrl->getHeight() - 6)));
        m_snakes.push_back(s);

        m_foodPos.push_back(Pointi(m_posDist(m_generator) % ctrl->getWidth(),
                                   m_posDist(m_generator) % ctrl->getHeight()));
    }

    m_lastUpdateTime = 0;

    m_soundCoin = createAudio("res/audio/sound/coin_flip.wav", 0, SDL_MIX_MAXVOLUME);
    playMusic("res/audio/music/Fantasy_Game_Background_Looping.wav", SDL_MIX_MAXVOLUME * 0.7f);
}

void Snake::deinitialize() {
    freeAudio(m_soundCoin);
}

void Snake::processInput(const BaseInput::InputEvents& events,
                         TimeUnit deltaTime) {
    if (BaseInput::isPressed(events, BaseInput::InputEventName::EXIT)) {
        m_hasFinished = true;
        return;
    }

    std::vector<Pointi> newDirs;

    for (SnakeData& s : m_snakes) {
        newDirs.push_back(s.currMoveDir);
    }

    for (const auto& e : events) {
        if ((e.state != BaseInput::InputEventState::KEY_PRESSED) ||
            (e.playerId >= m_snakes.size())) continue;

        switch (e.name) {
        case BaseInput::InputEventName::UP:
            newDirs[e.playerId].x = 0;
            newDirs[e.playerId].y = -1;
            break;

        case BaseInput::InputEventName::LEFT:
            newDirs[e.playerId].x = -1;
            newDirs[e.playerId].y = 0;
            break;

        case BaseInput::InputEventName::DOWN:
            newDirs[e.playerId].x = 0;
            newDirs[e.playerId].y = 1;
            break;

        case BaseInput::InputEventName::RIGHT:
            newDirs[e.playerId].x = 1;
            newDirs[e.playerId].y = 0;
            break;

        case BaseInput::InputEventName::EXIT:
            m_hasFinished = true;
            return;
        }

        if ((newDirs[e.playerId].x != -m_snakes[e.playerId].currMoveDir.x) ||
            (newDirs[e.playerId].y != -m_snakes[e.playerId].currMoveDir.y)) {
            m_snakes[e.playerId].nextMoveDir = newDirs[e.playerId];
        }
    }

    size_t snakeScoreSum = 0;

    for (SnakeData& s : m_snakes) {
        snakeScoreSum += s.snake.size();
    }
    float speed = 1000.0 / (0.2 * snakeScoreSum / m_snakes.size() + 1);

    if (m_ctrl->getTimeMs() - m_lastUpdateTime < speed) {
        return;
    }

    m_lastUpdateTime = m_ctrl->getTimeMs();
    m_requiresRedraw = true;

    for (size_t i = 0; i < m_snakes.size(); i++) {
        SnakeData& s = m_snakes[i];
        s.currMoveDir = s.nextMoveDir;
        Pointi newPos(s.snake.front().x + s.currMoveDir.x,
                      s.snake.front().y + s.currMoveDir.y);

        bool collision = false;

        for (SnakeData& s2 : m_snakes) {
            collision = std::find(s2.snake.begin(), s2.snake.end(), newPos) != s2.snake.end();

            if (collision) break;
        }

        if ((newPos.x < 0) || (newPos.x >= m_ctrl->getWidth()) ||
            (newPos.y < 0) || (newPos.y >= m_ctrl->getHeight()) ||
            collision) {
            for (int j = m_snakes.size() - 1; j >= 0; j--) {
                SnakeData& s = m_snakes[j];
                auto a       = std::make_shared<led::MessageDisplay>();
                std::stringstream ss;
                ss << "P" << j + 1 << ": " << s.snake.size() << "Pts";
                a->setText(ss.str());
                m_ctrl->addApplication(a, true);
            }
            auto a = std::make_shared<led::MessageDisplay>();
            std::stringstream ss;
            ss << "Player " << i + 1 << " died!";
            a->setText(ss.str());
            m_ctrl->addApplication(a, true);
            m_hasFinished = true;
            return;
        } else {
            s.snake.push_front(newPos);

            auto foodIt = std::find(m_foodPos.begin(), m_foodPos.end(), newPos);

            if (foodIt != m_foodPos.end()) {
                playSoundFromMemory(m_soundCoin, SDL_MIX_MAXVOLUME);
                m_foodPos.erase(foodIt);
                Pointi f;
                int idx = m_posDist(m_generator);
                f.x = idx % m_ctrl->getWidth();
                f.y = idx / m_ctrl->getWidth();
                m_foodPos.push_back(f);
            } else {
                s.snake.pop_back();
            }
        }
    }
}

void Snake::draw(Image& frame) {
    m_requiresRedraw = false;
    m_ctrl->clearFrame(0);

    for (Pointi& food : m_foodPos) {
        frame.data[food.x + food.y * frame.width] = 1;
    }

    for (int i = 0; i < m_snakes.size(); i++) {
        SnakeData& s = m_snakes[i];

        for (Pointi& p : s.snake) {
            frame.data[p.x + p.y * frame.width] = i + 4;
        }
    }
}
