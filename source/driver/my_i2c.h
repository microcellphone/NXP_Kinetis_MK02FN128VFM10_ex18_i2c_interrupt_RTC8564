#ifndef __MY_I2C_H__
#define __MY_I2C_H__

#include "board.h"
#include "stdint.h"
#include "fsl_i2c.h"

#define I2C_BUFFER_SIZE	  0x10

//#define DEFAULT_I2C          I2C0
//#define SPEED_100KHZ         100000
//#define SPEED_400KHZ         400000
//#define I2C_DEFAULT_SPEED    SPEED_100KHZ
//#define I2C_FASTPLUS_BIT     0

enum I2C_MODE {
	I2C_INTERRUPT_MODE = 0,
	I2C_POLLING_MODE,
	I2C_MODE_MAX
};

extern uint32_t i2c_Initialize_Request(void);
extern uint32_t i2c_Config_Request(void);
extern uint32_t i2c_Receive_Data_Request(I2C_Type *base, uint8_t *rcv_buff, uint32_t rcv_length);
extern uint32_t i2c_Send_Data_Request(I2C_Type *base, uint16_t i2c_slave_address, uint8_t *snd_buff, uint32_t snd_length);
extern uint32_t i2c_Send_Receive_Data_Request(I2C_Type *base, uint16_t i2c_slave_address, uint8_t *snd_buff, uint32_t snd_length, uint8_t *rcv_buff, uint32_t rcv_length);
extern uint32_t i2c_Tx_Rx_Data_Request(I2C_Type *base, uint16_t i2c_slave_address, uint8_t *snd_buff, uint32_t snd_length, uint8_t *rcv_buff, uint32_t rcv_length);

#endif // __MY_I2C_H__
