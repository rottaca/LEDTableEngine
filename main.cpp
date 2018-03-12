#include <iomanip>

#include "LEDTableEngine/configuration.hpp"
#include "LEDTableEngine/controllers/desktopController.hpp"
#include "LEDTableEngine/controllers/matrixController.hpp"
#include "LEDTableEngine/core/audio.hpp"

#include "LEDTableEngine/inputHandlers/keyboardInput.hpp"
#ifdef LED_HAVE_I2C_H_
  # include "LEDTableEngine/inputHandlers/gameControllerInput.hpp"
#endif // ifdef LED_HAVE_I2C_H_

#include "LEDTableApps/update.hpp"
#include "LEDTableApps/testPatterns.hpp"
#include "LEDTableApps/textMenu.hpp"
#include "LEDTableApps/snake.hpp"
#include "LEDTableApps/imageSlideShow.hpp"
#include "LEDTableApps/pong.hpp"
#include "LEDTableApps/fractal.hpp"
#include "LEDTableApps/shaders.hpp"
#include "LEDTableApps/gameOfLife.hpp"
#include "LEDTableApps/flappyBird.hpp"
#include "LEDTableApps/tetris.hpp"
#include "LEDTableApps/minesweeper.hpp"
#include "LEDTableApps/clock.hpp"

extern char *optarg;
extern int   optind, opterr, optopt;

using namespace led;

class SettingsMenuHandler : public MenuEntryHandler {
private:

  std::shared_ptr<led::BaseController> m_ctrl;

public:

  SettingsMenuHandler(std::shared_ptr<led::BaseController>ctrl) {
    m_ctrl = ctrl;
  }

  bool onSelectMenuItem(TextMenu            *menu,
                        TextMenu::MenuEntry& menuEntry,
                        size_t               idx) {
    switch (idx) {
    // Brightness
    case 0: {
      float f = m_ctrl->getBrightness();
      f += 0.2;

      if (f > 1) {
        f = 0.2;
      }
      m_ctrl->setBrightness(f);
      std::stringstream stream;
      stream << "Brightness: ";
      stream << std::fixed << std::setprecision(1) << f;
      menuEntry.name = stream.str();
      break;
    }

    // Player Count
    case 1: {
      size_t p = m_ctrl->getPlayerCount();
      p = (p + 1) % (led::kMaxPlayerCount + 1);
      m_ctrl->setPlayerCount(p);
      std::stringstream stream;
      stream << "Players: ";

      if (p == 0) {
        stream << "all";
      } else {
        stream << p;
      }
      menuEntry.name = stream.str();
      break;
    }

    default:
      return false;

      break;
    }
    menu->updateTextData();
    return false;
  }
};

int main(int argc, char **argv)
{
  bool   debug            = false;
  size_t controllerIdx    = 0;
  size_t inputIdx         = 0;
  float  desktopUpscale   = 20;
  std::string keyboardDev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
  std::string i2cDev      = "/dev/i2c-1";

  int opt;

  while ((opt = getopt(argc, argv, "dc:i:k:I:")) != -1) {
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
        std::cerr << "Unknwon argument for controller option: " << optarg <<
        std::endl;
        exit(1);
      }
      break;

    case 'i':

      if (strcmp(optarg, "keyboard") == 0) {
        inputIdx = 0;
      }

#ifdef LED_HAVE_I2C_H_
      else if (strcmp(optarg, "i2c") == 0) {
        inputIdx = 1;
      }
#endif // ifdef LED_HAVE_I2C_H_
      else {
        std::cerr << "Unknwon argument for controller option: " << optarg <<
        std::endl;
        exit(1);
      }
      break;

    case 'k':
      keyboardDev = optarg;
      break;

    case 'I':
      i2cDev = optarg;
      break;

    default:
      std::cerr << "Unknwon commandline option: " << (char)opt << std::endl;
        exit(1);
    }
  }

  // Controllers/Displays
  std::vector<std::shared_ptr<led::BaseController> > controllers;
  controllers.push_back(std::make_shared<DesktopController>(desktopUpscale));
  controllers.push_back(std::make_shared<MatrixController>());

  // Inputs
  std::vector<std::shared_ptr<led::BaseInput> > inputs;
  inputs.push_back(std::make_shared<KeyboardInput>(keyboardDev));

