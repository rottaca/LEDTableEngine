#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <cstring>

#include "controllers/desktopController.hpp"
#include "controllers/matrixController.hpp"

#include "inputHandlers/keyboardInput.hpp"

#include "applications/testPatterns.hpp"
#include "applications/textMenu.hpp"
#include "applications/snake.hpp"
#include "applications/imageSlideShow.hpp"
#include "applications/pong.hpp"
#include "applications/fractal.hpp"

extern char *optarg;
extern int optind, opterr, optopt;

int main (int argc, char **argv)
{
  bool debug = false;
  // Controllers/Displays
  size_t controllerIdx = 0;
  std::vector<std::shared_ptr<BaseController> > controllers;
  controllers.push_back(std::make_shared<DesktopController>());
  controllers.push_back(std::make_shared<MatrixController>());

  // Inputs
  size_t inputIdx = 0;
  std::vector<std::shared_ptr<BaseInput> > inputs;
  inputs.push_back(std::make_shared<KeyboardInput>());

  int opt;
  while ((opt = getopt(argc, argv ,"dc:i:")) != -1) {
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

      default:
        std::cerr << "Unknwon commandline option: " << (char)opt << std::endl;
        exit(1);
    }
  }
  auto c = controllers[controllerIdx];
  if(!c->initialize(20, 15, inputs[inputIdx], debug)){
    std::cout << "Init of controller failed" << std::endl;
    exit(1);
  }

  // Apps
  auto iss = std::make_shared<ImageSlideShowApp>();
  iss->setImages({
    "strand.bmp",
    "img1.bmp",
    "img2.bmp"
  });

  // Main Menu
  auto a = std::make_shared<TextMenu>();
  a->setMenuItems({
    TextMenu::MenuEntry("Test Pattern",
                    std::make_shared<AppLauncher>(c,std::make_shared<TestPatternApp>())),
    TextMenu::MenuEntry("Slide Show",
                    std::make_shared<AppLauncher>(c,iss)),
    TextMenu::MenuEntry("Snake",
                    std::make_shared<AppLauncher>(c,std::make_shared<Snake>())),
    TextMenu::MenuEntry("Pong",
                    std::make_shared<AppLauncher>(c,std::make_shared<Pong>())),
    TextMenu::MenuEntry("Fractals",
                    std::make_shared<AppLauncher>(c,std::make_shared<FractalApp>()))
  });

  c->addApplication(a);
  c->run(25);

  return 0;
}
