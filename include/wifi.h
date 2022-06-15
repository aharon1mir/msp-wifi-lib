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



#endif
