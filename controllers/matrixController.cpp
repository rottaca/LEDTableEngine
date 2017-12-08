#include "matrixController.hpp"
#include <cmath>

MatrixController::MatrixController() {}

MatrixController::~MatrixController() {}

bool MatrixController::initialize(size_t width, size_t height,
                                  std::shared_ptr<BaseInput>input, bool debug) {
  BaseController::initialize(width, height, input, debug);
  m_tpm2 = std::make_shared<TPM2>();

  if (!m_tpm2->connect("/dev/ttyACM0")) {
    return false;
  }
  m_frameDataPacket = m_tpm2->createTPM2DataPacket(width * height * 3);
  m_tpm2->sendPacket(m_frameDataPacket);
  return true;
}

void MatrixController::copyImageToBuffer(const Image& frame) {
  const BaseApplication::Palette& palette = getCurrentPalette();
  uint8_t *pixels                         = m_frameDataPacket.dataPtr;
  uint8_t *p;
  size_t   idx = 0;
  uint8_t  r, g, b;

  for (size_t y = 0; y < m_height; y++) {
    p = pixels + 3 * y;

    for (size_t x = 0; x < m_width; x++) {
      if(x%2==0)
      	p = pixels + 3*(y + x*m_height);
      else
        p = pixels + 3*(m_height- 1 - y + x*m_height);

      // Palette mode
      if (m_bufferMode == BufferColorMode::PALETTE) {
        int c = frame.data[idx++];
        r = palette[c * 3];
        g = palette[c * 3 + 1];
        b = palette[c * 3 + 2];
      }

      // RGB Mode
      else {
        r = frame.data[idx++];
        g = frame.data[idx++];
        b = frame.data[idx++];
      }
      *(p + 0) = r;
      *(p + 1) = g;
      *(p + 2) = b;
    }
  }
}

void MatrixController::showFrame(const Image& frame) {
  copyImageToBuffer(frame);
  m_tpm2->sendPacket(m_frameDataPacket);
}

void MatrixController::shutdown() {}
