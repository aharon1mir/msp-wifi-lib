#ifndef __WIFI_H
#define __WIFI_H

#include <msp430g2553.h>
#include <spi.h>

int WIFI_STATUS; // global status for the wifi shield.

/*
 * param: 
 * buffer pointer (at least 16 bytes).
 *
 * action:
 * Setting in the buffer the firmware version of the wifi shield.
 */
void FirmwareVersion(char *); 

/*
 * param:
 *
 * return:
 * if connection to the given ssid with the given password success return True, else False.
 *
 */
int Wifi_begin(char *, char *);

// Key index length
#define KEY_IDX_LEN     1
// 5 secs of delay to have the connection established
#define WL_DELAY_START_CONNECTION 5000
// firmware version string length
#define WL_FW_VER_LENGTH 6


void getNetworkData(uint8_t*, uint8_t*, uint8_t*);

void wifi_init();

#endif
