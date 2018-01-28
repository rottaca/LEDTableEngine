#include "minesweeper.hpp"
#include "../engine/baseController.hpp"
#include <algorithm>
#include "textDisplay.hpp"
#include <sstream>

using namespace led;

#define PAD_X (1)
#define NEIGHBOR_CNT 8
#define COL_IDX_CLOSED 9
#define COL_IDX_MARKED 10
#define COL_IDX_BOMB 11
#define COL_IDX_CURSOR 12

MineSweeper::MineSweeper() {}

MineSweeper::~MineSweeper() {}

void MineSweeper::initialize(BaseController *ctrl) {
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {   255,255,255 }, // 0 Neighbor
    {  31, 140, 255 }, // 1 Neighbor
    {  30, 253, 255 }, // 2 Neighbor
    {  29, 255, 142 }, // 3 Neighbor
    {  29, 255, 28 }, // 4 Neighbor
    {  142, 255, 28 }, // 5 Neighbor
    {  255, 252, 27}, // 6 Neighbor
    {  255, 137, 26 }, // 7 Neighbor
    {  255, 25, 29 }, // 8 Neighbor
    {   0,   0,  0 }, // Closed
    {  255, 27, 192 }, // Marked
    {  255, 0, 0 }, // Bomb
    {  128, 128, 128 }, // Cursor
  };

  fillPalette(m_colorPalette);
  size_t w = m_ctrl->getWidth();
  size_t h = m_ctrl->getHeight();
  m_lastBlinkTime = 0;
  m_cursorVisible = true;
  m_gameOver = false;
  m_generator = std::default_random_engine(m_ctrl->getTimeMs());
  m_posDist   = std::uniform_int_distribution<int>(0, (w-2*PAD_X)*h);
  m_steps = 0;
  m_cursorPos.x = (w-2*PAD_X)/2;
  m_cursorPos.y = h/2;

  initGameField(30,w-2*PAD_X,h);
}
bool MineSweeper::openField(int x, int y){
  size_t w = m_ctrl->getWidth()-2*PAD_X;
  size_t h = m_ctrl->getHeight();

  size_t idx = w*(y) + x;

  if(m_gamefield[idx].opened)
    return false;

  m_gamefield[idx].opened = true;

  if(m_gamefield[idx].bomb)
    return true;

  for (int dy = -1; dy <= 1; dy++) {
    if(y + dy < 0 || y + dy >= h)
      continue;
      for (int dx = -1; dx <= 1; dx++) {
        if(x + dx < 0 || x + dx >= w || (dx == 0 && dy == 0))
          continue;
      size_t idx2 = w*(y+dy) + x + dx;

      if(!m_gamefield[idx2].bomb && !m_gamefield[idx2].opened){
        if(m_gamefield[idx2].neighborBombs == 0){
          openField(x+dx,y+dy);
        }else{
          m_gamefield[idx2].opened = true;
        }
      }
    }
  }
  return false;

}

void MineSweeper::initGameField(size_t bombCnt, size_t w, size_t h){

  m_gamefield.resize(w*h);
  FieldState fs;
  fs.bomb = false;
  fs.marked = false;
  fs.opened = false;
  std::fill(m_gamefield.begin(), m_gamefield.end(),fs);
  for (size_t i = 0; i < bombCnt; i++) {
    size_t idx = m_posDist(m_generator);
    m_gamefield[idx].bomb = true;
  }

  size_t idx = 0;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      uint8_t nBombs = 0;
      for (int dy = -1; dy <= 1; dy++) {
        if(y + dy < 0 || y + dy >= h)
          continue;
        for (int dx = -1; dx <= 1; dx++) {
          if(x + dx < 0 || x + dx >= w || (dx == 0 && dy == 0))
            continue;
          size_t idx2 = idx + w*dy + dx;
          if(m_gamefield[idx2].bomb){
            nBombs++;
          }
        }
      }
      m_gamefield[idx].neighborBombs = nBombs;

      idx++;
    }
  }
}
void MineSweeper::deinitialize() {

}

