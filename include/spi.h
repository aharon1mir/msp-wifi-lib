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
#define SPI_IN  P1IN
#define SS_SD BIT5
#define SS_WIFI BIT6
#define SLAVEREADY BITA
#define DATAIN BIT1
#define DATAOUT BIT2
#define SPICLOCK BIT4

/*
 * define data communication values.
 */
#define DUMMY 0xff
#define START_CMD 0xE0
#define END_CMD 0xEE
#define ERR_CMD 0xEF
#define CMD_FLAG 0x0 
#define REPLY_FLAG 1<<7
#define DATA_FLAG 0x40 
#define WIFI_SPI_ACK 0x01
#define WIFI_SPI_ERR 0xFF
#define TIMEROUT_CHAR 1000
#define MAX_SOCK 0xff
#define SPI_START_CMD_DELAY 12
#define DELAY_SUB 3
#define TIMEOUT_CHAR 1000

/*
 * define wifi 
 */

// Maximum size of a SSID
#define WL_SSID_MAX_LENGTH 32
// Length of passphrase. Valid lengths are 8-63.
#define WL_WPA_KEY_MAX_LENGTH 63
// Length of key in bytes. Valid values are 5 and 13.
#define WL_WEP_KEY_MAX_LENGTH 13
// Size of a MAC-address or BSSID
#define WL_MAC_ADDR_LENGTH 6
// Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH 4
// Maximum size of a SSID list
#define WL_NETWORKS_LIST_MAXNUM	10
// Maxmium number of socket
#define	MAX_SOCK_NUM		4
//Maximum number of attempts to establish wifi connection
#define WL_MAX_ATTEMPT_CONNECTION	10



typedef enum {
		WL_NO_SHIELD = 255,
        WL_IDLE_STATUS = 0,
        WL_NO_SSID_AVAIL,
        WL_SCAN_COMPLETED,
        WL_CONNECTED,
        WL_CONNECT_FAILED,
        WL_CONNECTION_LOST,
        WL_DISCONNECTED
} wl_status_t;

/* Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
        ENC_TYPE_WEP  = 5,
        ENC_TYPE_TKIP = 2,
        ENC_TYPE_CCMP = 4,
        /* ... except these two, 7 and 8 are reserved in 802.11-2007 */
        ENC_TYPE_NONE = 7,
        ENC_TYPE_AUTO = 8
};



/*
 * define 
 */

enum {
	SET_NET_CMD 		= 0x10,
	SET_PASSPHRASE_CMD	= 0x11,
	SET_KEY_CMD	        = 0x12,
	TEST_CMD	        = 0x13,

	GET_CONN_STATUS_CMD	= 0x20,
	GET_IPADDR_CMD		= 0x21,
	GET_MACADDR_CMD		= 0x22,
	GET_CURR_SSID_CMD	= 0x23,
	GET_CURR_BSSID_CMD	= 0x24,
	GET_CURR_RSSI_CMD	= 0x25,
	GET_CURR_ENCT_CMD	= 0x26,
	SCAN_NETWORKS		= 0x27,
	START_SERVER_TCP_CMD= 0x28,
	GET_STATE_TCP_CMD   = 0x29,
	DATA_SENT_TCP_CMD	= 0x2A,
    AVAIL_DATA_TCP_CMD	= 0x2B,
    GET_DATA_TCP_CMD	= 0x2C,
    START_CLIENT_TCP_CMD= 0x2D,
    STOP_CLIENT_TCP_CMD = 0x2E,
    GET_CLIENT_STATE_TCP_CMD= 0x2F,
    DISCONNECT_CMD		= 0x30,
	GET_IDX_SSID_CMD	= 0x31,
	GET_IDX_RSSI_CMD	= 0x32,
	GET_IDX_ENCT_CMD	= 0x33,
	REQ_HOST_BY_NAME_CMD= 0x34,
	GET_HOST_BY_NAME_CMD= 0x35,
	START_SCAN_NETWORKS	= 0x36,
	GET_FW_VERSION_CMD	= 0x37,

