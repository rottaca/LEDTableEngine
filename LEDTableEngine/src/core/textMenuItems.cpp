#include <LEDTableEngine/core/textMenuItems.hpp>
#include <LEDTableEngine/core/textMenu.hpp>
#include <iomanip>
#include <sstream>

namespace led{

MenuEntry::MenuEntry(std::string name, std::shared_ptr<MenuEntryHandler>h) {
    m_name = name;
    m_handler = h;
}
MenuEntry::MenuEntry(std::shared_ptr<AppLauncher> h) {
    m_name = h->getName();
    m_handler    = h;
}

AppLauncher::AppLauncher(std::shared_ptr<BaseController> ctrl,
  std::shared_ptr<BaseApplication> app){
    m_app = app;
    m_ctrl = ctrl;
}
bool AppLauncher::onSelectMenuItem(TextMenu* menu,
                                   MenuEntry& menuEntry,
                                   size_t idx,
                                   const BaseInput::InputEvent& event){
    if(event.name != BaseInput::InputEventName::ENTER) return false;
    m_ctrl->addApplication(m_app, true);
    return false;
}
const std::string AppLauncher::getName() const {
  return m_app->getName();
}


SettingsMenuHandler::SettingsMenuHandler(std::shared_ptr<led::BaseController>ctrl) {
    m_ctrl = ctrl;
}

std::vector<MenuEntry> SettingsMenuHandler::createSettingsMenu(){
  std::vector<MenuEntry> m = {
      MenuEntry(std::string("Brightness: ")+std::to_string(int(100*m_ctrl->getBrightness())), shared_from_this()),
      MenuEntry(std::string("Players: ")+std::to_string(m_ctrl->getPlayerCount()), shared_from_this()),
      MenuEntry(std::string("Min RGB Stretch: ")+std::to_string(int(100*m_ctrl->getStretchMin()[0]/255)), shared_from_this()),
      MenuEntry(std::string("Max RGB Stretch: ")+std::to_string(int(100*m_ctrl->getStretchMax()[0]/255)), shared_from_this())
    };
  return m;
}

bool SettingsMenuHandler::onSelectMenuItem(TextMenu* menu,
                                           MenuEntry& menuEntry,
                                           size_t idx,
                                           const BaseInput::InputEvent& event) {
    std::stringstream stream;
    switch (idx) {
    // Brightness
    case 0: {
        float brightness = m_ctrl->getBrightness();
        switch (event.name) {
          case BaseInput::InputEventName::LEFT:
            brightness -= 0.2;
            if (brightness < 0.2) {
                brightness = 1;
            }
          break;
          case BaseInput::InputEventName::RIGHT:
          case BaseInput::InputEventName::ENTER:
            brightness += 0.2;
            if (brightness > 1) {
                brightness = 0.2;
            }
          break;
        }
        m_ctrl->setBrightness(brightness);
        stream.str("");
        stream << "Brightness: ";
        stream << std::fixed << std::setprecision(1) << brightness*100 << "%";
        menuEntry.setName(stream.str());
        break;
    }

    // Player Count
    case 1: {
        size_t p = m_ctrl->getPlayerCount();
        switch (event.name) {
          case BaseInput::InputEventName::LEFT:
            p--;
            if (p < 1) {
                p = m_ctrl->getMaxPlayerCount();
            }
          break;
          case BaseInput::InputEventName::RIGHT:
          case BaseInput::InputEventName::ENTER:
            p++;
            if (p > m_ctrl->getMaxPlayerCount()) {
                p=1;
            }
          break;
        }
        m_ctrl->setPlayerCount(p);
        stream.str("");
        stream << "Players: ";
        stream << p;
        menuEntry.setName(stream.str());
        break;
    }

    // Min stretch intensity
    case 2: {
        int stretchStep = 5;
        ColorRGB cMin = m_ctrl->getStretchMin();
        ColorRGB cMax = m_ctrl->getStretchMax();
        switch (event.name) {
          case BaseInput::InputEventName::LEFT:
            if (cMin[0] > stretchStep &&
                cMin[1] > stretchStep &&
                cMin[2] > stretchStep) {
              cMin[0]-=stretchStep;
              cMin[1]-=stretchStep;
              cMin[2]-=stretchStep;
            }
          break;
          case BaseInput::InputEventName::RIGHT:
          case BaseInput::InputEventName::ENTER:
            if (cMin[0] < 255-stretchStep &&
                cMin[1] < 255-stretchStep &&
                cMin[2] < 255-stretchStep &&
                cMin[0] < cMax[0]-stretchStep &&
                cMin[1] < cMax[1]-stretchStep &&
                cMin[2] < cMax[2]-stretchStep) {
              cMin[0]+=stretchStep;
              cMin[1]+=stretchStep;
              cMin[2]+=stretchStep;
            }
          break;
        }
        m_ctrl->setStretchMin(cMin);
        stream.str("");
        stream << "Min RGB Stretch: ";
        stream << int(100*cMin[0]/255);
        stream << " %";
        menuEntry.setName(stream.str());
        break;
    }

    // Max stretch intensity
    case 3: {
        int stretchStep = 5;
        ColorRGB cMin = m_ctrl->getStretchMin();
        ColorRGB cMax = m_ctrl->getStretchMax();
        switch (event.name) {
          case BaseInput::InputEventName::LEFT:
            if (cMax[0] > stretchStep &&
                cMax[1] > stretchStep &&
                cMax[2] > stretchStep &&
                cMin[0] < cMax[0]-stretchStep &&
                cMin[1] < cMax[1]-stretchStep &&
                cMin[2] < cMax[2]-stretchStep) {
              cMax[0]-=stretchStep;
              cMax[1]-=stretchStep;
              cMax[2]-=stretchStep;
            }
          break;
          case BaseInput::InputEventName::RIGHT:
          case BaseInput::InputEventName::ENTER:
            if (cMax[0] < 255-stretchStep &&
                cMax[1] < 255-stretchStep &&
                cMax[2] < 255-stretchStep) {
              cMax[0]+=stretchStep;
              cMax[1]+=stretchStep;
              cMax[2]+=stretchStep;
            }
          break;
        }
        m_ctrl->setStretchMax(cMax);
        stream.str("");
        stream << "Max RGB Stretch: ";
        stream << int(100*cMax[0]/255);
        stream << " %";
        menuEntry.setName(stream.str());
        break;
    }

    default:
        return false;
    }
    menu->updateTextData();
    return false;
}

}
