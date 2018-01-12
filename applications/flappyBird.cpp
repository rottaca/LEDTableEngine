#include "flappyBird.hpp"
#include "../engine/baseController.hpp"
#include <algorithm>
#include "textDisplay.hpp"
using namespace led;
FlappyBird::FlappyBird() {}

FlappyBird::~FlappyBird() {}

void FlappyBird::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {   0,   0, 0 }, // Background
    {   0, 255, 0 }, // Bird
    { 255,   0, 0 }, // Wall
    { 255,   0, 0 }, // Death
    { 255, 255,255 }                    // Text
  };

  fillPalette(m_colorPalette);

  m_birdY      = ctrl->getHeight() / 2.0;
  m_birdYSpeed = 0;
  m_walls.clear();
  m_birdX          = 1;
  m_lastUpdateTime = 0;
  m_score          = 0;
  m_generator      = std::default_random_engine(m_ctrl->getTimeMs());
  m_posDist        = std::uniform_int_distribution<int>(0,
                                                        m_ctrl->getHeight() - 1);
}

void FlappyBird::processInput(const BaseInput::InputEvents& events,
                              TimeUnit                      deltaTime) {
  for (const auto& e : events) {
    switch (e.name) {
    case BaseInput::InputEventName::UP :
      m_birdYSpeed -= 20 * deltaTime / 1000.0;
      break;

    case BaseInput::InputEventName::EXIT:
      m_hasFinished = true;
      return;
    }
  }
  m_birdYSpeed += 5 * deltaTime / 1000.0;

  if (m_birdYSpeed < -5) {
    m_birdYSpeed = -5;
  } else if (m_birdYSpeed > 5) {
    m_birdYSpeed = 5;
  }

  m_birdY += m_birdYSpeed * deltaTime / 1000.0;

  if (m_birdY < 0) {
    m_birdY = 0;
    auto a = std::make_shared<TextDisplay>();
    a->setText(std::string("Score: ") + std::to_string(m_score));
    m_ctrl->addApplication(a, true);
    m_hasFinished = true;
  } else if (m_birdY > m_ctrl->getHeight() - 1) {
    m_birdY = m_ctrl->getHeight() - 1;
    auto a = std::make_shared<TextDisplay>();
    a->setText(std::string("Score: ") + std::to_string(m_score));
    m_ctrl->addApplication(a, true);
    m_hasFinished = true;
  }

  if (m_ctrl->getTimeMs() - m_lastUpdateTime > 2000.0 / (0.2 * m_score + 1)) {
    m_lastUpdateTime = m_ctrl->getTimeMs();
    Wall w;
    ssize_t openingHeight =  3 + m_posDist(m_generator) % 3 + 1 -
                            m_ctrl->getHeight() % 2;
    w.openingYMin = m_posDist(m_generator) % (m_ctrl->getHeight() - openingHeight);
    w.openingYMax = w.openingYMin + openingHeight;
    w.posX        =  m_ctrl->getWidth() - 1;
    m_walls.push_back(w);
  }

  for (auto& w : m_walls) {
    w.posX -= 2 * deltaTime / 1000.0;
  }

  for (auto it = m_walls.begin(); it != m_walls.end();) {
    if (it->posX < -0.5) {
      it = m_walls.erase(it);
      m_score++;
    } else {
      ++it;
    }
  }

  if ((m_walls.size() > 0) &&
      (fabs(m_walls.front().posX - m_birdX) < 1) &&
      ((round(m_birdY) < m_walls.front().openingYMin) ||
       (round(m_birdY) > m_walls.front().openingYMax))) {
    auto a = std::make_shared<TextDisplay>();
    a->setText(std::string("Score: ") + std::to_string(m_score));
    m_ctrl->addApplication(a, true);
    m_hasFinished = true;
  }
}

void FlappyBird::draw(Image& frame) {
  m_ctrl->clearFrame(0);

  for (auto& w : m_walls) {
    for (int i = 0; i < w.openingYMin; i++) {
      frame.data[(int)round(w.posX) + i * frame.width] = 2;
    }

    for (int i = w.openingYMax + 1; i < frame.height; i++) {
      frame.data[(int)round(w.posX) + i * frame.width] = 2;
    }
  }

  frame.data[m_birdX + (int)round(m_birdY) * frame.width] = 1;
}