    // All command with DATA_FLAG 0x40 send a 16bit Len

	SEND_DATA_TCP_CMD		= 0x44,
    GET_DATABUF_TCP_CMD		= 0x45,
};


enum wl_tcp_state {
  CLOSED      = 0,
  LISTEN      = 1,
  SYN_SENT    = 2,
  SYN_RCVD    = 3,
  ESTABLISHED = 4,
  FIN_WAIT_1  = 5,
  FIN_WAIT_2  = 6,
  CLOSE_WAIT  = 7,
  CLOSING     = 8,
  LAST_ACK    = 9,
  TIME_WAIT   = 10
};


enum numParams{
    PARAM_NUMS_0,
    PARAM_NUMS_1,
    PARAM_NUMS_2,
    PARAM_NUMS_3,
    PARAM_NUMS_4,
    PARAM_NUMS_5,
    MAX_PARAM_NUMS
};

#define MAX_PARAMS MAX_PARAM_NUMS-1
#define PARAM_LEN_SIZE 1

typedef struct  __attribute__((__packed__))
{
	uint8_t     paramLen;
	char*	    param;
}tParam;

typedef struct  __attribute__((__packed__))
{
	uint16_t     dataLen;
	char*	     data;
}tDataParam;


typedef struct  __attribute__((__packed__))
{
	unsigned char	cmd;
	unsigned char	tcmd;
	unsigned char	nParam;
	tParam	params[MAX_PARAMS];
}tSpiMsg;

typedef struct  __attribute__((__packed__))
{
	unsigned char	cmd;
	unsigned char	tcmd;
	unsigned char	nParam;
	tDataParam		params[MAX_PARAMS];
}tSpiMsgData;


typedef struct  __attribute__((__packed__))
{
	unsigned char	cmd;
	unsigned char	tcmd;
	//unsigned char	totLen;
	unsigned char	nParam;
}tSpiHdr;

typedef struct  __attribute__((__packed__))
{
	uint8_t     paramLen;
	uint32_t	param;
}tLongParam;

typedef struct  __attribute__((__packed__))
{
	uint8_t     paramLen;
	uint16_t	param;
}tIntParam;

typedef struct  __attribute__((__packed__))
{
	uint8_t     paramLen;
	uint8_t	param;
}tByteParam;



#define send(data) sndArcv(data)
#define sendW(data) sndArcvW(data)


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


int getSlaveSign();

void waitForSlaveSign();
void waitForSlaveReady();

void delay(unsigned int);

void sendCmd(uint8_t, uint8_t);
void sendParam_W(uint16_t, uint8_t);
void sendParamW(uint16_t)
void sendParamC(uint8_t);
void sendBuffer(uint8_t*, uint16_t, uint8_t);
uint16_t readParamW(uint16_t*);
uint8_t readParamC(uint8_t*);
int waitSpiChar(unsigned char);

/*
 * define some macros
 */
#define WAIT_START_CMD(x) waitSpiChar(START_CMD)

#define IF_CHECK_START_CMD(x)                      \
    if (!WAIT_START_CMD(_data))                 \
    {                                           \
        return 0;                               \
    }else                                       \

#define CHECK_DATA(check, x)                   \
        if (!readAndCheckChar(check, &x))   \
        {                                               \
            return 0;                                   \
        }else                                           \

#define WAIT_FOR_SLAVE_SELECT()	 \
	waitForSlaveReady(); \
	spiSlaveSelect();

#define NO_LAST_PARAM   0
#define LAST_PARAM 1




int waitResponseCmd(uint8_t, uint8_t, uint8_t*, uint8_t*);
int readAndCheckChar(char, char*);
int waitResponseDataW(uint8_t, uint8_t*, uint16_t*);
int waitResponseDataC(uint8_t, uint8_t*, uint8_t*);
int waitResponseParams(uint8_t, uint8_t, tParam*);
int waitResponse(uint8_t, uint8_t*, uint8_t**, uint8_t);
#endif
