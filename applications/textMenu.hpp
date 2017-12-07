#ifndef _H_TEXTMENU_APP_
#define _H_TEXTMENU_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../LEDTableEngine/baseApplication.hpp"
#include "../LEDTableEngine/font.hpp"

class MenuEntryHandler;
class BaseController;

class TextMenu: public BaseApplication {
public:

  struct MenuEntry{
    std::string name;
    std::shared_ptr<MenuEntryHandler> handler;
    MenuEntry(std::string name, std::shared_ptr<MenuEntryHandler> h){
      this->name = name;
      handler = h;
    }
  };

protected:
  enum ColorPaleteIdx {
    BG = 0,
    ARROW=1,
    TEXT=2
  };

  bmfont::Font m_font;
  TimeUnit m_lastStepUpdate;
  TimeUnit m_timeTextWarpInitialWait;
  TimeUnit m_timeTextWarpStart;
  TimeUnit m_timeTextWarpPerChar;
  size_t m_menuEntryIdx;
  Pointi m_currTextSize;
  size_t m_scrollXPixels;
  size_t m_currScrollXPixels;
  bool m_isClosable;
  std::vector<MenuEntry> m_menuEntries;
  size_t m_paddingX;

public:

  TextMenu();
  virtual ~TextMenu ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void continueApp();

  void setMenuItems(std::vector<MenuEntry> entries);

private:

  void updateTextData();
};

class MenuEntryHandler{
public:
  virtual bool onSelectMenuItem(const TextMenu::MenuEntry& menuEntry, size_t idx)=0;
};

class AppLauncher: public MenuEntryHandler{
private:
  std::shared_ptr<BaseApplication> m_app;
  std::shared_ptr<BaseController> m_ctrl;
public:
  AppLauncher(std::shared_ptr<BaseController> ctrl, std::shared_ptr<BaseApplication> app);
  bool onSelectMenuItem(const TextMenu::MenuEntry& menuEntry, size_t idx);
};
#endif
