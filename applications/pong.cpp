#include "pong.hpp"
#include "../engine/baseController.hpp"
#include <algorithm>
using namespace led;
template<typename T>int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

Pong::Pong() {}

Pong::~Pong() {}

void Pong::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {   0,   0,   0 }, // Background
    {   0, 255,   0 }, // Ball
    { 255, 255,   0 }, // P1
    {   0, 255, 255 }, // P2
    { 255,   0,   0 }  // Death
  };

  fillPalette(m_colorPalette);

  float speedScale = 5;
  m_playerHeight  = 3;
  m_speedIncrease = 0.01;
  m_movementSpeed = 100;

  m_lastUpdateTime = 0;
  m_deathTime      = 0;
  m_generator      = std::default_random_engine(m_ctrl->getTimeMs());
  m_randFloat      = std::uniform_real_distribution<float>(-1, 1);
  m_ballSpeed      = Pointf(speedScale * m_randFloat(m_generator),
                            speedScale * m_randFloat(m_generator));
  m_ballPos           = Pointf(ctrl->getWidth() / 2, ctrl->getHeight() / 2);
  m_playerPos[0]      = ctrl->getHeight() / 2;
  m_playerPos[1]      = ctrl->getHeight() / 2;
  m_playerWonRound[0] = false;
  m_playerWonRound[1] = false;
}

void Pong::processInput(const BaseInput::InputEvents& events,
                        TimeUnit                      deltaTime) {
  if (BaseInput::isPressed(events, BaseInput::InputEventName::EXIT)) {
    m_hasFinished = true;
    return;
  }

  if (m_deathTime > 0) {
    if (m_ctrl->getTimeMs() - m_deathTime > 3000) {
      m_hasFinished = true;
    }
    return;
  }

  m_ballPos.x += m_ballSpeed.x * deltaTime / 1000.0f;
  m_ballPos.y += m_ballSpeed.y * deltaTime / 1000.0f;

  if (m_ctrl->getTimeMs() - m_lastUpdateTime > m_movementSpeed) {
    m_lastUpdateTime = m_ctrl->getTimeMs();

    for (const auto& e : events) {
      if ((e.state != BaseInput::InputEventState::KEY_PRESSED) &&
          (e.playerId > m_ctrl->getPlayerCount())) continue;

      switch (e.name) {
      case BaseInput::InputEventName::UP:

        if (m_playerPos[e.playerId] > m_playerHeight / 2) m_playerPos[e.playerId]--;
        break;

      case BaseInput::InputEventName::LEFT:
        break;

      case BaseInput::InputEventName::DOWN:

        if (m_playerPos[e.playerId] < m_ctrl->getHeight() - 1 - m_playerHeight /
            2) m_playerPos[e.playerId]++;
        break;

      case BaseInput::InputEventName::RIGHT:
        break;

      case BaseInput::InputEventName::EXIT:
        m_hasFinished = true;
        return;
      }
    }

    if (m_deathTime == 0) {
      m_ballSpeed.x += m_speedIncrease * (m_ballSpeed.x > 0 ? 1 : -1);
      m_ballSpeed.y += m_speedIncrease * (m_ballSpeed.y > 0 ? 1 : -1);
    }

    if ((m_ballPos.x >= m_ctrl->getWidth() / 2) && (m_ctrl->getPlayerCount() == 1)) {
      m_playerPos[1] = std::min(
        std::max(
          m_playerPos[1] + sgn((int)m_ballPos.y - m_playerPos[1]),
          m_playerHeight / 2),
        (int)(m_ctrl->getHeight() - 1 - m_playerHeight / 2));
    }

    // else if (m_autoplay) {
    //   m_playerPos[0] = std::min(
    //     std::max(
    //       m_playerPos[0] + sgn((int)m_ballPos.y - m_playerPos[0]),
    //       m_playerHeight / 2),
    //     (int)(m_ctrl->getHeight() - 1 - m_playerHeight / 2));
    // }
  }


  if (m_ballPos.x < 1) {
    if (fabs(m_ballPos.y - m_playerPos[0]) < m_playerHeight / 2) {
      m_ballSpeed.x = -m_ballSpeed.x;
      m_ballPos.x   = 1;
    }
  } else if (m_ballPos.x > m_ctrl->getWidth() - 2) {
    if (fabs(m_ballPos.y - m_playerPos[1]) < m_playerHeight / 2) {
      m_ballSpeed.x = -m_ballSpeed.x;
      m_ballPos.x   =  m_ctrl->getWidth() - 2;
    }
  }

  if (m_ballPos.x < 0) {
    m_ballSpeed.x       = 0;
    m_ballSpeed.y       = 0;
    m_ballPos.x         = 0;
    m_deathTime         = m_ctrl->getTimeMs();
    m_playerWonRound[1] = true;
  } else if (m_ballPos.x > m_ctrl->getWidth() - 1) {
    m_ballSpeed.x       = 0;
    m_ballSpeed.y       = 0;
    m_ballPos.x         =  m_ctrl->getWidth() - 1;
    m_deathTime         = m_ctrl->getTimeMs();
    m_playerWonRound[0] = true;
  }

  if (m_ballPos.y < 0) {
    m_ballPos.y   = 0;
    m_ballSpeed.y = -m_ballSpeed.y;
  } else if (m_ballPos.y > m_ctrl->getHeight() - 1) {
    m_ballPos.y   = m_ctrl->getHeight() - 1;
    m_ballSpeed.y = -m_ballSpeed.y;
  }
}

void Pong::draw(Image& frame) {
  if (m_playerWonRound[0]) {
    m_ctrl->clearFrame({ 2 });
  } else if (m_playerWonRound[1]) {
    m_ctrl->clearFrame({ 3 });
  } else {
    m_ctrl->clearFrame({ 0 });
  }

  frame.data[(int)round(m_ballPos.x) + (int)round(m_ballPos.y) * frame.width] = 1;

  for (int j = 0; j < 2; j++) {
    for (int i = -m_playerHeight / 2; i <= m_playerHeight / 2; i++) {
      int idx = j * (frame.width - 1) + (m_playerPos[j] + i) * frame.width;
      frame.data[idx] = 2 + j;
    }
  }
}
