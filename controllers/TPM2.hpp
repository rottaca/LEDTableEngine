#ifndef _H_TPM2_
#define _H_TPM2_

#include <memory>
#include <string>

#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

class TPM2{
public:
  const uint8_t kStartByte = 0xC9;
  const uint8_t kEndByte = 0x36;

  enum BlockTypes{
    DATA=0xDA,
    CMD=0xC0,
    ACK=0xAC,
    ACKDATA=0xAD
  };

  struct Packet{
    BlockTypes type;
    std::shared_ptr<uint8_t> bufferPtr;
    uint8_t* dataPtr;
    size_t dataSize;
  };

private:
  int m_deviceFile;
  struct termios m_serialConfig;

public:
  TPM2();
  ~TPM2();

  Packet createTPM2DataPacket(size_t dataSz);
  bool connect(const std::string &device);
  void disconnect();
  bool sendPacket(const Packet& p);
private:
  bool configureSerial();

};

#endif
