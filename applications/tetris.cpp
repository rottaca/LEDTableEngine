#include "tetris.hpp"
#include "../engine/baseController.hpp"
#include <algorithm>
#include "textDisplay.hpp"

using namespace led;

const std::vector<Tetris::ShapeDef> shapes = {
{
  // Box
  { Pointi(0, 0), Pointi(1, 0), Pointi(0, 1), Pointi(1, 1) }
},
{
  // L
  { Pointi(0, 0), Pointi(0, 1), Pointi(0, 2), Pointi(1, 2) },
  { Pointi(0, 1), Pointi(1, 1), Pointi(2, 1), Pointi(2, 0) },
  { Pointi(0, 0), Pointi(1, 0), Pointi(1, 1), Pointi(1, 2) },
  { Pointi(0, 1), Pointi(0, 2), Pointi(1, 1), Pointi(2, 1) }
},
{
  // L (mirrored)
  { Pointi(1, 0), Pointi(1, 1), Pointi(0, 2), Pointi(1, 2) },
  { Pointi(0, 1), Pointi(1, 1), Pointi(2, 1), Pointi(2, 2) },
  { Pointi(0, 0), Pointi(0, 1), Pointi(0, 2), Pointi(1, 0) },
  { Pointi(0, 0), Pointi(0, 1), Pointi(1, 1), Pointi(2, 1) }
},
{
  // z
  { Pointi(0, 0), Pointi(1, 0), Pointi(1, 1), Pointi(2, 1) },
  { Pointi(1, 0), Pointi(1, 1), Pointi(0, 1), Pointi(0, 2) },
},
{
  // z (mirrored)
  { Pointi(0, 1), Pointi(1, 0), Pointi(1, 1), Pointi(2, 0) },
  { Pointi(0, 0), Pointi(1, 1), Pointi(0, 1), Pointi(1, 2) },
},
{
  // _|_
  { Pointi(1, 0), Pointi(0, 1), Pointi(1, 1), Pointi(2, 1) },
  { Pointi(1, 0), Pointi(0, 1), Pointi(1, 1), Pointi(1, 2) },
  { Pointi(1, 1), Pointi(0, 0), Pointi(1, 0), Pointi(2, 0) },
  { Pointi(0, 0), Pointi(0, 1), Pointi(0, 2), Pointi(1, 1) },
},
{
  // |
  { Pointi(0, 0), Pointi(1, 0), Pointi(2, 0), Pointi(3, 0) },
  { Pointi(1, 0), Pointi(1, 1), Pointi(1, 2), Pointi(1, 3) }
}
};

Tetris::Tetris() {}

Tetris::~Tetris() {}

void Tetris::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {   0,   0,   0 }, // Background
    {   0,   0, 255 }, // Color 1
    {   0, 255,   0 }, // Color 2
    { 255,   0,   0 }, // Color 3
    { 255, 255,   0 }, // Color 4
    {   0, 255, 255 }, // Color 5
    { 255,   0, 255 }, // Color 6
  };

  fillPalette(m_colorPalette);

  m_gameField.resize(m_ctrl->getHeight(), m_ctrl->getWidth(), 1);
  memset(m_gameField.data, 0, m_gameField.size);
  m_lastUpdateTimeFall = 0;
  m_lastUpdateTimeMove = 0;
  m_generator          = std::default_random_engine(m_ctrl->getTimeMs());
  m_posDist            = std::uniform_int_distribution<int>(0, m_ctrl->getWidth());
  m_soundClick         = createAudio("res/audio/sound/click_x.wav", 0, SDL_MIX_MAXVOLUME);
  m_soundCoin          = createAudio("res/audio/sound/coin_flip.wav", 0, SDL_MIX_MAXVOLUME);
  playMusic("res/audio/music/Tetris.wav", SDL_MIX_MAXVOLUME);
  newShape();
}

void Tetris::deinitialize() {
  freeAudio(m_soundClick);
  freeAudio(m_soundCoin);
}

