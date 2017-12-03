#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "controllers/desktopController.hpp"
#include "controllers/matrixController.hpp"
#include "inputHandlers/keyboardInput.hpp"
#include "applications/testPatterns.hpp"

int main (int argc, char *argv[])
{
  auto dc = std::make_shared<DesktopController>();
  //auto dc = std::make_shared<MatrixController>();
  auto kh = std::make_shared<KeyboardInput>();
  auto testPatterns = std::make_shared<TestPatternApp>();

  if(!dc->initialize(20, 15, kh, true)){
    std::cout << "Init of controller failed" << std::endl;
    exit(1);
  }

  dc->addApplication(testPatterns);
  dc->run(30);

  return 0;
}