#ifdef LED_HAVE_I2C_H_
  inputs.push_back(std::make_shared<GameControllerInput>(i2cDev));
#endif // ifdef LED_HAVE_I2C_H_

  auto c = controllers[controllerIdx];

  if (!c->initialize(PIXEL_MATRIX_WIDTH, PIXEL_MATRIX_HEIGHT, inputs[inputIdx], debug)) {
    std::cout << "Init of controller failed! Leaving." << std::endl;
        exit(1);
  }

  // Games
  auto games = std::make_shared<TextMenu>();
  games->setMenuItems({
                        TextMenu::MenuEntry("Snake",
                                            std::make_shared<AppLauncher>(c, std::make_shared<Snake>())),
                        TextMenu::MenuEntry("Pong",
                                            std::make_shared<AppLauncher>(c, std::make_shared<Pong>())),
                        TextMenu::MenuEntry("Game of Life",
                                            std::make_shared<AppLauncher>(c,
                                                                          std::make_shared<
                                                                            GameOfLife>())),
                        TextMenu::MenuEntry("Flappy Bird",
                                            std::make_shared<AppLauncher>(c,
                                                                          std::make_shared<
                                                                            FlappyBird>())),
                        TextMenu::MenuEntry("Tetris",
                                            std::make_shared<AppLauncher>(c,
                                                                          std::make_shared<Tetris>())),
                        TextMenu::MenuEntry("Minesweeper",
                                            std::make_shared<AppLauncher>(c,
                                                                          std::make_shared<
                                                                            MineSweeper>()))
                      });

  // Effects
  auto effects = std::make_shared<TextMenu>();
  effects->setMenuItems({
                          TextMenu::MenuEntry("Slide Show",
                                              std::make_shared<AppLauncher>(c,
                                                                            std::make_shared<
                                                                              ImageSlideShowApp>())),
                          TextMenu::MenuEntry("Fractals",
                                              std::make_shared<AppLauncher>(c,
                                                                            std::make_shared<
                                                                              FractalApp>())),
                          TextMenu::MenuEntry("Shaders",
                                              std::make_shared<AppLauncher>(c,
                                                                            std::make_shared<
                                                                              ShaderApp>())),
                          TextMenu::MenuEntry("Clock",
                                              std::make_shared<AppLauncher>(c,
                                                                            std::make_shared<
                                                                              ClockApp>()))
                        });

  // Settings
  auto settings        = std::make_shared<TextMenu>();
  auto settingsHandler = std::make_shared<SettingsMenuHandler>(c);
  settings->setMenuItems({
                           TextMenu::MenuEntry("Brightness: 1", settingsHandler),
                           TextMenu::MenuEntry("Players: all", settingsHandler),
                           TextMenu::MenuEntry("Update Firmware",
                                               std::make_shared<AppLauncher>(c,
                                                                             std::make_shared<
                                                                               UpdateApp>())),
                         });

  // Main Menu
  auto a = std::make_shared<TextMenu>(false);
  a->setMenuItems({
                    TextMenu::MenuEntry("Test Pattern",
                                        std::make_shared<AppLauncher>(c,
                                                                      std::make_shared<
                                                                        TestPatternApp>())),
                    TextMenu::MenuEntry("Effects",
                                        std::make_shared<AppLauncher>(c, effects)),
                    TextMenu::MenuEntry("Games",
                                        std::make_shared<AppLauncher>(c, games)),
                    TextMenu::MenuEntry("Settings",
                                        std::make_shared<AppLauncher>(c, settings))
                  });

  c->addApplication(a);
  c->run(UPDATE_RATE);
  return 0;
}
