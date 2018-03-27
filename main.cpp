#include <iomanip>

#include "LEDTableEngine/configuration.hpp"
#include "LEDTableEngine/controllers/desktopController.hpp"
#include "LEDTableEngine/controllers/matrixController.hpp"
#include "LEDTableEngine/core/audio.hpp"
#include "LEDTableEngine/core/textMenu.hpp"
#include "LEDTableEngine/core/textMenuItems.hpp"

#include "LEDTableEngine/inputHandlers/keyboardInput.hpp"
#ifdef LED_HAVE_I2C_H_
  # include "LEDTableEngine/inputHandlers/gameControllerInput.hpp"
#endif // ifdef LED_HAVE_I2C_H_

#include "LEDTableApps/update.hpp"
#include "LEDTableApps/testPatterns.hpp"
#include "LEDTableApps/snake.hpp"
#include "LEDTableApps/imageSlideShow.hpp"
#include "LEDTableApps/pong.hpp"
#include "LEDTableApps/fractal.hpp"
#include "LEDTableApps/animations.hpp"
#include "LEDTableApps/boardGameApp.hpp"
#include "LEDTableApps/gameOfLife.hpp"
#include "LEDTableApps/flappyBird.hpp"
#include "LEDTableApps/tetris.hpp"
#include "LEDTableApps/minesweeper.hpp"
#include "LEDTableApps/clock.hpp"
#include "LEDTableApps/connectFour.hpp"
#include "LEDTableApps/shutdownApp.hpp"

extern char *optarg;
extern int optind, opterr, optopt;

using namespace led;
using namespace ledApps;


class SettingsMenuHandler : public MenuEntryHandler {
private:
    std::shared_ptr<led::BaseController> m_ctrl;
public:
    SettingsMenuHandler(std::shared_ptr<led::BaseController>ctrl);
    bool onSelectMenuItem(TextMenu* menu,  MenuEntry& menuEntry, size_t idx);
};

void help(char* appName){
  std::cout << "Usage:" << std::endl;
  std::cout << appName << " -i <i2c|keyboard> - f <i2c/keyboard file> [-d]" << std::endl;
  std::cout << std::endl;
  std::cout << "Parameters: " << std::endl;
  std::cout << "   -i <i2c|keyboard>      : The input handler type. Either i2c controllers" << std::endl;
  std::cout << "                          : or a keyboard (required root permissions)." << std::endl;
  std::cout << "   -f <i2c/keyboard file> : The input device file. In case of the i2c controllers," << std::endl;
  std::cout << "                            this is the i2c device file e.g. /dev/i2c-1" << std::endl;
  std::cout << "                            In case of a keyboard, this is the keyboard event file e.g. " << std::endl;
  std::cout << "                            /dev/input/by-path/platform-i8042-serio-0-event-kbd" << std::endl;
  std::cout << "   -d                     : Enable debug printing," << std::endl;
  exit(1);
}

