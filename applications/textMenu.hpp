#ifndef _H_TEXTMENU_APP_
#define _H_TEXTMENU_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../engine/baseApplication.hpp"
#include "../engine/font.hpp"

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
  TimeUnit m_lastKeyPress;
  bool m_screenOff;
  
  Pointi m_currTextSize;
  size_t m_menuEntryIdx;
  size_t m_scrollXPixels;
  size_t m_currScrollXPixels;
  size_t m_paddingX;

  bool m_isClosable;

  std::vector<MenuEntry> m_menuEntries;

public:

  TextMenu(bool closable=true);
  virtual ~TextMenu ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void continueApp();

  void setMenuItems(std::vector<MenuEntry> entries);

  void updateTextData();

};

class MenuEntryHandler{
public:
  virtual bool onSelectMenuItem(TextMenu* menu, TextMenu::MenuEntry& menuEntry, size_t idx)=0;
};

class AppLauncher: public MenuEntryHandler{
private:
  std::shared_ptr<BaseApplication> m_app;
  std::shared_ptr<BaseController> m_ctrl;
public:
  AppLauncher(std::shared_ptr<BaseController> ctrl, std::shared_ptr<BaseApplication> app);
  bool onSelectMenuItem(TextMenu* menu, TextMenu::MenuEntry& menuEntry, size_t idx);
};
#endif
