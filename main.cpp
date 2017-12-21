#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip> // setprecision

#include "controllers/desktopController.hpp"
#include "controllers/matrixController.hpp"

#include "inputHandlers/keyboardInput.hpp"

#include "applications/update.hpp"
#include "applications/testPatterns.hpp"
#include "applications/textMenu.hpp"
#include "applications/snake.hpp"
#include "applications/imageSlideShow.hpp"
#include "applications/pong.hpp"
#include "applications/fractal.hpp"
#include "applications/shaders.hpp"
#include "applications/gameOfLife.hpp"
#include "applications/flappyBird.hpp"
#include "applications/tetris.hpp"

extern char *optarg;
extern int optind, opterr, optopt;


class SettingsMenuHandler: public MenuEntryHandler{
private:
  std::shared_ptr<BaseController> m_ctrl;
public:
  SettingsMenuHandler(std::shared_ptr<BaseController> ctrl){
    m_ctrl = ctrl;
  }
  bool onSelectMenuItem(TextMenu* menu, TextMenu::MenuEntry& menuEntry, size_t idx){
    switch (idx) {
      case 0:{
        float f = m_ctrl->getBrightness();
        f += 0.2;
        if(f > 1){
          f = 0.2;
        }
        m_ctrl->setBrightness(f);
        std::stringstream stream;
        stream << "Brightness: ";
        stream << std::fixed << std::setprecision(1) << f;
        menuEntry.name = stream.str();
      }
      break;
      default:
      return false;
      break;
    }
    menu->updateTextData();
    return false;
  }
};

int main (int argc, char **argv)
{
  bool debug = false;
  size_t controllerIdx = 0;
  size_t inputIdx = 0;
  std::string keyboardDev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

  int opt;
  while ((opt = getopt(argc, argv ,"dc:i:k:")) != -1) {
    switch (opt) {
      case 'd':
        debug = true;
        break;
      case 'c':
        if (strcmp(optarg,"desktop") == 0) {
          controllerIdx = 0;
        } else if (strcmp(optarg,"matrix") == 0) {
          controllerIdx = 1;
        }else{
          std::cerr << "Unknwon argument for controller option: " << optarg << std::endl;
          exit(1);
        }
      break;
      case 'i':
        if (strcmp(optarg,"keyboard") == 0) {
          inputIdx = 0;
        }else{
          std::cerr << "Unknwon argument for controller option: " << optarg << std::endl;
          exit(1);
        }
      break;
      case 'k':
        keyboardDev = optarg;
      break;

      default:
        std::cerr << "Unknwon commandline option: " << (char)opt << std::endl;
        exit(1);
    }
  }

  // Controllers/Displays
  std::vector<std::shared_ptr<BaseController> > controllers;
  controllers.push_back(std::make_shared<DesktopController>());
  controllers.push_back(std::make_shared<MatrixController>());

  // Inputs
  std::vector<std::shared_ptr<BaseInput> > inputs;
  inputs.push_back(std::make_shared<KeyboardInput>(keyboardDev));

  auto c = controllers[controllerIdx];
  if(!c->initialize(20, 15, inputs[inputIdx], debug)){
    std::cout << "Init of controller failed" << std::endl;
    exit(1);
  }

  // Games
  auto games = std::make_shared<TextMenu>();
  games->setMenuItems({
    TextMenu::MenuEntry("Snake",
                    std::make_shared<AppLauncher>(c,std::make_shared<Snake>())),
    TextMenu::MenuEntry("Pong",
                    std::make_shared<AppLauncher>(c,std::make_shared<Pong>())),
    TextMenu::MenuEntry("Game of Life",
                    std::make_shared<AppLauncher>(c,std::make_shared<GameOfLife>())),
    TextMenu::MenuEntry("Flappy Bird",
                    std::make_shared<AppLauncher>(c,std::make_shared<FlappyBird>())),
    TextMenu::MenuEntry("Tetris",
                    std::make_shared<AppLauncher>(c,std::make_shared<Tetris>()))
  });

  // Effects
  auto effects = std::make_shared<TextMenu>();
  effects->setMenuItems({
    TextMenu::MenuEntry("Slide Show",
      std::make_shared<AppLauncher>(c,std::make_shared<ImageSlideShowApp>())),
    TextMenu::MenuEntry("Fractals",
      std::make_shared<AppLauncher>(c,std::make_shared<FractalApp>())),
    TextMenu::MenuEntry("Shaders",
      std::make_shared<AppLauncher>(c,std::make_shared<ShaderApp>()))
  });

  // Settings
  auto settings = std::make_shared<TextMenu>();
  auto settingsHandler = std::make_shared<SettingsMenuHandler>(c);
  settings->setMenuItems({
    TextMenu::MenuEntry("Brightness: 1",settingsHandler),
    TextMenu::MenuEntry("Update Firmware",
       std::make_shared<AppLauncher>(c,std::make_shared<UpdateApp>())),
  });

  // Main Menu
  auto a = std::make_shared<TextMenu>(false);
  a->setMenuItems({
    TextMenu::MenuEntry("Test Pattern",
                    std::make_shared<AppLauncher>(c,std::make_shared<TestPatternApp>())),
    TextMenu::MenuEntry("Effects",
                    std::make_shared<AppLauncher>(c,effects)),
    TextMenu::MenuEntry("Games",
                    std::make_shared<AppLauncher>(c,games)),
    TextMenu::MenuEntry("Settings",
                    std::make_shared<AppLauncher>(c,settings))
  });

  c->addApplication(a);
  c->run(25);
  return 0;
}
