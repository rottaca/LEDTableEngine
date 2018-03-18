#include <LEDTableEngine/core/baseController.hpp>
#include <LEDTableEngine/core/baseInput.hpp>
#include <LEDTableEngine/configuration.hpp>

#include <cstring>
#include <assert.h>

namespace led {
#define FPS_INTERPOLATE (0.01)

BaseController::BaseController() {
  m_width        = 0;
  m_height       = 0;
  m_size         = 0;
  m_debug        = false;
  m_inputHandler = nullptr;
}

BaseController::~BaseController() {}

bool BaseController::initialize(size_t width, size_t height,
                                std::shared_ptr<BaseInput>input, bool debug) {
  std::cout << "Initialize Controller" << std::endl;
  m_width        = width;
  m_height       = height;
  m_size         = width * height;
  m_debug        = debug;
  m_inputHandler = input;
  m_inputHandler->setController(this);
  m_isRunning  = true;
  m_isStandby  = false;
  m_bufferMode = BufferColorMode::PALETTE;
  m_brightness = 1;
  m_playerCnt  = 1;
  auto now = std::chrono::high_resolution_clock::now();
  m_refTimeStartUs =
    std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

  while (!m_applicationStack.empty()) m_applicationStack.pop();

  m_queuedApplications.clear();
  createFrame();
  updateBufferColorMode();

  m_lastKeyPress = getTimeMs();

  m_font = std::make_shared<bmfont::Font>();
  m_font->loadFromFile("res/font/myfont.fnt");

  if (!m_inputHandler->initialize()) return false;

  if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
  {
    std::cerr << "SDL_INIT_AUDIO Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }
  initAudio();
  return true;
}

const Palette& BaseController::getCurrentPalette() {
  assert(m_applicationStack.size() > 0);
  return m_applicationStack.top()->getPalette();
}

void BaseController::addApplication(std::shared_ptr<BaseApplication>app, bool queuedInsert)
{
  if (!queuedInsert) {
    addApplicationDirect(app);
  } else {
    m_queuedApplications.push_back(app);
  }
}

void BaseController::addApplicationDirect(std::shared_ptr<BaseApplication>app) {
  if (m_applicationStack.size() > 0) {
    m_applicationStack.top()->pauseApp();
  }
  m_applicationStack.push(app);
  m_applicationStack.top()->initialize(this);
}

void BaseController::run(size_t fps) {
  if (!m_inputHandler) {
    std::cerr << "Input handler not set" << std::endl;
    return;
  }
  std::cout << "Starting Controller" << std::endl;

  TimeUnit fpsMs                = 1000.0 / fps;
  TimeUnit lastTime             = getTimeMs();
  TimeUnit newTime              = lastTime;
  TimeUnit lastDisplayUpdate    = newTime;
  TimeUnit lastDebugStatsUpdate = newTime;
  TimeUnit frameTime            = fpsMs;
  TimeUnit deltaTime;
  float    avgDispUpdateRate = 0;
  float    avgDispUpdateTime = 0;
  float    avgLoopRate       = 0;
  float    avgFrameTime      = 0;
  float    avgInputProcTime  = 0;

  updateBufferColorMode();

  // //////////////////////
  // The actual main loop
  // //////////////////////
  while (m_isRunning && m_applicationStack.size() > 0) {
    // First of all, compute a few times and time differences
    deltaTime   = newTime - lastTime;
    lastTime    = newTime;
    newTime     = getTimeMs();
    avgLoopRate = (1 - FPS_INTERPOLATE) * avgLoopRate + FPS_INTERPOLATE * deltaTime;

    // Gather new input events
    BaseInput::InputEvents events = m_inputHandler->getInputEvents();

    // Debounce input events, independent from its source
    // Convert continous key presses to a key hold event
    m_kdb.processInput(events);

    // Extract debounced events
    BaseInput::InputEvents eventsDebounced = m_kdb.getDebouncedEvents();

    if (m_debug && (eventsDebounced.size() > 0)) {
      std::cout << "Debounced Events: ";
      for (const BaseInput::InputEvent& e : eventsDebounced) {
        std::cout << BaseInput::inputEventName2Str[e.name] <<
        "(" << BaseInput::inputEventState2Str[e.state] << "), ";
      }
      std::cout << std::endl;
    }

    // Don't do anything if standby is enabeled
    if (!m_isStandby) {
      // Send input events to currently active application
      // and measure the processing time
      TimeUnit t1 = getTimeMs();
      m_applicationStack.top()->processInput(eventsDebounced, deltaTime);
      TimeUnit t2 = getTimeMs();

      // Compute the lowpass filtered time, that was necessary to process the
      // input events
      avgInputProcTime = 0.9 * avgInputProcTime + 0.1 * (t2 - t1);

      // Redraw the frame, if the applications requires a redraw
      if (m_applicationStack.top()->requiresRedraw()) {
        // Draw application to frame
        m_applicationStack.top()->draw(m_frameBuffer);

        // Render the frame to the render target
        showFrame(m_frameBuffer);

        // And compute a few update rates
        avgDispUpdateTime =
          (1 - FPS_INTERPOLATE) * avgDispUpdateTime + FPS_INTERPOLATE * (getTimeMs() - t2);
        avgDispUpdateRate =
          (1 -
           FPS_INTERPOLATE) * avgDispUpdateRate + FPS_INTERPOLATE * (newTime - lastDisplayUpdate);
        lastDisplayUpdate = newTime;
      }

      // Are there queued applications available ?
      if (m_queuedApplications.size() > 0) {
        // If yes, reset the audio controller
        endAudio();
        initAudio();

        // Add the applications to the stack
        for (auto app : m_queuedApplications) {
          addApplicationDirect(app);
          if(m_debug) std::cout << "Added application to stack" << std::endl;
        }
        m_queuedApplications.clear();

        // Update the color buffer mode and continue the application
        updateBufferColorMode();
        if(m_debug) std::cout << "New color mode: " << ((m_bufferMode == PALETTE)?"PALETTE":"RGB") << std::endl;
        m_applicationStack.top()->continueApp();
      }

      // If all remaining applications have to be deintialized,
      // pause them, disable the audio, deinit dem and remove them.
      while (m_applicationStack.size() > 0 &&
             m_applicationStack.top()->hasFinished()) {
        m_applicationStack.top()->pauseApp();
        endAudio();
        m_applicationStack.top()->deinitialize();
        m_applicationStack.pop();

        if (m_applicationStack.size() > 0) {
          initAudio();
          updateBufferColorMode();
          m_applicationStack.top()->continueApp();
        }
      }

      // Compute a few more timings
      frameTime    = getTimeMs() - newTime;
      avgFrameTime = (1 - FPS_INTERPOLATE) * avgFrameTime + FPS_INTERPOLATE * frameTime;
    }

    // Is there something pressed?
    if (eventsDebounced.size() > 0) {
      m_isStandby    = false;
      m_lastKeyPress = getTimeMs();
    }

    // Otherwise check, if we have to switch to the standby mode
    else if ((getTimeMs() - m_lastKeyPress > MINIMUM_AUTO_STANDBY_TIME_MS)
             && m_applicationStack.top()->allowsStandby()) {
      if (!m_isStandby) {
        m_applicationStack.top()->pauseApp();
        // Clear screen before standby
        if (m_frameBuffer.channels == 3) {
          clearFrame({ 0, 0, 0 });
        } else {
          clearFrame(0);
        }
        // Render the frame to the render target
        showFrame(m_frameBuffer);
      }
      m_isStandby = true;
    }

    TimeUnit deltaTime = fpsMs;

    // Throttle the processing rate if we are in standby
    if (m_isStandby) {
      deltaTime = STANDBY_CHECK_TIME_MS;
    }

    // And check if we need to sleep for a bit,
    // to keep our processing rate
    int sleepTime = 0;

    if (frameTime < deltaTime) {
      sleepTime = (int)(deltaTime - frameTime);
      std::chrono::milliseconds sleepDur = std::chrono::milliseconds(sleepTime);
      std::this_thread::sleep_for(sleepDur);
    }

    if (m_debug && (newTime - lastDebugStatsUpdate  > 1000)) {
      lastDebugStatsUpdate = newTime;
      std::cout << "Debug Statistics" << std::endl;
      std::cout << "------------------------------------------------------" << std::endl;
      std::cout << "Avg. Display Rate:        " << 1000.0 / avgDispUpdateRate << " FPS" <<
      std::endl;
      std::cout << "Avg. Loop Rate:           " << 1000.0 / avgLoopRate << " FPS" << std::endl;
      std::cout << "Avg. Display Time:        " << avgDispUpdateTime << " ms" << std::endl;
      std::cout << "Avg. Proc Time:           " << avgInputProcTime << " ms" << std::endl;
      std::cout << "Avg. Proc + Display Time: " << avgFrameTime << " ms" << std::endl;
      std::cout << "Last Sleep Time:          " << sleepTime << " ms" << std::endl;
      std::cout << "Apps on Stack:            " << m_applicationStack.size() << std::endl;
      std::cout << "------------------------------------------------------"  << std::endl;
    }
  }

  // Clear screen before exit
  if (m_frameBuffer.channels == 3) {
    clearFrame({ 0, 0, 0 });
  } else {
    clearFrame(0);
  }
  showFrame(m_frameBuffer);
  shutdown();
}

void BaseController::createFrame() {
  switch (m_bufferMode) {
  case RGB:
    m_frameBuffer.resize(m_height, m_width, 3);
    break;

  case PALETTE:
    m_frameBuffer.resize(m_height, m_width, 1);
    break;
  }
}

void BaseController::clearFrame(ColorRGB color) {
  assert(m_frameBuffer.channels == 3);

  for (size_t i = 0; i < m_frameBuffer.width * m_frameBuffer.height; i++) {
    m_frameBuffer.data[3 * i + 0] = color[0];
    m_frameBuffer.data[3 * i + 1] = color[1];
    m_frameBuffer.data[3 * i + 2] = color[2];
  }
}

void BaseController::clearFrame(uint8_t paletteIdx)
{
  assert(m_frameBuffer.channels == 1);
  memset(m_frameBuffer.data, paletteIdx, m_frameBuffer.width * m_frameBuffer.height);
}

void BaseController::updateBufferColorMode() {
  if (m_applicationStack.size() == 0) return;

  BufferColorMode mode = m_applicationStack.top()->getBufferColorMode();

  if (m_bufferMode != mode) {
    m_bufferMode = mode;
    createFrame();
  } else {
    m_bufferMode = mode;
  }
}

void BaseController::shutdown() {
  m_inputHandler->deinitialize();
}

TimeUnit BaseController::getTimeMs() {
  auto now    = std::chrono::high_resolution_clock::now();
  auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

  return (micros - m_refTimeStartUs) / 1000.0;
}
}
