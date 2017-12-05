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

class TextMenu: public BaseApplication {
public:

  struct MenuEntry{
    std::string name;
    MenuEntryHandler* handler;
    MenuEntry(std::string name, MenuEntryHandler* h){
      this->name = name;
      handler = h;
    }
  };

protected:
  bmfont::Font m_font;
  TimeUnit m_lastStepUpdate;
  TimeUnit m_timeTextWarpInitialWait;
  TimeUnit m_timeTextWarpStart;
  TimeUnit m_timeTextWarpPerChar;
  size_t m_menuEntryIdx;
  Point m_currTextSize;
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
  virtual bool onSelectMenuItem(const TextMenu::MenuEntry& menuEntry, size_t idx)=0;
};

#endif
