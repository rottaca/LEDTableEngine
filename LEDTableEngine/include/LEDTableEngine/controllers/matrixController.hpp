#ifndef _H_MATRIX_CONTROLLER_
#define _H_MATRIX_CONTROLLER_

#include <LEDTableEngine/core/baseController.hpp>
#include <LEDTableEngine/controllers/TPM2.hpp>

/**
 * @brief Matrix controller implementation that renders the generated image
 * onto the LED matrix hardware.
 */
class MatrixController : public led::BaseController {
public:

    MatrixController();
    ~MatrixController();

    bool initialize(size_t width,
                    size_t height,
                    std::shared_ptr<led::BaseInput>input,
                    bool debug = false);

private:

    void showFrame(const led::Image& frame);
    void shutdown();
    void copyImageToBuffer(const led::Image& frame);

    TPM2::Packet m_frameDataPacket;
    std::shared_ptr<TPM2> m_tpm2;
};


#endif // ifndef _H_MATRIX_CONTROLLER_
