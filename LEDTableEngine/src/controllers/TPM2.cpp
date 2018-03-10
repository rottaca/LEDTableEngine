#include <LEDTableEngine/controllers/TPM2.hpp>
#include <iostream>

TPM2::TPM2(){



}
TPM2::~TPM2(){
}

void TPM2::disconnect(){
  close(m_deviceFile);
}
bool TPM2::connect(const std::string &device)
{
  m_deviceFile = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if(m_deviceFile == -1) {
    std::cout << "failed to open port " << device << std::endl;
    return false;
  }else{
    std::cout << "Connected to " << device << std::endl;
  }

  std::cout << "Configure serial device "<< std::endl;
  if(!configureSerial()){
    std::cout << "Configure serial device failed."<< std::endl;
    close(m_deviceFile);
    return false;
  }
  return true;
}
bool TPM2::sendPacket(const Packet& p){
  // Wait for all previous data to be written
  // This allows the transmition of the previous frame until now!
  tcdrain(m_deviceFile);
  int sz = write(m_deviceFile,p.bufferPtr.get(),p.dataSize+5);
  return sz == p.dataSize+5;
}

TPM2::Packet TPM2::createTPM2DataPacket(size_t dataSz){
  Packet p;
  size_t packetSize = 4 + dataSz + 1;
  p.dataSize = dataSz;
  uint8_t* rawBuffer = new uint8_t[packetSize];
  std::shared_ptr<uint8_t> buffer( rawBuffer,
                                     std::default_delete<uint8_t[]>() );
  p.bufferPtr = buffer;
  p.dataPtr = rawBuffer + 4;
  p.type = BlockTypes::DATA;
  rawBuffer[0] = kStartByte;
  rawBuffer[1] = BlockTypes::DATA;
  uint8_t high = (dataSz >> 8);
  rawBuffer[2] = high;
  uint8_t low = dataSz;
  rawBuffer[3] = low;
  rawBuffer[packetSize-1] = kEndByte;
  return p;
}

bool TPM2::configureSerial(){
  //
 // Check if the file descriptor is pointing to a TTY device or not.
 //
 if(!isatty(m_deviceFile)) {
   std::cout << "File is not a TTY device" << std::endl;
   return false;
 }

 //
 // Get the current configuration of the serial interface
 //
 if(tcgetattr(m_deviceFile, &m_serialConfig) < 0) {
   std::cout << "Failed to get serial interface config" << std::endl;
   return false;
 }

 //
 // Input flags - Turn off input processing
 //
 // convert break to null byte, no CR to NL translation,
 // no NL to CR translation, don't mark parity errors or breaks
 // no input parity check, don't strip high bit off,
 // no XON/XOFF software flow control
 //
 m_serialConfig.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                     INLCR | PARMRK | INPCK | ISTRIP | IXON);

 //
 // Output flags - Turn off output processing
 //
 // no CR to NL translation, no NL to CR-NL translation,
 // no NL to CR translation, no column 0 CR suppression,
 // no Ctrl-D suppression, no fill characters, no case mapping,
 // no local output processing
 //
 // m_serialConfig.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
 //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
 m_serialConfig.c_oflag = 0;

 //
 // No line processing
 //
 // echo off, echo newline off, canonical mode off,
 // extended input processing off, signal chars off
 //
 m_serialConfig.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

 //
 // Turn off character processing
 //
 // clear current char size mask, no parity checking,
 // no output processing, force 8 bit input
 //
 m_serialConfig.c_cflag &= ~(CSIZE | PARENB);
 m_serialConfig.c_cflag |= CS8;

 //
 // One input byte is enough to return from read()
 // Inter-character timer off
 //
 m_serialConfig.c_cc[VMIN]  = 1;
 m_serialConfig.c_cc[VTIME] = 0;

 //
 // Communication speed (simple version, using the predefined
 // constants)
 //
 if(cfsetispeed(&m_serialConfig, B115200) < 0 || cfsetospeed(&m_serialConfig, B115200) < 0) {
   std::cout << "Failed to set serial speed" << std::endl;
   return false;
 }

 //
 // Finally, apply the configuration
 //
 if(tcsetattr(m_deviceFile, TCSAFLUSH, &m_serialConfig) < 0) {
   std::cout << "Failed to save configuration" << std::endl;
   return false;
 }

 return true;
}
