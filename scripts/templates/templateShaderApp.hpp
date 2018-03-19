#ifndef _H_TEMPLATE_APP_INC_GUARD_NAME_
#define _H_TEMPLATE_APP_INC_GUARD_NAME_

#include <LEDTableEngine/core/shaderApplication.hpp>
#include <LEDTableEngine/core/types.hpp>

/**
 * This implements a ShaderApplications.
 * The convinience class already handels the user input in a simple way
 * and provides an interface to switch between different animations
 * by pressing ENTER. For each pixel in the image, the renderPixel(...)
 * function is called.
 */
class TEMPLATE_APP_NAME : public led::ShaderApplication {
public:

    TEMPLATE_APP_NAME();
    virtual ~TEMPLATE_APP_NAME();

    void initialize(led::BaseController *ctrl);

protected:

/**
 * Implement pure virtual function
 * from ShaderApplication. This function is called
 * for each pixel in the image.
 * Return the color for that pixel by setting
 * the r,g,b values to something between 0 and 255
 * @param tm       Current time in milliseconds
 * @param x        X coordinate
 * @param y        Y coordinate
 * @param r        Reference (output) to red component
 * @param g        Reference (output) to green component
 * @param b        Reference (output) to blue component
 * @param shaderId Id of the animation, that is selected.
 *                 Can be changed by the user by pressing ENTER.
 */
    void renderPixel(led::TimeUnit tm,
                     size_t x,
                     size_t y,
                     uint8_t     & r,
                     uint8_t     & g,
                     uint8_t     & b,
                     size_t shaderId);

private:

    void effect1(led::TimeUnit tm,
                 size_t x,
                 size_t y,
                 uint8_t     & r,
                 uint8_t     & g,
                 uint8_t     & b);
};


#endif // ifndef _H_TEMPLATE_APP_INC_GUARD_NAME_
