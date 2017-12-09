#ifndef _H_TESTPATTERN_APP_
#define _H_TESTPATTERN_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../engine/baseApplication.hpp"
#include "../engine/font.hpp"

class TestPatternApp: public BaseApplication {
protected:
  enum PatternType{SINGLE_PIXEL, RANDOM, COLORFADE, COLORFADE_BW, FONT_TEST, END};

  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_colDist;
  std::uniform_int_distribution<int> m_posDist;

  PatternType m_patternType;
  float m_interpolate;
  bmfont::Font m_font;
  TimeUnit m_lastStepUpdate;
  size_t m_pos;

public:
  TestPatternApp();
  virtual ~TestPatternApp ();

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void continueApp();
};


#endif
