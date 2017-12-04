#include "baseController.hpp"
#include "baseInput.hpp"

#define FPS_INTERPOLATE (0.01)

BaseController::BaseController(){
  m_width = 0;
  m_height = 0;
  m_size = 0;
  m_debug = false;
  m_inputHandler = nullptr;
}
BaseController::~BaseController(){

}
bool BaseController::initialize(size_t width, size_t height,
                           std::shared_ptr<BaseInput> input, bool debug){
  std::cout << "Initialize Controller" << std::endl;
  m_width = width;
  m_height = height;
  m_size = width*height;
  m_debug = debug;
  m_inputHandler = input;
  m_inputHandler->setController(this);
  m_isRunning = true;
  m_bufferMode = BufferColorMode::PALETTE;
  
  while(!m_applicationStack.empty())
    m_applicationStack.pop();

  m_queuedApplications.clear();
  createFrame();
  updateBufferColorMode();

  return true;
}
const BaseApplication::Palette& BaseController::getCurrentPalette(){
  assert(m_applicationStack.size() > 0);
  return m_applicationStack.top()->getPalette();
}

void BaseController::addApplication(std::shared_ptr<BaseApplication> app, bool queuedInsert)
{
  if (!queuedInsert) {
    addApplicationDirect(app);
  }else{
    m_queuedApplications.push_back(app);
  }
}
void BaseController::addApplicationDirect(std::shared_ptr<BaseApplication> app){
  if (m_applicationStack.size() > 0) {
    m_applicationStack.top()->pauseApp();
  }
  m_applicationStack.push(app);
  m_applicationStack.top()->initialize(this);
}


void BaseController::run(size_t fps){
  if(!m_inputHandler){
    std::cerr << "Input handler not set" << std::endl;
    return;
  }
  std::cout << "Starting Controller" << std::endl;

  TimeUnit fpsMs = 1000.0/fps;
  TimeUnit lastTime = getTimeMs();
  TimeUnit newTime = lastTime;
  TimeUnit lastDisplayUpdate = newTime;
  TimeUnit lastDebugStatsUpdate = newTime;
  TimeUnit frameTime = fpsMs;
  TimeUnit deltaTime;
  float avgDispUpdateRate = 0;
  float avgDispUpdateTime = 0;
  float avgLoopRate = 0;
  float avgFrameTime = 0;
  float avgInputProcTime = 0;
  
  updateBufferColorMode();
  
  while (m_isRunning && m_applicationStack.size() > 0) {

    newTime = getTimeMs();
    deltaTime = newTime-lastTime;
    avgLoopRate = (1-FPS_INTERPOLATE)*avgLoopRate + FPS_INTERPOLATE*deltaTime;

    BaseInput::InputEvents events = m_inputHandler->getInputEvents();
    m_kdb.processInput(events);
    BaseInput::InputEvents eventsDebounced = m_kdb.getDebouncedEvents();

    if(m_debug && eventsDebounced.size() > 0){
      std::cout << "Debounced Events:" << std::endl;
      for(const BaseInput::InputEvent& e: eventsDebounced){
        std::cout << BaseInput::inputEventName2Str[e.name] <<
                  "(" << BaseInput::inputEventState2Str[e.state] << "), ";
      }
      std::cout << std::endl;
    }

    TimeUnit t1 = getTimeMs();
    m_applicationStack.top()->processInput(events, eventsDebounced,deltaTime);
    TimeUnit t2 = getTimeMs();

    if(m_applicationStack.top()->requiresRedraw()){
      m_applicationStack.top()->draw(m_frameBuffer);
      showFrame(m_frameBuffer);
      avgDispUpdateTime = (1-FPS_INTERPOLATE)*avgDispUpdateTime + FPS_INTERPOLATE*(getTimeMs() - t2);
      avgDispUpdateRate = (1-FPS_INTERPOLATE)*avgDispUpdateRate + FPS_INTERPOLATE*(newTime - lastDisplayUpdate);
      lastDisplayUpdate = newTime;
    }
	
    avgInputProcTime = 0.9*avgInputProcTime + 0.1*(t2-t1);

    if(m_queuedApplications.size() > 0){
      for(auto app: m_queuedApplications){
        addApplicationDirect(app);
      }
      m_queuedApplications.clear();
	  updateBufferColorMode();
    }
    if(m_applicationStack.top()->hasFinished()){
      m_applicationStack.top()->pauseApp();
      m_applicationStack.pop();
      if(m_applicationStack.size() > 0){
		  updateBufferColorMode();
          m_applicationStack.top()->continueApp();
	  }
    }

    frameTime = getTimeMs() - newTime;
    avgFrameTime = (1-FPS_INTERPOLATE)*avgFrameTime + FPS_INTERPOLATE*frameTime;
    lastTime = newTime;

    int sleepTime = 0;
    if(frameTime < fpsMs){
      sleepTime = (int)(fpsMs - frameTime);
      std::chrono::milliseconds sleepDur = std::chrono::milliseconds(sleepTime);
      std::this_thread::sleep_for(sleepDur);
    }

    if (m_debug && newTime - lastDebugStatsUpdate  > 1000){
      lastDebugStatsUpdate = newTime;
      std::cout << "Debug Statistics" << std::endl;
      std::cout << "------------------------------------------------------" << std::endl;
      std::cout << "Avg. Display Rate:        " << 1000.0/avgDispUpdateRate << " FPS" << std::endl;
      std::cout << "Avg. Loop Rate:           " << 1000.0/avgLoopRate << " FPS" << std::endl;
      std::cout << "Avg. Display Time:        " << avgDispUpdateTime << " ms" << std::endl;
      std::cout << "Avg. Proc Time:           " << avgInputProcTime << " ms" << std::endl;
      std::cout << "Avg. Proc + Display Time: " << avgFrameTime << " ms" << std::endl;
      std::cout << "Last Sleep Time:          " << sleepTime << " ms" << std::endl;
      std::cout << "Apps on Stack:            " << m_applicationStack.size() << std::endl;
      std::cout << "------------------------------------------------------"  << std::endl;
    }
  }
  shutdown();
}

void BaseController::createFrame(){
  switch(m_bufferMode){
	  case RGB:
		m_frameBuffer.resize(m_size*3);
	  break;
	  case PALETTE:
		m_frameBuffer.resize(m_size);
	  break;
  }
}
void BaseController::clearFrame(uint8_t val){
  m_frameBuffer.assign(m_frameBuffer.size(),val);
}
void BaseController::updateBufferColorMode(){
    assert(m_applicationStack.size() > 0);
	BufferColorMode mode = m_applicationStack.top()->getBufferColorMode();
	if(m_bufferMode != mode){
		m_bufferMode = mode;
		createFrame();
	}else{
		m_bufferMode = mode;
	}
}
void BaseController::shutdown(){

}
TimeUnit BaseController::getTimeMs(){
    auto now = std::chrono::high_resolution_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    return millis/1000.0;
}
