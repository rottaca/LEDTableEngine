#include <LEDTableApps/textMenu.hpp>
#include <LEDTableEngine/core/baseController.hpp>

using namespace led;

TextMenu::TextMenu(bool closable){
  m_isClosable = closable;
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

  fillPalette(m_colorPalette);

  m_menuEntryIdx = 0;
  m_standbyAllowed = true;

  m_scrollText.init(m_ctrl->getHeight(), m_ctrl->getWidth(),m_ctrl->getDefaultFont());
  m_scrollText.setColorPalette(2);
  updateTextData();
}

void TextMenu::continueApp(){
  m_ctrl->clearFrame(0);

}
void TextMenu::updateTextData()
{
  m_scrollText.setText(m_menuEntries[m_menuEntryIdx].name);
}

void TextMenu::setMenuItems(std::vector<MenuEntry> entries){
  m_menuEntries = entries;
}

void TextMenu::processInput(const BaseInput::InputEvents &events,
                          TimeUnit deltaTime){
    TimeUnit t = m_ctrl->getTimeMs();
    m_requiresRedraw = m_scrollText.update(m_ctrl->getTimeMs());

    for(auto& e: events){
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

  m_scrollText.draw(frame);

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
