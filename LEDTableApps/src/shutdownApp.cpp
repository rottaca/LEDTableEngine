#include <LEDTableApps/shutdownApp.hpp>
#include <LEDTableEngine/core/baseController.hpp>
#include <LEDTableEngine/core/yesNoDisplay.hpp>

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> // library for fcntl function

using namespace led;
namespace ledApps{

ShutdownApp::ShutdownApp() {
    // Do some initialize that should only happen once!
}

ShutdownApp::~ShutdownApp() {
}

void ShutdownApp::initialize(BaseController *ctrl) {
    // Initialize base class
    BaseApplication::initialize(ctrl);

    // Switch to RGB color mode
    // This will create a new image buffer with RGB values
    // instead of a palette
    m_bufferColorMode = BufferColorMode::RGB;

    // Initialize your member variables here!
    m_state = IDLE;
    m_yesNoDisplay = std::make_shared<led::YesNoDisplay>();
    m_yesNoDisplay->setText("Shutdown: Are you sure?");
}

void ShutdownApp::deinitialize() {
    // Deinitialize something
}

void ShutdownApp::continueApp() {
    // Continue your app (only necessary if you launched new apps by your own)
}

void ShutdownApp::pauseApp() {
    // Pause your app (only necessary if you launched new apps by your own)
}

void ShutdownApp::processInput(const BaseInput::InputEvents& events,
                                     TimeUnit deltaTime) {
    switch (m_state) {
      case IDLE:
        m_ctrl->addApplication(m_yesNoDisplay, true);
        m_state = USER_RESPONSE;
      break;
      case USER_RESPONSE:
        if(m_yesNoDisplay->exitedWithYes()){
          if(m_ctrl->isDebug())
            std::cout << "Shutdown started" << std::endl;
          pid_t fk = fork();
          // Child process
          if(!fk){
            // Call shutdown command
            execl("/bin/sh","/bin/sh","-c","sudo shutdown -h now",(char*) NULL);
            exit(0);
          }
          m_state = SHUTDOWN_STARTED;
        }else{
          if(m_ctrl->isDebug())
            std::cout << "Shutdown aborted" << std::endl;
          m_hasFinished = true;
          m_state = SHUTDOWN_ABORTED;
        }
      break;
      case SHUTDOWN_STARTED:
      break;
      case SHUTDOWN_ABORTED:
      break;
    }
}

void ShutdownApp::draw(Image& frame) {
    // Set the frame to (0,0,0)
    switch (m_state) {
      case SHUTDOWN_STARTED:
        m_ctrl->clearFrame({ 0, 255, 0 });
      break;
      default:
        m_ctrl->clearFrame({ 0, 0, 0 });
    }
}
}
