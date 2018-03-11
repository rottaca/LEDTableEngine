#ifndef _H_TEXTMENU_APP_
#define _H_TEXTMENU_APP_


#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/font.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>


namespace led {
class BaseController;
}
class MenuEntryHandler;

class TextMenu : public led::BaseApplication {
public:

  struct MenuEntry {
    std::string                      name;
    std::shared_ptr<MenuEntryHandler>handler;
    MenuEntry(std::string                      name,
              std::shared_ptr<MenuEntryHandler>h) {
      this->name = name;
      handler    = h;
    }
  };

protected:

  enum ColorPaleteIdx {
    BG    = 0,
    ARROW = 1,
    TEXT  = 2
  };

  led::TimeUnit m_lastKeyPress;

  bool m_screenOff;
  bool m_isClosable;

  size_t m_menuEntryIdx;

  std::vector<MenuEntry> m_menuEntries;
  led::MatrixScrollText  m_scrollText;

public:

  TextMenu(bool closable = true);
  virtual ~TextMenu();

  void initialize(led::BaseController *ctrl);

  void processInput(const led::BaseInput::InputEvents& events,
                    led::TimeUnit                      deltaTime);
  void draw(led::Image& frame);
  void continueApp();

  void setMenuItems(std::vector<MenuEntry>entries);

  void updateTextData();
};

class MenuEntryHandler {
public:

  virtual bool onSelectMenuItem(TextMenu            *menu,
                                TextMenu::MenuEntry& menuEntry,
                                size_t               idx) = 0;
};

class AppLauncher : public MenuEntryHandler {
private:

  std::shared_ptr<led::BaseApplication> m_app;
  std::shared_ptr<led::BaseController>  m_ctrl;

public:

  AppLauncher(std::shared_ptr<led::BaseController> ctrl,
              std::shared_ptr<led::BaseApplication>app);
  bool onSelectMenuItem(TextMenu            *menu,
                        TextMenu::MenuEntry& menuEntry,
                        size_t               idx);
};

#endif // ifndef _H_TEXTMENU_APP_
