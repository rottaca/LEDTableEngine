#include "textMenu.hpp"
#include "../engine/baseController.hpp"

TextMenu::TextMenu(bool closable){
  m_isClosable = closable;
  m_font.loadFromFile("res/font/myfont.fnt");
}
TextMenu::~TextMenu (){

}

void TextMenu::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_colorPalette = {
    {0,0,0},
    {255,0,0},
    {255,90,0},
    {255,154,0},
    {255,206,0},
    {255,232,8}
  };

  for(int i = m_colorPalette.size(); i <= 255; i++){
    m_colorPalette.push_back({0,0,0});
  }

  m_lastStepUpdate = 0;
  m_timeTextWarpInitialWait = 1000;
  m_timeTextWarpStart = 0;
  m_timeTextWarpPerChar = 100;
  m_menuEntryIdx = 0;
  m_scrollXPixels = 0;
  m_currScrollXPixels = 0;
  m_menuEntryIdx = 0;
  m_paddingX = 3;
  updateTextData();
  m_lastKeyPress = m_ctrl->getTimeMs();
  m_screenOff = false;
}

void TextMenu::continueApp(){
  m_ctrl->clearFrame(0);
}
void TextMenu::updateTextData()
{
  m_currTextSize = m_font.getTextSize(m_menuEntries[m_menuEntryIdx].name);
  m_scrollXPixels = m_currTextSize.x - m_ctrl->getWidth() + 2*m_paddingX;
  m_currScrollXPixels = 0;
  m_timeTextWarpStart = 0;
}

void TextMenu::setMenuItems(std::vector<MenuEntry> entries){
  m_menuEntries = entries;
}

void TextMenu::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
    TimeUnit t = m_ctrl->getTimeMs();

    if(eventsDebounced.size() > 0){
      m_lastKeyPress = t;
      if(m_screenOff){
        m_screenOff = false;
        return;
      }
    }

    if(t - m_lastKeyPress > 10000){
      m_screenOff = true;
      return;
    }else{
      m_screenOff = false;
    }

    if((m_currScrollXPixels != 0 && m_currScrollXPixels != m_scrollXPixels && t-m_timeTextWarpStart >= m_timeTextWarpPerChar) ||
       ((m_currScrollXPixels == 0 || m_currScrollXPixels == m_scrollXPixels) && t-m_timeTextWarpStart >= m_timeTextWarpPerChar + m_timeTextWarpInitialWait))
       {
         m_timeTextWarpStart = t;
         m_currScrollXPixels++;
         if (m_currScrollXPixels > m_scrollXPixels){
           m_currScrollXPixels = 0;
         }
         m_requiresRedraw = true;
       }


    for(auto& e: eventsDebounced){
      if(e.state != BaseInput::InputEventState::KEY_PRESSED){
        continue;
      }
      switch (e.name) {
        case BaseInput::InputEventName::UP:{
          if(m_menuEntryIdx > 0){
            m_menuEntryIdx--;
            updateTextData();
          }
        }
        break;
        case BaseInput::InputEventName::DOWN:{
          if(m_menuEntryIdx < m_menuEntries.size()-1){
            m_menuEntryIdx++;
            updateTextData();
          }
        }
        break;
        case BaseInput::InputEventName::ENTER:{
          if(m_menuEntries[m_menuEntryIdx].handler){
           if(m_menuEntries[m_menuEntryIdx].handler->onSelectMenuItem(this, m_menuEntries[m_menuEntryIdx], m_menuEntryIdx) && m_isClosable)
            m_hasFinished = true;
          }else if(m_isClosable){
            m_hasFinished = true;
          }
        }
        break;
        case BaseInput::InputEventName::EXIT:{
          if(m_isClosable){
            m_hasFinished = true;
          }
        }
        break;
      }
    }
}
void TextMenu::draw(Image &frame){
  m_requiresRedraw = false;

  m_ctrl->clearFrame({ColorPaleteIdx::BG});

  if(m_screenOff)
    return;


  // Upper arrow
  if(m_menuEntryIdx > 0){
    if(frame.width % 2 == 0){
      frame(0, frame.width/2,0) = ColorPaleteIdx::ARROW;
      frame(0, frame.width/2-1,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2-1,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2-2,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2+1,0) = ColorPaleteIdx::ARROW;
    }else{
      frame(0, frame.width/2,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2-1,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2,0) = ColorPaleteIdx::ARROW;
      frame(1, frame.width/2+1,0) = ColorPaleteIdx::ARROW;
    }
  }

  m_font.draw(frame,
    m_menuEntries[m_menuEntryIdx].name,
    m_paddingX-(float)m_currScrollXPixels,
    frame.height/2.0 - m_currTextSize.y/2.0,
    {ColorPaleteIdx::TEXT});

  // Upper arrow
  if(m_menuEntryIdx < m_menuEntries.size()-1){
    if(frame.width % 2 == 0){
      frame(frame.height-2, frame.width/2+1,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-2, frame.width/2,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-2, frame.width/2-1,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-2, frame.width/2-2,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-1, frame.width/2-1,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-1, frame.width/2,0) = ColorPaleteIdx::ARROW;
    }else{
      frame(frame.height-2, frame.width/2-1,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-2, frame.width/2,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-2, frame.width/2+1,0) = ColorPaleteIdx::ARROW;
      frame(frame.height-1, frame.width/2,0) = ColorPaleteIdx::ARROW;
    }
  }
}

AppLauncher::AppLauncher(std::shared_ptr<BaseController> ctrl, std::shared_ptr<BaseApplication> app){
  m_app = app;
  m_ctrl = ctrl;
}
bool AppLauncher::onSelectMenuItem(TextMenu* menu, TextMenu::MenuEntry& menuEntry, size_t idx){
  m_ctrl->addApplication(m_app, true);
  return false;
}