void Tetris::processInput(const BaseInput::InputEvents& events,
                          TimeUnit                      deltaTime) {
  bool fastDown = false;
  bool rotate   = false;
  int  dX       = 0;

  for (const auto& e : events) {
    switch (e.name) {
    case BaseInput::InputEventName::UP :
      rotate = e.state == BaseInput::InputEventState::KEY_PRESSED;
      break;

    case BaseInput::InputEventName::LEFT:

      if (dX == 0) dX = -1;
      break;

    case BaseInput::InputEventName::DOWN:
      fastDown = true;
      break;

    case BaseInput::InputEventName::RIGHT:

      if (dX == 0) dX = 1;
      break;

    case BaseInput::InputEventName::EXIT:
      m_hasFinished = true;
      return;
    }
  }

  if (rotate) {
    m_fallingShape.rotate();
  }

  bool moveDown = false;

  if (fastDown) {
    if (m_ctrl->getTimeMs() - m_lastUpdateTimeFall > 10.0) {
      moveDown = true;
    }
  } else {
    if (m_ctrl->getTimeMs() - m_lastUpdateTimeFall > 1000.0) {
      moveDown = true;
      playSoundFromMemory(m_soundClick, SDL_MIX_MAXVOLUME);
    }
  }

  if (moveDown) {
    m_lastUpdateTimeFall = m_ctrl->getTimeMs();
    m_fallingShape.translate(0, 1);
  }

  // Check movement to left and right
  if ((dX != 0) && (m_ctrl->getTimeMs() - m_lastUpdateTimeMove > 100.0)) {
    bool moveX = true;

    Pointi& pos = m_fallingShape.pos;

    for (Pointi& p : m_fallingShape.currShape()) {
      if ((p.x + pos.x + dX < 0) || (p.x + pos.x + dX >= m_gameField.width)) {
        moveX = false;
        break;
      }

      if ((pos.y + p.y < 0) || (pos.y + p.y >= m_gameField.height)) continue;

      if (m_gameField.data[pos.x + p.x + dX + m_gameField.width *
                           (pos.y + p.y)] > 0) {
        moveX = false;
        break;
      }
    }

    if (moveX) {
      m_lastUpdateTimeMove = m_ctrl->getTimeMs();
      m_fallingShape.translate(dX, 0);
    }
  }

  m_requiresRedraw = true;

  // Check for collision and add to game field if falling complete
  if (m_fallingShape.isAlive) {
    bool collision = false;

    Pointi& pos = m_fallingShape.pos;

    for (Pointi& p : m_fallingShape.currShape()) {
      if ((pos.x + p.x < 0) || (pos.y + p.y < 0) ||
          (pos.x + p.x >= m_gameField.width)) continue;

      if ((pos.y + p.y >= m_gameField.height - 1) ||
          (m_gameField.data[pos.x + p.x + m_gameField.width *
                            (pos.y + p.y + 1)] > 0)) {
        collision = true;
        break;
      }
    }

    if (collision) {
      for (Pointi& p : m_fallingShape.currShape()) {
        if (pos.y + p.y < 0) {
          auto a = std::make_shared<TextDisplay>();
          a->setText(std::string("Score: ") + std::to_string(m_score));
          m_ctrl->addApplication(a, true);
          m_hasFinished = true;
          return;
        }
      }

      for (Pointi& p : m_fallingShape.currShape()) {
        if ((pos.x + p.x < 0) || (pos.y + p.y < 0) ||
            (pos.x + p.x >= m_gameField.width) ||
            (pos.y + p.y >= m_gameField.height))
        {
          continue;
        }
        m_gameField.data[pos.x + p.x + m_gameField.width *
                         (pos.y + p.y)] = m_fallingShape.colorIdx;
      }
      m_fallingShape.isAlive = false;
      playSoundFromMemory(m_soundCoin, SDL_MIX_MAXVOLUME);
      m_score++;
    }
  }

  // Check for complete lines that can be removed
  for (int y = m_gameField.height - 1; y >= 0; y--) {
    bool lineFull = true;

    for (int x = 0; x < m_gameField.width; x++) {
      if (m_gameField.data[x + m_gameField.width * y] == 0) {
        lineFull = false;
        break;
      }
    }

    if (lineFull) {
      for (int y2 = y; y2 > 0; y2--) {
        memcpy(&m_gameField.data[m_gameField.width * y2],
               &m_gameField.data[m_gameField.width * (y2 - 1)], m_gameField.width);
      }
      memset(m_gameField.data, 0, m_gameField.width);

      // reprocess the current line
      y++;
    }
  }

  // Spawn a new item
  if (!m_fallingShape.isAlive) {
    newShape();
  }
}

void Tetris::draw(Image& frame) {
  m_requiresRedraw = false;
  m_ctrl->clearFrame(0);
  memcpy(frame.data, m_gameField.data, m_gameField.size);

  for (Pointi& p : m_fallingShape.currShape()) {
    int x = p.x + m_fallingShape.pos.x;
    int y = p.y + m_fallingShape.pos.y;

    if ((x < 0) || (x >= frame.width) || (y < 0) || (y >= frame.height)) {
      continue;
    }
    frame.data[x + y * frame.width] = m_fallingShape.colorIdx;
  }
}

void Tetris::newShape() {
  int shapeIdx = m_posDist(m_generator) % shapes.size();
  int posX     = 3 + m_posDist(m_generator) % (m_ctrl->getWidth() - 6);
  int colIdx   = 1 + m_posDist(m_generator) % 6;

  m_fallingShape.update(shapes[shapeIdx], colIdx);
  m_fallingShape.translate(posX, -3);
}
