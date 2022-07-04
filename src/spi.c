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

void sendParam_W(uint16_t param, uint8_t lastParam)
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

int waitSpiChar(unsigned char waitChar)
{
    int timeout = TIMEOUT_CHAR;
    unsigned char _readChar = 0;
    do{
        _readChar = send(DUMMY);
        if (_readChar == ERR_CMD)
        {
        	//WARN("Err cmd received\n");
        	return -1;
        }
    }while((timeout-- > 0) && (_readChar != waitChar));
    return  (_readChar == waitChar);
}


int waitResponseCmd(uint8_t cmd, uint8_t numParam, uint8_t* param, uint8_t* param_len)
{
    char _data = 0;
    int ii = 0;

    IF_CHECK_START_CMD(_data)
    {
        CHECK_DATA(cmd | REPLY_FLAG, _data){};

        CHECK_DATA(numParam, _data);
        {
            readParamC(param_len);
            for (ii=0; ii<(*param_len); ++ii)
            {
                // Get Params data
                //param[ii] = spiTransfer(DUMMY_DATA);
                param[ii] = send(DUMMY);
                //getParam(&param[ii]);
            }
        }

        readAndCheckChar(END_CMD, &_data);
    }

    return 1;
}

int readAndCheckChar(char checkChar, char* readChar)
{
    *readChar = send(DUMMY);

    return  (*readChar == checkChar);
}

int waitResponseDataW(uint8_t cmd, uint8_t* param, uint16_t* param_len)
{
    char _data = 0;
    uint16_t ii = 0;

    IF_CHECK_START_CMD(_data)
    {
        CHECK_DATA(cmd | REPLY_FLAG, _data){};

        uint8_t numParam = send(DUMMY);
        if (numParam != 0)
        {
            readParamW(param_len);
            for (ii=0; ii<(*param_len); ++ii)
            {
                // Get Params data
                param[ii] = send(DUMMY);
            }
        }

        readAndCheckChar(END_CMD, &_data);
    }

    return 1;
}

int waitResponseDataC(uint8_t cmd, uint8_t* param, uint8_t* param_len)
{
    char _data = 0;
    int ii = 0;

    IF_CHECK_START_CMD(_data)
    {
        CHECK_DATA(cmd | REPLY_FLAG, _data){};

        uint8_t numParam = send(DUMMY);
        if (numParam != 0)
        {
            readParamC(param_len);
            for (ii=0; ii<(*param_len); ++ii)
            {
                // Get Params data
                param[ii] = send(DUMMY);
            }
        }

        readAndCheckChar(END_CMD, &_data);
    }

    return 1;
}

int waitResponseParams(uint8_t cmd, uint8_t numParam, tParam* params)
{
    char _data = 0;
    int i =0, ii = 0;


    IF_CHECK_START_CMD(_data)
    {
        CHECK_DATA(cmd | REPLY_FLAG, _data){};

        uint8_t _numParam = send(DUMMY);
        if (_numParam != 0)
        {
            for (i=0; i<_numParam; ++i)
            {
                params[i].paramLen = readParamC(NULL);
                for (ii=0; ii<params[i].paramLen; ++ii)
                {
                    // Get Params data
                    params[i].param[ii] = send(DUMMY);
                }
            }
        } else
        {
            return 0;
        }

        if (numParam != _numParam)
        {
            return 0;
        }

        readAndCheckChar(END_CMD, &_data);
    }
    return 1;
}


int waitResponse(uint8_t cmd, uint8_t* numParamRead, uint8_t** params, uint8_t maxNumParams)
{
    char _data = 0;
    int i =0, ii = 0;

    char    *index[WL_SSID_MAX_LENGTH];

    for (i = 0 ; i < WL_NETWORKS_LIST_MAXNUM ; i++)
            index[i] = (char *)params + WL_SSID_MAX_LENGTH*i;

    IF_CHECK_START_CMD(_data)
    {
        CHECK_DATA(cmd | REPLY_FLAG, _data){};

        uint8_t numParam = send(DUMMY);

        if (numParam > maxNumParams)
        {
            numParam = maxNumParams;
        }
        *numParamRead = numParam;
        if (numParam != 0)
        {
            for (i=0; i<numParam; ++i)
            {
            	uint8_t paramLen = readParamC(NULL);
                for (ii=0; ii<paramLen; ++ii)
                {
                	//ssid[ii] = spiTransfer(DUMMY_DATA);
                    // Get Params data
                    index[i][ii] = (uint8_t)send(DUMMY);

                }
                index[i][ii]=0;
            }
        } else
        {
            readAndCheckChar(END_CMD, &_data);
            return 0;
        }
        readAndCheckChar(END_CMD, &_data);
    }
    return 1;
}


