#include <LEDTableApps/connectFour.hpp>
#include <LEDTableEngine/core/baseController.hpp>
#include <LEDTableApps/textDisplay.hpp>

using namespace led;

ConnectFour::ConnectFour() {
    // Do some initialize that should only happen once!
}

ConnectFour::~ConnectFour() {
}

void ConnectFour::initialize(BaseController *ctrl) {
    // Initialize base class
    BaseApplication::initialize(ctrl);
    // Specify a RGB color palette
    // Only use the indices into that table
    // if you set color values in the image
    m_colorPalette = {
        {   255,   0,   0 },        // Player 1
        {   255, 255,   0 },        // Player 2
        {   0,     0,   0 },        // Background
        {   255, 255,   255 },        // Color 2
        {   0,   255,   0 }        // Color 2
    };
    // Fill all remaining indices with a default color
    fillPalette(m_colorPalette);

    // Initialize your member variables here!
    m_gameField.resize(m_ctrl->getHeight(), m_ctrl->getWidth(), 1);
    memset(m_gameField.data, 2, m_gameField.size);
    m_chipIsFalling = false;
    m_playerWon = false;
    m_currPlayerId = 0;
    m_currFallingChipPos.x = 0;
    m_currFallingChipPos.y = 0;
    m_lastFallTime = 0;
}

void ConnectFour::deinitialize() {
    // Deinitialize something
}
void ConnectFour::continueApp() {
    // Continue your app (only necessary if you launched new apps by your own)
}
void ConnectFour::pauseApp() {
    // Pause your app (only necessary if you launched new apps by your own)
}

void ConnectFour::processInput(const BaseInput::InputEvents& events,
                               TimeUnit deltaTime) {
    if(m_ctrl->getPlayerCount() != 2) {
        // Display final message
        auto a = std::make_shared<TextDisplay>();
        a->setText("This game requires 2 players.");
        m_ctrl->addApplication(a, true);
        m_hasFinished = true;
        return;
    }
    // Process incoming events
    for (const auto& e : events) {
        // Only consider key pressed and ignore KEY_RELEASED and KEY_HOLD
        if(e.state != BaseInput::InputEventState::KEY_PRESSED || e.playerId != m_currPlayerId)
            continue;
        // Check which key has been pressed
        // This could happen multiple times in the "events" structure if you
        // have multiple players. Have a look at e.playerId to check that.
        switch (e.name) {
        case BaseInput::InputEventName::LEFT:
            if(!m_chipIsFalling && !m_playerWon) m_currFallingChipPos.x = std::max(0,m_currFallingChipPos.x-1);
            break;

        case BaseInput::InputEventName::RIGHT:
            if(!m_chipIsFalling && !m_playerWon) m_currFallingChipPos.x = std::min((int)m_ctrl->getWidth()-1,m_currFallingChipPos.x+1);
            break;

        case BaseInput::InputEventName::ENTER:
            if(!m_playerWon) {
                m_chipIsFalling = true;
            }
            else{
                // Display final message
                auto a = std::make_shared<TextDisplay>();
                a->setText(std::string("P") + std::to_string(1 + m_currPlayerId) + " wins.");
                m_ctrl->addApplication(a, true);
                m_hasFinished = true;
            }
            break;

        case BaseInput::InputEventName::EXIT:
            m_hasFinished = true;
            return;
        }
    }

    if(m_chipIsFalling && m_ctrl->getTimeMs() - m_lastFallTime > 100) {
        m_lastFallTime = m_ctrl->getTimeMs();
        int newPosY = m_currFallingChipPos.y;
        newPosY++;
        if(newPosY >= m_ctrl->getHeight() || m_gameField(newPosY,m_currFallingChipPos.x,0) != 2) {
            m_chipIsFalling = false;
            // First line has to stay empty
            if(newPosY > 1) {
                m_gameField(m_currFallingChipPos.y,m_currFallingChipPos.x,0) = m_currPlayerId;
                m_winPoints = checkForWin(m_currFallingChipPos);
                if(m_winPoints.size() == 4) {
                    m_playerWon = true;
                }else{
                    m_currPlayerId = (m_currPlayerId + 1) % 2;
                }
            }
            m_currFallingChipPos.x = 0;
            m_currFallingChipPos.y = 0;
        }else{
            m_currFallingChipPos.y++;
        }
    }
}

std::vector<led::Pointi> ConnectFour::checkForWin(led::Pointi insertedPoint){
    // Check for all 4 directions if there are more than 4 connected fields
    std::vector<led::Pointi> points = checkForWin(insertedPoint, led::Pointi(0,1));
    if(points.size() < 4) points = checkForWin(insertedPoint, led::Pointi(1, 0));
    if(points.size() < 4) points = checkForWin(insertedPoint, led::Pointi(1,1));
    if(points.size() < 4) points = checkForWin(insertedPoint, led::Pointi(1,-1));
    return points;
}
std::vector<led::Pointi> ConnectFour::checkForWin(led::Pointi insertedPoint, led::Pointi searchDir){
    std::vector<led::Pointi> points;
    led::Pointi currPos = insertedPoint;
    while(currPos.x >= 0 && currPos.x < m_gameField.width &&
          currPos.y >= 0 && currPos.y < m_gameField.height &&
          m_gameField(currPos.y,currPos.x,0) == m_currPlayerId) {
        points.push_back(currPos);
        currPos.x += searchDir.x;
        currPos.y += searchDir.y;
    }
    currPos = insertedPoint;
    currPos.x -= searchDir.x;
    currPos.y -= searchDir.y;
    while(currPos.x >= 0 && currPos.x < m_gameField.width &&
          currPos.y >= 0 && currPos.y < m_gameField.height &&
          m_gameField(currPos.y,currPos.x,0) == m_currPlayerId) {
        points.push_back(currPos);
        currPos.x -= searchDir.x;
        currPos.y -= searchDir.y;
    }
    std::cout << points.size() << std::endl;
    return points;
}

void ConnectFour::draw(Image& frame) {
    // Set the frame to palette index 0
    m_ctrl->clearFrame(2);

    memcpy(frame.data,m_gameField.data,m_gameField.size);

    if(m_chipIsFalling) {
        frame(m_currFallingChipPos.y,m_currFallingChipPos.x,0) = m_currPlayerId;
    }else{
        frame(m_currFallingChipPos.y,m_currFallingChipPos.x,0) = 3;
    }
    if(m_playerWon) {
        for(auto p: m_winPoints) {
            frame(p.y,p.x,0) = 4;
        }
    }
}
