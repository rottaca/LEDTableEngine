#include <LEDTableApps/imageSlideShow.hpp>

#include <dirent.h>

#include <LEDTableEngine/core/baseController.hpp>

using namespace led;
namespace ledApps{
ImageSlideShowApp::ImageSlideShowApp() {

}

ImageSlideShowApp::~ImageSlideShowApp() {
}

void ImageSlideShowApp::initialize(BaseController *ctrl) {
    BaseApplication::initialize(ctrl);
    m_bufferColorMode = BufferColorMode::RGB;
    m_blendTime       = 1500;
    m_changeTime      = 5000;

    m_interpolate     = 1;
    m_currImageIdx    = 0;
    m_lastImageChange = m_ctrl->getTimeMs();

    std::cout << "Searching for slide show images..." << std::endl;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("res/ImageSlideShowApp/")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".bmp") != 0) {
                std::cout << "Found image file for slideshow: " << ent->d_name <<
                    std::endl;
                m_imageFilePaths.push_back(ent->d_name);
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        std::cerr << "Couldn't open directory with slide show images!" << std::endl;
    }

    std::cout << m_imageFilePaths.size() << " image files found!" << std::endl;
    std::cout << "Loading images..." << std::endl;
    for (std::string f : m_imageFilePaths) {
        auto img = loadImage("res/ImageSlideShowApp/" + f);

        if (!img) {
            std::cerr << "Failed to load image " << f << std::endl;
        }

        if ((img->height != ctrl->getHeight()) || (img->width != ctrl->getWidth())) {
            std::cerr << "Image " << f << " could be loaded, but its dimension (" << img->width << "x" <<
                img->height << ") ";
            std::cerr << "does not match the display resolution (" << ctrl->getWidth() << "x" <<
                ctrl->getHeight() << ")" << std::endl;
            std::cerr << "If you changed the display resolution, use the provided " << std::endl;
            std::cerr << "convertImage.sh script in res/ImageSlideShowApp. " << std::endl;
        } else {
            m_images.push_back(img);
        }
    }
    std::cout << m_images.size() << " images loaded!" << std::endl;
}

void ImageSlideShowApp::continueApp() {
}

void ImageSlideShowApp::processInput(const BaseInput::InputEvents& events,
                                     TimeUnit deltaTime) {
    if (BaseInput::isPressed(events, BaseInput::InputEventName::ENTER)) {}

    if (BaseInput::isPressed(events, BaseInput::InputEventName::EXIT)) {
        m_hasFinished = true;
        return;
    }

    if (m_interpolate < 1) {
        m_interpolate += (float)deltaTime / m_blendTime;

        if (m_interpolate > 1) m_interpolate = 1;
    }

    if (m_ctrl->getTimeMs() - m_lastImageChange > m_changeTime) {
        m_lastImageChange = m_ctrl->getTimeMs();
        m_interpolate     = 0;
        m_currImageIdx    = (m_currImageIdx + 1) % m_images.size();
    }
}

void ImageSlideShowApp::draw(Image& frame) {
    size_t lastIdx;

    if (m_currImageIdx == 0) lastIdx = m_images.size() - 1;
    else if (m_currImageIdx >= 1) lastIdx = m_currImageIdx - 1;

    auto& imgCurr = m_images[m_currImageIdx];
    auto& imgLast = m_images[lastIdx];

    for (size_t i = 0; i < frame.size; i++) {
        frame.data[i] = imgCurr->data[i] * m_interpolate +
                        imgLast->data[i] * (1.0 - m_interpolate);
    }
}

std::shared_ptr<Image>ImageSlideShowApp::loadImage(std::string path)
{
    SDL_Surface *surf = SDL_LoadBMP(path.c_str());

    if (!surf)
    {
        std::cout << "ERROR: "
                  << "Couldn't load image file \"" << path << "\"" << std::endl;
        return nullptr;
    }
    std::shared_ptr<Image> img = std::make_shared<Image>(surf->h,
                                                         surf->w,
                                                         surf->pitch / surf->w);
    assert(img->channels == 3);

    // Swap from BGR to RGB
    for (size_t i = 0; i < img->size; i += 3) {
        img->data[i + 2] = ((uint8_t *)surf->pixels)[i];
        img->data[i + 1] = ((uint8_t *)surf->pixels)[i + 1];
        img->data[i]     = ((uint8_t *)surf->pixels)[i + 2];
    }

    // memcpy(img->data, surf->pixels,img->size);
    SDL_FreeSurface(surf);
    return img;
}
}