void MineSweeper::processInput(const BaseInput::InputEvents& events,
                         TimeUnit                      deltaTime) {
  if (BaseInput::isPressed(events, BaseInput::InputEventName::EXIT)) {
    m_hasFinished = true;
    return;
  }

  if(m_ctrl->getTimeMs() - m_lastBlinkTime > 200){
    m_lastBlinkTime = m_ctrl->getTimeMs();
    m_cursorVisible = !m_cursorVisible;
    m_requiresRedraw = true;
  }

  Pointi moveDir(0,0);
  bool open = false;
  bool mark = false;
  for (const auto& e : events) {
    if (e.state != BaseInput::InputEventState::KEY_PRESSED) continue;

    switch (e.name) {
    case BaseInput::InputEventName::UP:
      moveDir.x = 0;
      moveDir.y = -1;
      break;

    case BaseInput::InputEventName::LEFT:
      moveDir.x = -1;
      moveDir.y = 0;
      break;

    case BaseInput::InputEventName::DOWN:
      moveDir.x = 0;
      moveDir.y = 1;
      break;

    case BaseInput::InputEventName::RIGHT:
      moveDir.x = 1;
      moveDir.y = 0;
      break;

    case BaseInput::InputEventName::A:
      mark = true;
      break;

    case BaseInput::InputEventName::ENTER:
      if(m_gameOver){
        m_hasFinished = true;
      }else{
        open = true;
      }
      break;

    case BaseInput::InputEventName::EXIT:
      m_hasFinished = true;
      return;
    }
  }
  m_cursorPos.x += moveDir.x;
  m_cursorPos.y += moveDir.y;

  if(open){
    m_steps++;
    m_gameOver = openField(m_cursorPos.x,m_cursorPos.y);
  } else if(mark){
    size_t w = m_ctrl->getWidth()-2*PAD_X;
    m_gamefield[m_cursorPos.x + m_cursorPos.y*w].marked = !m_gamefield[m_cursorPos.x + m_cursorPos.y*w].marked;
  }else if(m_hasFinished && m_gameOver){
    auto a       = std::make_shared<TextDisplay>();
    std::stringstream ss;
    ss << "Lost after " << m_steps << " steps";
    a->setText(ss.str());
    m_ctrl->addApplication(a, true);
  } else if(gameWon()){
    m_gameOver = true;
    auto a       = std::make_shared<TextDisplay>();
    std::stringstream ss;
    ss << "Won after " << m_steps << " steps";
    a->setText(ss.str());
    m_ctrl->addApplication(a, true);
  }
}

bool MineSweeper::gameWon(){
  for (size_t i = 0; i < m_gamefield.size(); i++) {
    if(!m_gamefield[i].opened){
      return false;
    }
  }
  return true;
}
void MineSweeper::draw(Image& frame) {
  m_requiresRedraw = false;
  m_ctrl->clearFrame(0);

  uint8_t* p = frame.data;
  size_t idx = 0;
  for (size_t y = 0; y < frame.height; y++) {
    int barOffset = (frame.height-NEIGHBOR_CNT)/2;
    if(y >= barOffset && y <= NEIGHBOR_CNT + barOffset)
      *p = y - barOffset;
    p+=PAD_X;
    for (size_t x = 0; x < frame.width-2*PAD_X; x++) {
      if(m_gameOver){
        if(m_gamefield[idx].bomb){
          *p = COL_IDX_BOMB;
        }else{
          *p = COL_IDX_CLOSED;
        }
      }else if (m_gamefield[idx].opened){
        *p = m_gamefield[idx].neighborBombs;
      }else if(m_gamefield[idx].marked){
        *p = COL_IDX_MARKED;
      }else{
        *p = COL_IDX_CLOSED;
      }
      idx++;
      p++;
    }

    if(y >= barOffset && y <= NEIGHBOR_CNT + barOffset)
      *p = y - barOffset;
    p+=PAD_X;
  }

  if(m_cursorVisible){
    frame.data[m_cursorPos.x + PAD_X + m_cursorPos.y*frame.width] = COL_IDX_CURSOR;
  }
}
