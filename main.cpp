#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "controllers/desktopController.hpp"
#include "inputHandlers/keyboardInput.hpp"
#include "applications/testPatterns.hpp"

int main (int argc, char *argv[])
{
  auto dc = std::make_shared<DesktopController>();
  auto kh = std::make_shared<KeyboardInput>();
  auto testPatterns = std::make_shared<TestPatternApp>();

  dc->initialize(20, 15, kh, true);
  dc->addApplication(testPatterns);
  dc->run(30);
  return 0;
}
