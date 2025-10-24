#define BUILD_TYPE_H2 1 
#define BUILD_TYPE_C6 2

#ifdef BOARD_TYPE

#if BOARD_TYPE == BUILD_TYPE_H2
#pragma message "esp32-h2 stuff happening!"
#define I2C_SDA 13
#define I2C_SCL 14
#endif
#if BOARD_TYPE == BUILD_TYPE_C6
#pragma message "esp32-c6 stuff happening!"
#define I2C_SDA 6
#define I2C_SCL 4
#endif

#endif
