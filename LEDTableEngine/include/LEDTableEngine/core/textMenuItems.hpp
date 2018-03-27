#ifndef _H_TEXT_MENU_ITEMS_
#define _H_TEXT_MENU_ITEMS_


#include <LEDTableEngine/core/baseController.hpp>

namespace led {
class MenuEntryHandler;
class AppLauncher;
class TextMenu;

/**
 * @brief A menu entry, that is displayed inside a menu.
 */
class MenuEntry {

private:
    std::string m_name;
    std::shared_ptr<MenuEntryHandler>m_handler;
public:
    /**
     * Create a menu entry with the specified name and a
     * menu entry handler attached.
     */
    MenuEntry(std::string name,
              std::shared_ptr<MenuEntryHandler>h);
    /**
     * Create a menu entry that acts a an application launcher.
     * This constructor takes the menu name from the app launcher.
     */
    MenuEntry(std::shared_ptr<AppLauncher> h);

    std::string getName(){
      return m_name;
    }

    void setName(std::string name){
      m_name = name;
    }

    std::shared_ptr<MenuEntryHandler> getHandler(){
      return m_handler;
    }
};
/**
 * @brief The MenuEntryHandler is used, to specify the
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
                                  MenuEntry& menuEntry,
                                  size_t idx) = 0;
};

/**
 * @brief General implementation of the menu entry handler interface
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
                          MenuEntry& menuEntry,
                          size_t idx);

    const std::string getName() const;
};
}

#endif // ifndef _H_TEXT_MENU_ITEMS_
