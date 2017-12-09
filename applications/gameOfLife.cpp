#include "gameOfLife.hpp"
#include "../engine/baseController.hpp"

GameOfLife::GameOfLife(){

}
GameOfLife::~GameOfLife (){

}

void GameOfLife::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_bufferColorMode = BufferColorMode::RGB;
  m_gameField[0].resize(ctrl->getWidth(), ctrl->getHeight(), 1);
  m_gameField[1].resize(ctrl->getWidth(), ctrl->getHeight(), 1);
  for (size_t i = 0; i < m_gameField[0].size; i++) {
    m_gameField[0].data[i] = 0;
    m_gameField[1].data[i] = 0;
  }
  m_currentFieldIdx = 0;

  m_generator = std::default_random_engine(m_ctrl->getTimeMs());
  m_dist = std::uniform_real_distribution<float>(0.0,1.0);
  m_lastUpdateTime = 0;
  randomInitField();
}

void GameOfLife::randomInitField(){
  size_t sz = m_gameField[m_currentFieldIdx].size;
  float livingCellRatio = 0.54;
  for (size_t i = 0; i < sz*livingCellRatio; i++) {
    size_t x = m_dist(m_generator)*m_gameField[m_currentFieldIdx].width;
    size_t y = m_dist(m_generator)*m_gameField[m_currentFieldIdx].height;
    m_gameField[m_currentFieldIdx](y,x,1) = 1;
  }
}

void GameOfLife::continueApp(){
}
void GameOfLife::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::EXIT)){
      m_hasFinished = true;
      return;
    }
    if(m_ctrl->getTimeMs() - m_lastUpdateTime > 500){
      m_lastUpdateTime = m_ctrl->getTimeMs();
    }else{
      return;
    }

    size_t nextField = (m_currentFieldIdx+1)%2;
    bool livingCellFOund = false;
    for (size_t y = 0; y < m_gameField[0].height; y++) {
      for (size_t x = 0; x < m_gameField[0].width; x++) {
        uint8_t c = m_gameField[m_currentFieldIdx](y,x,1);
        uint8_t n = checkNeighbours(m_gameField[m_currentFieldIdx], Pointi(x,y));
        if(c > 0){
          if(n ==2 || n == 3){
            c++;
            livingCellFOund = true;
          }else {
            c = 0;
          }
        }else{
          if(n == 3){
            c = 1;
            livingCellFOund = true;
          }
        }
        m_gameField[nextField](y,x,1) = c;
      }
    }
    if(!livingCellFOund)
      randomInitField();
    m_currentFieldIdx = nextField;

}
void GameOfLife::draw(Image &frame){
  for (size_t i = 0; i < frame.width*frame.height; i++) {
    float r,g,b;
    if(m_gameField[m_currentFieldIdx].data[i] > 0){
      hsv2rgb((int)m_gameField[m_currentFieldIdx].data[i]*360/100, 1, 1, r,g,b);
    }else{
      r = g = b = 0;
    }
    frame.data[3*i] = r*255;
    frame.data[3*i+1] = g*255;
    frame.data[3*i+2] = b*255;
  }
}

uint8_t GameOfLife::checkNeighbours(Image& gameField, Pointi p){
  uint8_t neighbours = 0;
  for (int y = -1; y <= 1; y++) {
    if(p.y + y < 0 || p.x + y >= gameField.height)
      continue;
    for (int x = -1; x <= 1; x++) {
      if(p.x + x < 0 || p.x + x >= gameField.width || (x == 0 && y == 0))
        continue;
      neighbours += (gameField(p.y+y, p.x+x, 1)>0) ;
    }
  }
  return neighbours;
}
