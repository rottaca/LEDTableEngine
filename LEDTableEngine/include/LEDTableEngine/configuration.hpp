#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

// ////////////////////////////////////
// General Settings
// ////////////////////////////////////
// The actual width of the matrix table in pixels
#define PIXEL_MATRIX_WIDTH (20)

// The actual height of the matrix table in pixels
#define PIXEL_MATRIX_HEIGHT (15)

// The refresh rate for rendering the content.
#define UPDATE_RATE (25)

// The device file that allows the communication the
// LED controller chip via a serial connection.
// This name might change, if you use a different controller.
// Could be also ttyUSB*
#define LED_USB_CONTROLLER_DEV "/dev/ttyACM0"

// ////////////////////////////////////
// Game-Controller Settings
// ////////////////////////////////////
// The values below are the actual range of adresses, that are used
// by your I2C port extender. Have a look into its documentation
// and change the values here accordingly.
// If you are not sure, you can use i2ctools to find your device by calling
// > sudo i2cdetect -r 1
// Have a look at its documentation.

// Start address for the device scan
#define I2C_CTRL_ADR_START 0x20

// End adress for the device scan
#define I2C_CTRL_ADR_END 0x27

#endif // ifndef CONFIGURATION_H_
