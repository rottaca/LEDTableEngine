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

// Minimum time since the last user inputs
// to switch to the standby mode (milliseconds)
// This is only available, if the currently active
// application supports this feature
#define MINIMUM_AUTO_STANDBY_TIME_MS (20 * 1000)

// Time between subsequent checks for new input events
// if the engine is in standby mode
#define STANDBY_CHECK_TIME_MS (1000)

// ////////////////////////////////////
// LED-Matrix Settings
// ////////////////////////////////////

// The device file that allows the communication the
// LED controller chip via a serial connection.
// This name might change, if you use a different controller.
// Could be also ttyUSB*
#define LED_USB_CONTROLLER_DEV "/dev/ttyACM0"

// Uncomment the line below if your
// matrix orienation is flipped from left to right
//#define LED_MATRIX_FLIP_X

// Uncomment the line below if your
// matrix orienation is flipped from top to bottom
//#define LED_MATRIX_FLIP_Y

// If your matrix table uses the zig-zag layout along the X direction (up/down),
// uncomment this option.
// ATTENTION: YOU HAVE TO COMMENT OUT LED_MATRIX_ZIGZAG_Y
//#define LED_MATRIX_ZIGZAG_X

// If your matrix table uses the zig-zag layout along the Y direction (left/right),
// uncomment this option.
// ATTENTION: YOU HAVE TO COMMENT OUT LED_MATRIX_ZIGZAG_X
#define LED_MATRIX_ZIGZAG_Y

// ////////////////////////////////////
// Game-Controller Settings
// ////////////////////////////////////

// The values below are the actual range of adresses, that are used
// by your I2C port extender. Have a look into its documentation
// and change the values here accordingly.
// If you are not sure, you can use i2ctools to find your device by calling
// > sudo i2cdetect -r 1
// Please have a look at the documentation.

// Start address for the device scan
#define I2C_CTRL_ADR_START 0x20

// End adress for the device scan
#define I2C_CTRL_ADR_END 0x27

// ////////////////////////////////////
// Key-Mappings
// ////////////////////////////////////

// Helper macro to convert index into binary
#define BIN_FROM_IDX(I) (1 << I)

// Keymappings for Game-Controllers (I2C)
// The value of each symbol corresponds
// to the pin on the i2c port extender.
// Each value should only occur once!

// Keymappings (for all players)
#define I2C_CTRL_KEYMAP_A BIN_FROM_IDX(0)
#define I2C_CTRL_KEYMAP_B BIN_FROM_IDX(1)
#define I2C_CTRL_KEYMAP_EXIT BIN_FROM_IDX(2)
#define I2C_CTRL_KEYMAP_ENTER BIN_FROM_IDX(3)
#define I2C_CTRL_KEYMAP_DOWN BIN_FROM_IDX(4)
#define I2C_CTRL_KEYMAP_UP BIN_FROM_IDX(5)
#define I2C_CTRL_KEYMAP_LEFT BIN_FROM_IDX(6)
#define I2C_CTRL_KEYMAP_RIGHT BIN_FROM_IDX(7)

// Keymappings for Keyboard
// Player 0
#define KEYBOARD_CTRL_0_KEYMAP_A (KEY_R)
#define KEYBOARD_CTRL_0_KEYMAP_B (KEY_F)
#define KEYBOARD_CTRL_0_KEYMAP_EXIT (KEY_Q)
#define KEYBOARD_CTRL_0_KEYMAP_ENTER (KEY_E)
#define KEYBOARD_CTRL_0_KEYMAP_DOWN (KEY_S)
#define KEYBOARD_CTRL_0_KEYMAP_UP (KEY_W)
#define KEYBOARD_CTR_0_KEYMAP_LEFT (KEY_A)
#define KEYBOARD_CTRL_0_KEYMAP_RIGHT (KEY_D)
// Player 1
#define KEYBOARD_CTRL_1_KEYMAP_A (KEY_O)
#define KEYBOARD_CTRL_1_KEYMAP_B (KEY_L)
#define KEYBOARD_CTRL_1_KEYMAP_EXIT (KEY_BACKSPACE)
#define KEYBOARD_CTRL_1_KEYMAP_ENTER (KEY_ENTER)
#define KEYBOARD_CTRL_1_KEYMAP_DOWN (KEY_DOWN)
#define KEYBOARD_CTRL_1_KEYMAP_UP (KEY_UP)
#define KEYBOARD_CTRL_1_KEYMAP_LEFT (KEY_LEFT)
#define KEYBOARD_CTRL_1_KEYMAP_RIGHT (KEY_RIGHT)


////////////////////////////////////////////////////////////////////////////////
/////////////////////// Parameter Validation ///////////////////////////////////
// ////////////////////////////////////
// LED-Matrix Settings
// ////////////////////////////////////
#if defined(LED_MATRIX_ZIGZAG_X) && defined(LED_MATRIX_ZIGZAG_Y)
  #error "LED_MATRIX_ZIGZAG_X and LED_MATRIX_ZIGZAG_Y are defined at the same time!\
   Please comment out one of both symbols!"
#endif

#if ! defined(LED_MATRIX_ZIGZAG_X) && ! defined(LED_MATRIX_ZIGZAG_Y)
  #error "No matrix zigzag layout defined!"
#endif




#endif // ifndef CONFIGURATION_H_
