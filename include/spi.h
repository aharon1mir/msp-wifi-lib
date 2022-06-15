#ifndef __SPI_H
#define __SPI_H

#include <sys/types.h>
#include <msp430g2553.h>
#include <sys/inttypes.h>

/*
 * define basic boolean
 */
#define True 1
#define False 0

/*
 * define basic pinout diagram.
 */
#define WIFI_BOARD_LED BIT9

// The spi pins:
#define SPI_DIR P1DIR
#define SPI_OUT P1OUT
#define SS_SD BIT5
#define SS_WIFI BIT6
#define SLAVEREADY BIT10
#define DATAIN BIT1
#define DATAOUT BIT2
#define SPICLOCK BIT4


/*
 * param: byte / word for send by spi.
 * return: byte / word recv by spi.
 */
uint8_t sndArcv(uint8_t);
uint16_t sndArcvW(uint16_t);

/*
 * action: initate the spi peripheral.
 */
void spi_initate();

/*
 * action: initate the spi for wifi shield.
 */
void Wifi_spi_begin();

/*
 * action: deselect SD or WIFI SPI bus.
 */
void deselect_sd();
void deselect_wifi();


#endif
