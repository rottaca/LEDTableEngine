#ifndef _H_IMAGE_SLIDE_SHOW_APP_
#define _H_IMAGE_SLIDE_SHOW_APP_

#include <cstdlib>
#include <memory>
#include <vector>
#include <random>
#include <array>

#include "../LEDTableEngine/baseApplication.hpp"
#include "../LEDTableEngine/font.hpp"

class ImageSlideShowApp: public BaseApplication {
protected:

  TimeUnit m_lastImageChange;
  float m_interpolate;
  size_t m_currImageIdx;
  std::vector<std::shared_ptr<Image> > m_images;
  std::vector<std::string> m_imageFilePaths;
  TimeUnit m_blendTime, m_changeTime;
public:
  ImageSlideShowApp();
  virtual ~ImageSlideShowApp ();

  void setImages(std::vector<std::string> imgFiles);

  void initialize(BaseController * ctrl);

  void processInput(const BaseInput::InputEvents &events,
                    const BaseInput::InputEvents &eventsDebounced,
                    TimeUnit deltaTime);
  void draw(Image &frame);
  void continueApp();

private:
  std::shared_ptr<Image> loadImage(std::string path);
};


#endif
