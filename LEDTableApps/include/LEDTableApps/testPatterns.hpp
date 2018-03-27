#ifndef _H_TESTPATTERN_APP_
#define _H_TESTPATTERN_APP_

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/font.hpp>

namespace ledApps{
class TestPatternApp : public led::BaseApplication {
protected:

    enum PatternType { SINGLE_PIXEL, RANDOM, COLORFADE, COLORFADE_BW, FONT_TEST,
                       END };

    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_colDist;
    std::uniform_int_distribution<int> m_posDist;

    PatternType m_patternType;
    float m_interpolate;
    led::bmfont::Font m_font;
    led::TimeUnit m_lastStepUpdate;
    size_t m_pos;

public:

    TestPatternApp();
    virtual ~TestPatternApp();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
    void continueApp();


    const std::string getName() const{
      return "Test Patterns";
    }
};
}

#endif // ifndef _H_TESTPATTERN_APP_
