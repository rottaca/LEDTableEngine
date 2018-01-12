
#include "gameControllerInput.hpp"

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define I2C_CTRL_ADR_START 0x20
#define I2C_CTRL_ADR_END 0x27

GameControllerInput::GameControllerInput(std::string i2cDev) {
  m_deviceHandle = 0;
  m_i2cDevName = i2cDev;
}

GameControllerInput::~GameControllerInput() {}

bool GameControllerInput::initialize() {
  disconnect();
  if(!connectToDevice(m_i2cDevName))
    return false;

  detectConnectedControllers();
  return m_controllerId.size() > 0;
}

led::BaseInput::InputEvents GameControllerInput::getInputEvents() {
  led::BaseInput::InputEvents ie;

  for(int pId = 0; pId < m_controllerId.size(); pId++){
    //std::cout << "Read from dev " << m_controllerId[pId] << std::endl;

    if (ioctl(m_deviceHandle, I2C_SLAVE, m_controllerId[pId]) < 0)
      perror("ioctl() I2C_SLAVE failed\n");
    else
    {
      /* kann gelesen werden? */
      int res = i2c_smbus_read_byte(m_deviceHandle);
      if (res >= 0){

        BaseInput::InputEvent e;
        e.playerId = pId;
        e.state = InputEventState::KEY_PRESSED;

        // With the used hardware setup and the PCF8574
        // Key presses correspond zeros
        // -> Invert res from 0xff to 0x00 for no presses
        //std::cout << "Raw " << res << std::endl;
        //res = ~res && 0xFF);
        //std::cout << "Input: " << (res & 0x01) << std::endl;
        if((res & 0x01) == 0){
          std::cout << "A" << std::endl;
          e.name = InputEventName::A;
          ie.push_back(e);
        }
        if((res & 0x02) == 0){
          e.name = InputEventName::B;
          ie.push_back(e);
        }
        if((res & 0x04) == 0){
          e.name = InputEventName::EXIT;
          ie.push_back(e);
        }
        if((res & 0x08) == 0){
          e.name = InputEventName::ENTER;
          ie.push_back(e);
        }
        if((res & 0x10) == 0){
          e.name = InputEventName::DOWN;
          ie.push_back(e);
        }
        if((res & 0x20) == 0){
          e.name = InputEventName::UP;
          ie.push_back(e);
        }
        if((res & 0x40) == 0){
          e.name = InputEventName::LEFT;
          ie.push_back(e);
        }
        if((res & 0x80) == 0){
          e.name = InputEventName::RIGHT;
          ie.push_back(e);
        }
      }else{
        std::cout << "Failed to read from I2C device with address " << pId <<std::endl;
      }
    }
  }
  return ie;
}

void GameControllerInput::disconnect(){
  close(m_deviceHandle);
  m_deviceHandle = 0;
}

bool GameControllerInput::connectToDevice(std::string device){
  std::cout << "Opening i2c device " << device << " ...";
  if ((m_deviceHandle = open(device.c_str(), O_RDWR)) < 0)
  {
    perror("open() failed");
    return false;
  }
  std::cout << " OK" << std::endl;
  unsigned long funcs;
  if (ioctl(m_deviceHandle,I2C_FUNCS,&funcs) < 0)
    {
    perror("ioctl() I2C_FUNCS failed");
    exit (1);
    }

  /* Ergebnis untersuchen */
  if (funcs & I2C_FUNC_I2C)
    std::cout << "I2C available" << std::endl;
  if (funcs & (I2C_FUNC_SMBUS_BYTE))
    std::cout << "I2C_FUNC_SMBUS_BYTE available" << std::endl;

  return true;
}
void GameControllerInput::detectConnectedControllers(){
  m_controllerId.clear();
  int port, res;
  for (port = I2C_CTRL_ADR_START; port <= I2C_CTRL_ADR_END; port++)
  {
    if (ioctl(m_deviceHandle, I2C_SLAVE, port) < 0)
      perror("ioctl() I2C_SLAVE failed\n");
    else
    {
      /* kann gelesen werden? */
      res = i2c_smbus_read_byte(m_deviceHandle);
      if (res >= 0){
        std::cout << "i2c chip found at: "<< ", val = " << res << std::endl;
        m_controllerId.push_back(port);
      }
    }
  }
  std::cout << "Found " << m_controllerId.size() << " i2c controllers." << std::endl;
}
