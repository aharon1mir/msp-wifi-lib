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

uint8_t sndArcv(uint8_t data)
{

}

uint16_t sndArcvW(uint16_t data)
{

}
