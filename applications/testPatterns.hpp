#ifndef _H_TESTPATTERN_APP_
#define _H_TESTPATTERN_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>

#include "../LEDTableEngine/baseApplication.hpp"

class TestPatternApp: public BaseApplication {
protected:
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_colDist;
  std::uniform_int_distribution<int> m_posDist;

public:
  TestPatternApp();
  virtual ~TestPatternApp ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced);
  void draw(std::vector<uint8_t> &frame);
};


#endif
