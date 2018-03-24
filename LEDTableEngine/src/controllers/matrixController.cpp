#include <LEDTableEngine/controllers/matrixController.hpp>

#include <cmath>

#include <LEDTableEngine/configuration.hpp>

MatrixController::MatrixController() {
}

MatrixController::~MatrixController() {
}

bool MatrixController::initialize(size_t width, size_t height,
                                  std::shared_ptr<led::BaseInput>input, bool debug) {
    if (!BaseController::initialize(width, height, input, debug)) return false;

    m_tpm2 = std::make_shared<TPM2>();

    if (!m_tpm2->connect(LED_USB_CONTROLLER_DEV)) {
        return false;
    }
    m_frameDataPacket = m_tpm2->createTPM2DataPacket(width * height * 3);
    m_tpm2->sendPacket(m_frameDataPacket);
    return true;
}

void MatrixController::copyImageToBuffer(const led::Image& frame) {
    const led::Palette& palette = getCurrentPalette();
    uint8_t *pixels             = m_frameDataPacket.dataPtr;
    uint8_t *p;
    size_t idx = 0;
    led::ColorRGB c;


    size_t y_matrix, y_matrix_inverted;
    size_t x_matrix, x_matrix_inverted;

    #ifdef LED_MATRIX_FLIP_Y
      y_matrix = m_height - 1;
      y_matrix_inverted = 0;
    #else
      y_matrix = 0;
      y_matrix_inverted = m_height - 1;
    #endif
    #ifdef LED_MATRIX_FLIP_X
      x_matrix = m_width - 1;
      x_matrix_inverted = 0;
    #else
      x_matrix = 0;
      x_matrix_inverted = m_height - 1;
    #endif

    for (size_t y = 0; y < m_height; y++) {
        #ifdef LED_MATRIX_FLIP_Y
          y_matrix--;
          y_matrix_inverted++;
        #else
          y_matrix++;
          y_matrix_inverted--;
        #endif

        for (size_t x = 0; x < m_width; x++){
          #ifdef LED_MATRIX_FLIP_X
            x_matrix--;
            x_matrix_inverted++;
          #else
            x_matrix++;
            x_matrix_inverted--;
          #endif

          if (x_matrix % 2 == 0){
              p = pixels + 3 * (y_matrix + x_matrix * m_height);
          }
          else {
              p = pixels + 3 * (y_matrix_inverted + x_matrix * m_height);
          }

          // Palette mode
          if (m_bufferMode == led::BufferColorMode::PALETTE) {
              int p = frame.data[idx++];
              c = palette[p];
          }

          // RGB Mode
          else {
              c[0] = frame.data[idx++];
              c[1] = frame.data[idx++];
              c[2] = frame.data[idx++];
          }

          *(p+0) = c[0] * m_brightness;
          *(p+1) = c[1] * m_brightness;
          *(p+2) = c[2] * m_brightness;
      }
    }
}

void MatrixController::showFrame(const led::Image& frame) {
    copyImageToBuffer(frame);
    m_tpm2->sendPacket(m_frameDataPacket);
}

void MatrixController::shutdown() {
}
