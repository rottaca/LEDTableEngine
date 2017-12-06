#include "imageSlideShow.hpp"
#include "../LEDTableEngine/baseController.hpp"

ImageSlideShowApp::ImageSlideShowApp(){
}
ImageSlideShowApp::~ImageSlideShowApp (){

}

void ImageSlideShowApp::setImages(std::vector<std::string> imgFiles){
  m_imageFilePaths = imgFiles;
}

void ImageSlideShowApp::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_bufferColorMode = BufferColorMode::RGB;
  m_blendTime = 1500;
  m_changeTime = 5000;

  m_interpolate = 1;
  m_currImageIdx = 0;
  m_lastImageChange = m_ctrl->getTimeMs();
  for (std::string f: m_imageFilePaths) {
    auto img = loadImage("res/ImageSlideShowApp/" + f);
    if(img)
      m_images.push_back(img);
  }
}

void ImageSlideShowApp::continueApp(){

}
void ImageSlideShowApp::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::ENTER)){

    }
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::EXIT)){
      m_hasFinished = true;
      return;
    }
    if(m_interpolate < 1){
      m_interpolate += (float)deltaTime/m_blendTime;
      if(m_interpolate > 1)
        m_interpolate = 1;
    }

    if(m_ctrl->getTimeMs() - m_lastImageChange > m_changeTime){
      m_lastImageChange = m_ctrl->getTimeMs();
      m_interpolate = 0;
      m_currImageIdx = (m_currImageIdx + 1) % m_images.size();
    }

}
void ImageSlideShowApp::draw(Image &frame){
  size_t lastIdx;
  if(m_currImageIdx == 0)
    lastIdx = m_images.size()-1;
  else if(m_currImageIdx >= 1)
    lastIdx = m_currImageIdx-1;

  auto& imgCurr = m_images[m_currImageIdx];
  auto& imgLast = m_images[lastIdx];

  assert(imgCurr->size == frame.size);
  for (size_t i = 0; i < frame.size; i++) {
    frame.data[i] = imgCurr->data[i]*m_interpolate +
                    imgLast->data[i]*(1.0-m_interpolate);
  }
}

std::shared_ptr<Image> ImageSlideShowApp::loadImage(std::string path)
{
  SDL_Surface* surf = SDL_LoadBMP(path.c_str());
  if(!surf)
  {
    std::cout << "ERROR: "
      << "Couldn't load image file \"" << path << "\"" << std::endl;
    return nullptr;
  }
  std::shared_ptr<Image> img = std::make_shared<Image>(surf->w, surf->h, surf->pitch/surf->w);
  assert(img->channels == 3);
  // Swap from BGR to RGB
  for (size_t i = 0; i < img->size; i+=3) {
    img->data[i+2] = ((uint8_t*)surf->pixels)[i];
    img->data[i+1] = ((uint8_t*)surf->pixels)[i+1];
    img->data[i] = ((uint8_t*)surf->pixels)[i+2];
  }
//memcpy(img->data, surf->pixels,img->size);
  SDL_FreeSurface(surf);
  return img;
}
