#include "testPatterns.hpp"
#include "../LEDTableEngine/baseController.hpp"

TestPatternApp::TestPatternApp(){

}
TestPatternApp::~TestPatternApp (){

}

void TestPatternApp::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);

  m_generator = std::default_random_engine(m_ctrl->getTimeMs());
  m_colDist = std::uniform_int_distribution<int>(0,255);
  m_posDist = std::uniform_int_distribution<int>(0,m_ctrl->getSize()-1);
  m_patternType = SINGLE_PIXEL;
  m_bufferColorMode = BufferColorMode::RGB;
  m_interpolate = 0;
  m_font.loadFromFile("res/font/myfont.fnt");
  m_lastStepUpdate = 0;
  m_pos = 0;
}

void TestPatternApp::continueApp(){
  m_ctrl->clearFrame({0,0,0});
}
void TestPatternApp::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
    bool modeChanged = false;
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::ENTER)){
      m_patternType = (PatternType)((int)m_patternType + 1);
      if(m_patternType == END)
        m_patternType = (PatternType)0;
      modeChanged = true;
      std::cout << "New Mode: " << m_patternType << std::endl;
    }
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::EXIT)){
      m_hasFinished = true;
      return;
    }

    if(m_ctrl->getTimeMs() - m_lastStepUpdate > 1000)
    {
      m_lastStepUpdate = m_ctrl->getTimeMs();
      m_pos =(m_pos+1) % m_ctrl->getSize();

    }
    switch (m_patternType) {
      case RANDOM:
      break;
      case COLORFADE:
      case COLORFADE_BW:
          m_interpolate+=deltaTime/5000.0;
          if(m_interpolate > 1)
            m_interpolate = 0;
      break;
    }
}
void TestPatternApp::draw(Image &frame){
  switch (m_patternType) {
    case SINGLE_PIXEL:{
      m_ctrl->clearFrame({0,0,0});
      frame.data[3*m_pos] = 255;
      frame.data[3*m_pos+1] = 255;
      frame.data[3*m_pos+2] = 0;
    }
    break;
    case RANDOM:{
      int pos = m_posDist(m_generator);
      int col = m_colDist(m_generator);
      frame.data[3*pos] = col;
      col = m_colDist(m_generator);
      frame.data[3*pos+1] = col;
      col = m_colDist(m_generator);
      frame.data[3*pos+2] = col;
      pos = m_posDist(m_generator);
      frame.data[3*pos] = 0;
      frame.data[3*pos+1] = 0;
      frame.data[3*pos+2] = 0;
    }
    break;
    case COLORFADE:{
      for (size_t i = 0; i < frame.size; i+=3) {
        if(m_interpolate < 0.5){
          frame.data[i] =(1- m_interpolate*2)*255;
          frame.data[i+1] = m_interpolate*2*255;
          frame.data[i+2] =0;
        }else{
          frame.data[i] = 0;
          frame.data[i+1] = (1-(m_interpolate-0.5)*2)*255;
          frame.data[i+2] = ((m_interpolate-0.5)*2)*255;
        }
      }
    }
    break;
    case COLORFADE_BW:
      std::fill(frame.data, frame.data+frame.size, m_interpolate*255);
    break;
    case FONT_TEST:{
      std::string txt = "Test text";
      m_font.draw(frame,txt,{26,254,100});
    }

    break;
  }
}
