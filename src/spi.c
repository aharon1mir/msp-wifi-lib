#include <spi.h>


void deselect_sd()
{
    SPI_DIR |= SS_SD;
    SPI_OUT |= SS_SD;
}

void deselect_wifi()
{
    SPI_DIR |= SS_WIFI;
    SPI_OUT |= SS_WIFI;
}

void Wifi_spi_begin()
{
    // turn off the led.
    SPI_DIR |= WIFI_BOARD_LED; 
    SPI_OUT &= ~WIFI_BOARD_LED;
}

void spi_initate()
{
    // set SPI peripheral master mode 

    // SS disable
    SPI_DIR |= SS_WIFI;
    SPI_OUT |= SS_WIFI;

    // SPI peripheral enable
    P1SEL   |= SPICLOCK | DATAIN | DATAOUT;
    P1SEL2  |= SPICLOCK | DATAIN | DATAOUT;
    
    // SPI config
}

uint8_t sndArcv(volatile register uint8_t data)
{
    /*
     * need to delay for read the result buffer...
     */

}

uint16_t sndArcvW(volatile register uint16_t data)
{
    uint8_t data1, data2;
    data1 = (uint8_t) (data & 0xff00) >> 8;
    data2 = (uint8_t) data & 0xff;
    data1 = sndArcv(data1);
    data2 = sndArcv(data2);
    data = ((data1 << 8) & 0xff00) | data2;
    return data;
}

int getSlaveSign()
{
    return SPI_IN & SLAVEREADY;
}

void waitForSlaveSign()
{
    while (!getSlaveSign());
}

void waitForSlaveReady()
{
    while (getSlaveSign());
}

void delay(unsigned int time)
{
    while (time) time--;
}


void sendCmd(uint8_t cmd, uint8_t numParam)
{
    send(START_CMD);

    delay(SPI_START_CMD_DELAY - DELAY_SUB);

    send(cmd & ~(REPLY_FLAG));

    send(numParam);

    if (!numParam)
        send(END_CMD);

}

void sendParamW(uint16_t paramLen)
{
    sendW(paramLen);
}

void sendParamC(uint8_t paramLen)
{
    send(paramLen);
}

void sendParam(uint16_t param, uint16_t lastParam)
{
    sendParamC(2); // param len
    
    sendW(param);

    if (lastParam == 1)
        send(END_CMD);
}

uint16_t readParamW(uint16_t* len)
{
    uint16_t _param_len = send(DUMMY)<<8 | (send(DUMMY)&0xff);
    if (len != NULL)
        *len = _param_len;

    return _param_len;
}

uint8_t readParamC(uint8_t* len)
{
    uint8_t param_len = send(DUMMY);
    if (len != NULL)
        *len = param_len;
    return param_len;
}
void sendBuffer(uint8_t* param, uint16_t param_len, uint8_t lastParam)
{
    uint16_t i = 0;
    sendParamW(param_len);
    for (; i < param_len; ++i)
        send(param[i]);
    if (lastParam == 1)
        send(END_CMD);
}

