#ifndef _H_TESTPATTERN_APP_
#define _H_TESTPATTERN_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../LEDTableEngine/baseApplication.hpp"
#include "../LEDTableEngine/font.hpp"

class TestPatternApp: public BaseApplication {
protected:
  enum PatternType{RANDOM = 0, COLORFADE, COLORFADE_BW, FONT_TEST, END};

  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_colDist;
  std::uniform_int_distribution<int> m_posDist;

  PatternType m_patternType;
  float m_interpolate;

  std::array<Palette,END> m_palettes;
  Palette m_randomPalette;
  Palette m_colorFadebwPalette;
  Palette m_colorFadePalette;
  bmfont::Font m_font;

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
