#ifndef _H_TEXTMENU_APP_
#define _H_TEXTMENU_APP_


#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/font.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>
#include <LEDTableEngine/core/textMenuItems.hpp>

namespace led {
class BaseController;

/**
 * @brief Text Menu application.
 * Used for launching other applications or changing settings.
 */

class TextMenu : public led::BaseApplication {
protected:
    enum ColorPaleteIdx {
        BG    = 0,
        ARROW = 1,
        TEXT  = 2
    };

/**
 * True, if the menu can be closed
 * Generally false for the main menu!
 */
    bool m_isClosable;

/**
 * Currently selected menu entry
 */
    size_t m_menuEntryIdx;

/**
 * Data structure that contains all menu entries
 */
    std::vector<MenuEntry> m_menuEntries;

/**
 * The current scroll text that has to be displayed
 */
    led::MatrixScrollText m_scrollText;
  /**
   * The name of the application, used for displaying the menu (as submenu).
   */
  std::string m_name;
public:
    /**
     * Create a new menu with a specific name.
     * @param name     Menu name
     * @param closable Can the menu be closed by pressing EXIT?
     */
    TextMenu(std::string name, bool closable = true);
    virtual ~TextMenu();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
    void continueApp();
    /**
     * Set the list of menu itmes that should be displayed.
     */
    void setMenuItems(std::vector<MenuEntry>entries);
    /**
     * Update the scrolling text to the currently selected menu entry
     */
    void updateTextData();

    const std::string getName()  const{
      return m_name;
    }
};
}

#endif // ifndef _H_TEXTMENU_APP_
