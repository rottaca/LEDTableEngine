#include <LEDTableEngine/core/textMenuItems.hpp>
#include <LEDTableEngine/core/textMenu.hpp>

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
bool AppLauncher::onSelectMenuItem(TextMenu* menu, MenuEntry& menuEntry, size_t idx){
    m_ctrl->addApplication(m_app, true);
    return false;
}
const std::string AppLauncher::getName() const {
  return m_app->getName();
}
}
