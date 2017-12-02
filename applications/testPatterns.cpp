#include "testPatterns.hpp"
#include "../LEDTableEngine/baseController.hpp"

TestPatternApp::TestPatternApp(){

}
TestPatternApp::~TestPatternApp (){

}

void TestPatternApp::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    0,0,0,
    255,0,0,
    255,90,0,
    255,154,0,
    255,206,0,
    255,232,8
  };
  m_generator = std::default_random_engine(m_ctrl->getTimeMs());
  m_colDist = std::uniform_int_distribution<int>(0,m_colorPalette.size()/3-1);
  m_posDist = std::uniform_int_distribution<int>(0,m_ctrl->getSize()-1);
}

void TestPatternApp::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced){
}
void TestPatternApp::draw(std::vector<uint8_t> &frame){
  //m_ctrl->clearFrame(0);
  int pos = m_posDist(m_generator);
  int col = m_colDist(m_generator);
  frame[pos] = col;
  pos = m_posDist(m_generator);
  frame[pos] = 0;
}
