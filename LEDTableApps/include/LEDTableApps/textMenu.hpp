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
        std::string name;
        std::shared_ptr<MenuEntryHandler>handler;
        MenuEntry(std::string name,
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

// / True, if the menu can be closed
// / Generally false for the main menu!
    bool m_isClosable;

// / Currently selected menu entry
    size_t m_menuEntryIdx;

// / Data structure that contains all menu entries
    std::vector<MenuEntry> m_menuEntries;

// / The current scroll text that has to be displayed
    led::MatrixScrollText m_scrollText;

public:

    TextMenu(bool closable = true);
    virtual ~TextMenu();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
    void continueApp();

    void setMenuItems(std::vector<MenuEntry>entries);

    void updateTextData();
};

/**
 * The MenuEntryHandler is used, to specify the
 * behaviour after selecting a menu entry.
 * It is a general interface with a single
 * function.
 */
class MenuEntryHandler {
public:

/**
 * This function is called after a menu item has been selected
 * by the user.
 * @param  menu      A pointer to the corresponding TextMenu Application
 * @param  menuEntry A reference to the selected menu item.
 * @param  idx       Index of this menu item inside the current menu
 * application.
 * @return           Returns true, if the menu should be closed after
 * executing this handler.
 */
    virtual bool onSelectMenuItem(TextMenu            *menu,
                                  TextMenu::MenuEntry& menuEntry,
                                  size_t idx) = 0;
};

/**
 * General implementation of the menu entry handler interface
 * for launching applications by selecting a menu item.
 */
class AppLauncher : public MenuEntryHandler {
private:

    std::shared_ptr<led::BaseApplication> m_app;
    std::shared_ptr<led::BaseController>  m_ctrl;

public:

/**
 * This class implements the menu entry handler interface for launching
 * new applications.
 * @param ctrl Pointer to the controller.
 * @param app  Pointer to an existing application. This application
 *             will be executed if the menu item has been selected.
 */
    AppLauncher(std::shared_ptr<led::BaseController> ctrl,
                std::shared_ptr<led::BaseApplication>app);

    bool onSelectMenuItem(TextMenu            *menu,
                          TextMenu::MenuEntry& menuEntry,
                          size_t idx);
};

#endif // ifndef _H_TEXTMENU_APP_
