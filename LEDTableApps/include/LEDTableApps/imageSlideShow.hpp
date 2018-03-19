#ifndef _H_IMAGE_SLIDE_SHOW_APP_
#define _H_IMAGE_SLIDE_SHOW_APP_

#include <LEDTableEngine/core/baseApplication.hpp>
#include <LEDTableEngine/core/font.hpp>

class ImageSlideShowApp : public led::BaseApplication {
protected:

    led::TimeUnit m_lastImageChange;
    float m_interpolate;
    size_t m_currImageIdx;
    std::vector<std::shared_ptr<led::Image> > m_images;
    std::vector<std::string> m_imageFilePaths;
    led::TimeUnit m_blendTime, m_changeTime;

public:

    ImageSlideShowApp();
    virtual ~ImageSlideShowApp();

    void initialize(led::BaseController *ctrl);

    void processInput(const led::BaseInput::InputEvents& events,
                      led::TimeUnit deltaTime);
    void draw(led::Image& frame);
    void continueApp();

private:

    std::shared_ptr<led::Image>loadImage(std::string path);
};


#endif // ifndef _H_IMAGE_SLIDE_SHOW_APP_