int main(int argc, char **argv)
{
    bool debug            = false;
    size_t controllerIdx    = 0;
    size_t inputIdx         = 0;
    float desktopUpscale   = 20;
    std::string devFile = "";

    int opt;

    while ((opt = getopt(argc, argv, "dc:i:f:")) != -1) {
        switch (opt) {
        case 'd':
            debug = true;
            break;

        case 'c':

            if (strcmp(optarg, "desktop") == 0) {
                controllerIdx = 0;
            } else if (strcmp(optarg, "matrix") == 0) {
                controllerIdx = 1;
            } else {
                std::cerr << "Unknown argument for display option: " << optarg <<
                    std::endl;
                help(argv[0]);
            }
            break;

        case 'i':

            if (strcmp(optarg, "keyboard") == 0) {
                inputIdx = 0;
            }

            else if (strcmp(optarg, "i2c") == 0) {
            #ifdef LED_HAVE_I2C_H_
                inputIdx = 1;
            #elif
                std::cerr << "I2C controller option selected, but framework is not compiled with I2C support! " <<
                    std::endl;
                help(argv[0]);
            #endif // ifdef LED_HAVE_I2C_H_
            }
            else {
                std::cerr << "Unknown argument for controller option: " << optarg <<
                    std::endl;
                help(argv[0]);
            }
            break;

        case 'f':
            devFile = optarg;
            break;

        default:
            help(argv[0]);
        }
    }

    // Controllers/Displays
    std::vector<std::shared_ptr<led::BaseController> > controllers;
    controllers.push_back(std::make_shared<DesktopController>(desktopUpscale));
    controllers.push_back(std::make_shared<MatrixController>());

    // Inputs
    std::vector<std::shared_ptr<led::BaseInput> > inputs;
    inputs.push_back(std::make_shared<KeyboardInput>(devFile));

#ifdef LED_HAVE_I2C_H_
    inputs.push_back(std::make_shared<GameControllerInput>(devFile));
#endif // ifdef LED_HAVE_I2C_H_

    auto c = controllers[controllerIdx];

    std::cout << "Initialize controller and display..." << std::endl;
    if (!c->initialize(PIXEL_MATRIX_WIDTH, PIXEL_MATRIX_HEIGHT, inputs[inputIdx], debug)) {
        std::cout << "Init of controller failed! Leaving." << std::endl;
        help(argv[0]);
    }
    std::cout << "Init of controller successful!" << std::endl;


    std::cout << "Building menu hirarchy... " << std::endl;
    // Games
    auto games = std::make_shared<TextMenu>("Games");
    games->setMenuItems({
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<Snake>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<Pong>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<GameOfLife>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<FlappyBird>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<Tetris>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<MineSweeper>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<ConnectFour>()))
    });

    // Effects
    auto effects = std::make_shared<TextMenu>("Effects");
    effects->setMenuItems({
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<ImageSlideShowApp>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<FractalApp>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<BoardGameApp>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<AnimationApp>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<ClockApp>()))
    });

    // Settings
    auto settings        = std::make_shared<TextMenu>("Settings");
    auto settingsHandler = std::make_shared<SettingsMenuHandler>(c);

    std::stringstream stream;
    stream << "Brightness: ";
    stream << std::fixed << std::setprecision(1) << 100*c->getBrightness() << "%";

    settings->setMenuItems({
        MenuEntry(stream.str(), settingsHandler),
        MenuEntry(std::string("Players: ")+std::to_string(c->getPlayerCount()), settingsHandler),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<UpdateApp>())),
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<ShutdownApp>()))
    });

    // Main Menu
    auto a = std::make_shared<TextMenu>("Main Menu", false);
    a->setMenuItems({
        MenuEntry(std::make_shared<AppLauncher>(c, std::make_shared<TestPatternApp>())),
        MenuEntry(std::make_shared<AppLauncher>(c, effects)),
        MenuEntry(std::make_shared<AppLauncher>(c, games)),
        MenuEntry(std::make_shared<AppLauncher>(c, settings))
    });

    c->addApplication(a);
    std::cout << "Starting main loop..." << std::endl;
    c->run(UPDATE_RATE);
    return 0;
}


SettingsMenuHandler::SettingsMenuHandler(std::shared_ptr<led::BaseController>ctrl) {
    m_ctrl = ctrl;
}
bool SettingsMenuHandler::onSelectMenuItem(TextMenu            *menu,
                      MenuEntry&           menuEntry,
                      size_t idx) {
    switch (idx) {
    // Brightness
    case 0: {
        float brightness = m_ctrl->getBrightness();
        brightness += 0.2;

        if (brightness > 1) {
            brightness = 0.2;
        }
        m_ctrl->setBrightness(brightness);
        std::stringstream stream;
        stream << "Brightness: ";
        stream << std::fixed << std::setprecision(1) << brightness*100 << "%";
        menuEntry.setName(stream.str());
        break;
    }

    // Player Count
    case 1: {
        size_t p = m_ctrl->getPlayerCount();
        p = (p + 1) % (m_ctrl->getMaxPlayerCount()+1);
        if(p == 0) p++;
        m_ctrl->setPlayerCount(p);
        std::stringstream stream;
        stream << "Players: ";
        stream << p;
        menuEntry.setName(stream.str());
        break;
    }

    default:
        return false;

        break;
    }
    menu->updateTextData();
    return false;
}
