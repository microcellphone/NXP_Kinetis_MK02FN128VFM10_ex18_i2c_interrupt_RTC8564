#include "fsl_i2c.h"
#include "my_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
i2c_master_handle_t g_m_handle;
i2c_master_config_t masterConfig;
volatile bool completionFlag = false;
volatile bool nakFlag        = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success) {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if ((status == kStatus_I2C_Nak) || (status == kStatus_I2C_Addr_Nak)) {
        nakFlag = true;
    }
}

uint32_t i2c_Config_Request(void)
{
	return 0;
}


uint32_t i2c_Send_Data_Request(I2C_Type *base, uint16_t i2c_slave_address, uint8_t *snd_buff, uint32_t snd_length)
{
    i2c_master_transfer_t masterXfer;
    status_t i2c_status;

    completionFlag = false;
    nakFlag        = false;

    memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    masterXfer.slaveAddress   = i2c_slave_address;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = snd_buff;
    masterXfer.dataSize       = snd_length;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferCreateHandle(base, &g_m_handle, i2c_master_callback, NULL);
	i2c_status = I2C_MasterTransferNonBlocking(base, &g_m_handle, &masterXfer);
	if(i2c_status != kStatus_Success) while(1);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag));
    nakFlag = false;

    if (completionFlag == true) {
    	completionFlag = false;
    } else {
    	while(1);
    }

    return snd_length;
}

uint32_t i2c_Send_Receive_Data_Request(I2C_Type *base, uint16_t i2c_slave_address, uint8_t *snd_buff, uint32_t snd_length, uint8_t *rcv_buff, uint32_t rcv_length)
{
	i2c_master_transfer_t masterXfer;
	status_t i2c_status;

    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    masterXfer.slaveAddress   = i2c_slave_address;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = snd_buff[0];
    masterXfer.subaddressSize = 1;
    masterXfer.data           = rcv_buff;
    masterXfer.dataSize       = rcv_length;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferCreateHandle(base, &g_m_handle, i2c_master_callback, NULL);
	i2c_status = I2C_MasterTransferNonBlocking(base, &g_m_handle, &masterXfer);
	if(i2c_status != kStatus_Success) while(1);

    /*  wait for transfer completed. */
    while ((!nakFlag) && (!completionFlag));
    nakFlag = false;

    if (completionFlag == true) {
    	completionFlag = false;
    } else {
    	while(1);
    }

    return rcv_buff[0];
}
