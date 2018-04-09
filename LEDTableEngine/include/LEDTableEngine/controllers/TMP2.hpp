#ifndef _H_TMP2_
#define _H_TMP2_

#include <memory>
#include <fstream>
#include <string>

#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions

#include <LEDTableEngine/core/types.hpp>

/**
 * @brief TMP2 helper class to generate output images
 */
class TMP2 {
public:

    static const uint8_t kStartByte = 0xC9;
    static const uint8_t kEndByte   = 0x36;

    enum BlockTypes {
        DATA    = 0xDA,
        CMD     = 0xC0,
        ACK     = 0xAC,
        ACKDATA = 0xAD
    };

    struct Packet {
        BlockTypes type;
        std::shared_ptr<uint8_t>bufferPtr;
        uint8_t                *dataPtr;
        size_t dataSize;
    };

private:

    int m_deviceFile;
    struct termios m_serialConfig;

public:

    TMP2();
    ~TMP2();
    /**
     * Create a tmp2 data package with a predefined data size
     * @param  dataSz Size of data
     */
    Packet createTMP2DataPacket(size_t dataSz);
    /**
     * Connect to a serial device that is able to process TMP2 data
     * @param  device Serial device file
     * @return        True on success, False on error
     */
    bool   connect(const std::string& device);
    /**
     * Disconnect from serial device
     */
    void   disconnect();
    /**
     * Send a packet via the serial connection to the device
     * @return   True on success, False on error
     */
    bool   sendPacket(const Packet& p);

private:

    bool configureSerial();
};
/**
 * @brief Helper class to convert tmp2 files to renderable images.
 */
class TMP2File {
private:
  std::ifstream m_file;
  bool m_loop;
public:
  TMP2File();
  /**
   * Load a tmp2 file and prepare for processing
   * @param fileName
   * @param loop     Loop the file
   * @return         True on success, False on error
   */
  bool loadTMP2File(const std::string& fileName, bool loop);
  /**
   * Extract the next image that should be displayed
   * @param  imgBuffer Output image that will be filled with the image data.
   *                   Should have the expected image dimensions!
   * @return           Returns false, if there are no new images available
   */
  bool getNextImage(led::Image &imgBuffer);

};

#endif // ifndef _H_TMP2_
