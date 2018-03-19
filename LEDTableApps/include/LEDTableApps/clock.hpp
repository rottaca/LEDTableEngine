#ifndef _H_CLOCK_APP
#define _H_CLOCK_APP

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/matrixScrollText.hpp>
#include <LEDTableEngine/core/rasterizer.hpp>

class ClockApp : public led::BaseApplication {
protected:

    led::Rasterizer m_rasterizer;

public:

    ClockApp();
    virtual ~ClockApp();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
};


#endif // ifndef _H_CLOCK_APP
