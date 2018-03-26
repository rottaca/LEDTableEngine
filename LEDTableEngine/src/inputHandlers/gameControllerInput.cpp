#include <LEDTableEngine/inputHandlers/gameControllerInput.hpp>

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <LEDTableEngine/configuration.hpp>


namespace led{
GameControllerInput::GameControllerInput(std::string i2cDev) {
    m_deviceHandle = 0;
    m_i2cDevName   = i2cDev;
}

GameControllerInput::~GameControllerInput() {
}

bool GameControllerInput::initialize() {
    disconnect();

    if (!connectToDevice(m_i2cDevName)) return false;

    detectConnectedControllers();
    return m_controllerId.size() > 0;
}

BaseInput::InputEvents GameControllerInput::getInputEvents() {
    BaseInput::InputEvents ie;

    for (int pId = 0; pId < m_controllerId.size(); pId++) {
        if (ioctl(m_deviceHandle, I2C_SLAVE, m_controllerId[pId]) < 0) perror(
                "ioctl() I2C_SLAVE failed\n");
        else
        {
            int res = i2c_smbus_read_byte(m_deviceHandle);

            if (res >= 0) {
                BaseInput::InputEvent e;
                e.playerId = static_cast<PlayerID>(pId);
                e.state    = InputEventState::KEY_PRESSED;

                // With the used hardware setup and the PCF8574,
                // Key presses result in zeros
                if ((res & (I2C_CTRL_KEYMAP_A)) == 0) {
                    e.name = InputEventName::A;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_B)) == 0) {
                    e.name = InputEventName::B;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_EXIT)) == 0) {
                    e.name = InputEventName::EXIT;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_ENTER)) == 0) {
                    e.name = InputEventName::ENTER;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_DOWN)) == 0) {
                    e.name = InputEventName::DOWN;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_UP)) == 0) {
                    e.name = InputEventName::UP;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_LEFT)) == 0) {
                    e.name = InputEventName::LEFT;
                    ie.push_back(e);
                }

                if ((res & (I2C_CTRL_KEYMAP_RIGHT)) == 0) {
                    e.name = InputEventName::RIGHT;
                    ie.push_back(e);
                }
            } else {
                std::cout << "Failed to read from I2C device with address " << pId << std::endl;
            }
        }
    }
    return ie;
}

void GameControllerInput::disconnect() {
    close(m_deviceHandle);
    m_deviceHandle = 0;
}

bool GameControllerInput::connectToDevice(std::string device) {
    std::cout << "Opening i2c device " << device << " ...";

    if ((m_deviceHandle = open(device.c_str(), O_RDWR)) < 0)
    {
        perror("Opening the i2c device file failed");
        return false;
    }
    std::cout << " OK" << std::endl;
    unsigned long funcs;

    if (ioctl(m_deviceHandle, I2C_FUNCS, &funcs) < 0)
    {
        perror("ioctl() I2C_FUNCS failed");
        exit(1);
    }

    if (funcs & I2C_FUNC_I2C) std::cout << "I2C available" << std::endl;
    else return false;

    if (funcs & I2C_FUNC_SMBUS_BYTE) std::cout << "I2C_FUNC_SMBUS_BYTE available" << std::endl;
    else return false;

    return true;
}

void GameControllerInput::detectConnectedControllers() {
    m_controllerId.clear();
    int port, res;

    for (port = I2C_CTRL_ADR_START; port <= I2C_CTRL_ADR_END; port++)
    {
        if (ioctl(m_deviceHandle, I2C_SLAVE, port) < 0) perror("ioctl() I2C_SLAVE failed\n");
        else
        {
            res = i2c_smbus_read_byte(m_deviceHandle);

            if (res >= 0) {
                std::cout << "i2c chip found at address: " << port << ", val = " << res << std::endl;
                m_controllerId.push_back(port);
            }
        }
    }
    std::cout << "Found " << m_controllerId.size() << " i2c controllers." << std::endl;
}
size_t GameControllerInput::getSupportedPlayerCnt(){
  return m_controllerId.size();
}
}
