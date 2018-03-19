#ifndef _H_FRACTAL_APP_
#define _H_FRACTAL_APP_

#include <complex>

#include <LEDTableEngine/core/baseApplication.hpp>

class FractalApp : public led::BaseApplication {
protected:

    struct FractalDef {
        led::Pointf center;
        led::Pointf dim;
        float zoomMin, zoomMax;
        FractalDef() {
        }

        FractalDef(float cx,
                   float cy,
                   float sx,
                   float sy,
                   float fmin,
                   float fmax) {
            center.x = cx;
            center.y = cy;
            dim.x    = sx;
            dim.y    = sy;
            zoomMin  = fmin;
            zoomMax  = fmax;
        }
    };

    float m_currZoom;
    float m_zoomDir;
    FractalDef m_frac;

public:

    FractalApp();
    virtual ~FractalApp();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
    void continueApp();

private:

    void draw_Mandelbrot(led::Image & frame,     // where to draw the image
                         double cxmin,
                         double cxmax,
                         double cymin,
                         double cymax,     // the rect to draw in the complex
                                           // plane
                         unsigned int max_iterations);

    void percentToRGB(float t,
                      uint8_t& r,
                      uint8_t& g,
                      uint8_t& b);
};


#endif // ifndef _H_FRACTAL_APP_
