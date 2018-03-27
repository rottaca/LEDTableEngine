#ifndef _H_TEXT_DISPLAY_APP
#define _H_TEXT_DISPLAY_APP

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/font.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>

namespace led {
class MenuEntryHandler;
class BaseController;
/**
 * @brief This class provides a simple interface to display a
 * moving text ontop of a running application.
 * It can be used to display status messages with very little effort.
 * Pressing ENTER or EXIT leaves the application.
 */
class YesNoDisplay : public led::BaseApplication {
public:

protected:

    led::MatrixScrollText m_scrollText;
    std::string m_text;
    bool m_exitWithYes;
public:

    YesNoDisplay();
    virtual ~YesNoDisplay();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);

    /**
     * Set the actual text that should be displayed.
     */
    void setText(std::string text);
    /**
     * Returns true, if the user left the display by pressing
     * ENTER instead of exit.
     */
    bool exitedWithYes();

    const std::string getName()  const{
      return m_text;
    }
};
}

#endif // ifndef _H_TEXT_DISPLAY_APP
