#include <LEDTableEngine/controllers/TMP2.hpp>
#include <iostream>

TMP2::TMP2(){
}
TMP2::~TMP2(){
}

void TMP2::disconnect(){
    close(m_deviceFile);
}
bool TMP2::connect(const std::string &device)
{
    m_deviceFile = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(m_deviceFile == -1) {
        std::cout << "failed to open port " << device << std::endl;
        return false;
    }else{
        std::cout << "Connected to " << device << std::endl;
    }

    std::cout << "Configure serial device "<< std::endl;
    if(!configureSerial()) {
        std::cout << "Configure serial device failed."<< std::endl;
        close(m_deviceFile);
        return false;
    }
    return true;
}
bool TMP2::sendPacket(const Packet& p){
    // Wait for all previous data to be written
    // This allows the transmition of the previous frame until now!
    tcdrain(m_deviceFile);
    int sz = write(m_deviceFile,p.bufferPtr.get(),p.dataSize+5);
    return sz == p.dataSize+5;
}

TMP2::Packet TMP2::createTMP2DataPacket(size_t dataSz){
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

bool TMP2::configureSerial(){
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
TMP2File::TMP2File(){
  m_file.close();
}

bool TMP2File::loadTMP2File(const std::string& fileName, bool loop){
  m_file.close();
  m_file.open(fileName);
  m_loop = loop;
  if(!m_file.is_open()){
    std::cout << "Can't open tmp2 file " << fileName << std::endl;
  }
  return m_file.is_open();
}

bool TMP2File::getNextImage(led::Image &imgBuffer){
  if(!m_file) return false;

  while (true) {
    int c = m_file.get();
    // Restart file if requested
    if(c < 0 && m_loop){
      m_file.clear();
      m_file.seekg (0, m_file.beg);
      c = m_file.get();
    }

    if(c < 0 || c != TMP2::kStartByte){
      std::cerr << "Invalid byte read from tmp2 file (expected kStartByte): " << c << std::endl;
      return false;
    }
    // Get type
    int t = m_file.get();
    switch (t) {
      case TMP2::BlockTypes::DATA:
      case TMP2::BlockTypes::CMD:
      case TMP2::BlockTypes::ACK:
      case TMP2::BlockTypes::ACKDATA:{
        int s1 = 0,s2 = 0;
        if( (s2 = m_file.get())<0 || (s1 = m_file.get())<0 ){
          std::cerr << "Invalid byte read from tmp2 file (expected size bytes): " << s1  << s2 << std::endl;
          return false;
        }
        size_t sz = (s2 << 8) | s1;
        if (t != TMP2::BlockTypes::DATA){
          m_file.seekg (m_file.tellg()+sz+1);
        }else{
          if(imgBuffer.size != sz){
            std::cerr << "Invalid data size read from tmp2 file: " << sz << std::endl;
            return false;
          }
          m_file.read((char*)imgBuffer.data,sz);
          m_file.seekg (m_file.tellg()+1);
          return m_file.good();
        }
        break;
      }
      default:
        std::cerr << "Invalid byte read from tmp2 file (expected BlockType): " << c << std::endl;
        return false;
    }
  }
  return true;
}
