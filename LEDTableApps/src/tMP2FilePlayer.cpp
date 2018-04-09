#include <LEDTableApps/tMP2FilePlayer.hpp>
#include <LEDTableEngine/core/baseController.hpp>

#include <dirent.h>
using namespace led;

TMP2FilePlayer::TMP2FilePlayer() {
    // Do some initialize that should only happen once!
}

TMP2FilePlayer::~TMP2FilePlayer() {
}

void TMP2FilePlayer::initialize(BaseController *ctrl) {
    // Initialize base class
    BaseApplication::initialize(ctrl);

    // Switch to RGB color mode
    // This will create a new image buffer with RGB values
    // instead of a palette
    m_bufferColorMode = BufferColorMode::RGB;

    // Initialize your member variables here!

    std::cout << "Searching for tmp2 files..." << std::endl;
    DIR *dir;
    struct dirent *ent;
    m_fileNames.clear();
    m_fileIdx = 0;
    if ((dir = opendir("res/TMP2FilePlayer/")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".tmp2") != 0) {
                std::cout << "Found tmp2 file: " << ent->d_name <<
                    std::endl;
                m_fileNames.push_back(std::string("res/TMP2FilePlayer/") + ent->d_name);
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        std::cerr << "Couldn't open directory with tmp2 files!" << std::endl;
    }

    m_tmp2File.loadTMP2File(m_fileNames.at(0),true);
}

void TMP2FilePlayer::deinitialize() {
    // Deinitialize something
}

void TMP2FilePlayer::continueApp() {
    // Continue your app (only necessary if you launched new apps by your own)
}

void TMP2FilePlayer::pauseApp() {
    // Pause your app (only necessary if you launched new apps by your own)
}

void TMP2FilePlayer::processInput(const BaseInput::InputEvents& events,
                                     TimeUnit deltaTime) {
    // Process incoming events
    for (const auto& e : events) {
        // Only consider key pressed and ignore KEY_RELEASED and KEY_HOLD
        if (e.state != BaseInput::InputEventState::KEY_PRESSED) continue;

        // Check which key has been pressed
        // This could happen multiple times in the "events" structure if you
        // have multiple players. Have a look at e.playerId to check that.
        switch (e.name) {
        case BaseInput::InputEventName::UP:
        case BaseInput::InputEventName::LEFT:
        case BaseInput::InputEventName::DOWN:
        case BaseInput::InputEventName::RIGHT:
        case BaseInput::InputEventName::ENTER:
            m_fileIdx = (m_fileIdx + 1) % m_fileNames.size();
            if(!m_tmp2File.loadTMP2File(m_fileNames.at(m_fileIdx),true)){
              std::cerr << "Couldn't open tmp2 file " << m_fileNames.at(m_fileIdx) << std::endl;
            }
            break;
        case BaseInput::InputEventName::EXIT:
            m_hasFinished = true;
            return;
        }
    }
}

void TMP2FilePlayer::draw(Image& frame) {
    m_tmp2File.getNextImage(frame);
}
