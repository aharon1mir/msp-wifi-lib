#include <wifi.h>
#include <spi.h>

// tmp main
int main()
{
    WDTCTL = WDTPW + WDTHOLD; // stop watchdog
    while (1)
    {
        asm ("nop");
    }
}

// Array of data to cache the information related to the networks discovered
char 	_networkSsid[][WL_SSID_MAX_LENGTH] = {{"1"},{"2"},{"3"},{"4"},{"5"}};
int32_t _networkRssi[WL_NETWORKS_LIST_MAXNUM] = { 0 };
uint8_t _networkEncr[WL_NETWORKS_LIST_MAXNUM] = { 0 };

// Cached values of retrieved data
char 	_ssid[] = {0};
uint8_t	_bssid[] = {0};
uint8_t _mac[] = {0};
uint8_t _localIp[] = {0};
uint8_t _subnetMask[] = {0};
uint8_t _gatewayIp[] = {0};
// Firmware version
char    fwVersion[] = {0};


void getNetworkData(uint8_t *ip, uint8_t *mask, uint8_t *gwip)
{
    tParam params[PARAM_NUMS_3] = { {0, (char*)ip}, {0, (char*)mask}, {0, (char*)gwip}};

    WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_IPADDR_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY;
    sendParam(&_dummy, sizeof(_dummy), LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    waitResponseParams(GET_IPADDR_CMD, PARAM_NUMS_3, params);

    spiSlaveDeselect();
}

// Public Methods

void wifi_init()
{
}

int8_t wifiSetNetwork(char* ssid, uint8_t ssid_len)
{
	WAIT_FOR_SLAVE_SELECT();
    // Send Command
    sendCmd(SET_NET_CMD, PARAM_NUMS_1);
    sendParam((uint8_t*)ssid, ssid_len, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = 0;
    uint8_t _dataLen = 0;
    if (!waitResponseCmd(SET_NET_CMD, PARAM_NUMS_1, &_data, &_dataLen))
    {
        _data = WL_FAILURE;
    }
    spiSlaveDeselect();

    return(_data == WIFI_SPI_ACK) ? WL_SUCCESS : WL_FAILURE;
}

int8_t wifiSetPassphrase(char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len)
{
	WAIT_FOR_SLAVE_SELECT();
    // Send Command
    sendCmd(SET_PASSPHRASE_CMD, PARAM_NUMS_2);
    sendParam((uint8_t*)ssid, ssid_len, NO_LAST_PARAM);
    sendParam((uint8_t*)passphrase, len, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = 0;
    uint8_t _dataLen = 0;
    if (!waitResponseCmd(SET_PASSPHRASE_CMD, PARAM_NUMS_1, &_data, &_dataLen))
    {
        _data = WL_FAILURE;
    }
    spiSlaveDeselect();
    return _data;
}


int8_t wifiSetKey(char* ssid, uint8_t ssid_len, uint8_t key_idx, const void *key, const uint8_t len)
{
	WAIT_FOR_SLAVE_SELECT();
    // Send Command
    sendCmd(SET_KEY_CMD, PARAM_NUMS_3);
    sendParam((uint8_t*)ssid, ssid_len, NO_LAST_PARAM);
    sendParam(&key_idx, KEY_IDX_LEN, NO_LAST_PARAM);
    sendParam((uint8_t*)key, len, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = 0;
    uint8_t _dataLen = 0;
    if (!waitResponseCmd(SET_KEY_CMD, PARAM_NUMS_1, &_data, &_dataLen))
    {
        _data = WL_FAILURE;
    }
    spiSlaveDeselect();
    return _data;
}

int8_t disconnect()
{
	WAIT_FOR_SLAVE_SELECT();
    // Send Command
    sendCmd(DISCONNECT_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY_DATA;
    sendParam(&_dummy, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = 0;
    uint8_t _dataLen = 0;
    int8_t result = waitResponseCmd(DISCONNECT_CMD, PARAM_NUMS_1, &_data, &_dataLen);

    spiSlaveDeselect();

    return result;
}

uint8_t getConnectionStatus()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_CONN_STATUS_CMD, PARAM_NUMS_0);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = -1;
    uint8_t _dataLen = 0;
    waitResponseCmd(GET_CONN_STATUS_CMD, PARAM_NUMS_1, &_data, &_dataLen);

    spiSlaveDeselect();

    return _data;
}

uint8_t* getMacAddress()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_MACADDR_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY_DATA;
    sendParam(&_dummy, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _dataLen = 0;
    waitResponseCmd(GET_MACADDR_CMD, PARAM_NUMS_1, _mac, &_dataLen);

    spiSlaveDeselect();

    return _mac;
}

void getIpAddress(IPAddress& ip)
{
	getNetworkData(_localIp, _subnetMask, _gatewayIp);
	ip = _localIp;
}

 void  getSubnetMask(IPAddress& mask)
 {
	getNetworkData(_localIp, _subnetMask, _gatewayIp);
	mask = _subnetMask;
 }

 void getGatewayIP(IPAddress& ip)
 {
	getNetworkData(_localIp, _subnetMask, _gatewayIp);
	ip = _gatewayIp;
 }

char* getCurrentSSID()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_CURR_SSID_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY_DATA;
    sendParam(&_dummy, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _dataLen = 0;
    waitResponseCmd(GET_CURR_SSID_CMD, PARAM_NUMS_1, (uint8_t*)_ssid, &_dataLen);

    spiSlaveDeselect();

    return _ssid;
}

uint8_t* getCurrentBSSID()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_CURR_BSSID_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY_DATA;
    sendParam(&_dummy, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _dataLen = 0;
    waitResponseCmd(GET_CURR_BSSID_CMD, PARAM_NUMS_1, _bssid, &_dataLen);

    spiSlaveDeselect();

    return _bssid;
}

int32_t getCurrentRSSI()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_CURR_RSSI_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY_DATA;
    sendParam(&_dummy, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _dataLen = 0;
    int32_t rssi = 0;
    waitResponseCmd(GET_CURR_RSSI_CMD, PARAM_NUMS_1, (uint8_t*)&rssi, &_dataLen);

    spiSlaveDeselect();

    return rssi;
}

uint8_t getCurrentEncryptionType()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_CURR_ENCT_CMD, PARAM_NUMS_1);

    uint8_t _dummy = DUMMY_DATA;
    sendParam(&_dummy, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t dataLen = 0;
    uint8_t encType = 0;
    waitResponseCmd(GET_CURR_ENCT_CMD, PARAM_NUMS_1, (uint8_t*)&encType, &dataLen);

    spiSlaveDeselect();

    return encType;
}

int8_t startScanNetworks()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(START_SCAN_NETWORKS, PARAM_NUMS_0);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = 0;
    uint8_t _dataLen = 0;

    if (!waitResponseCmd(START_SCAN_NETWORKS, PARAM_NUMS_1, &_data, &_dataLen))
     {
         _data = WL_FAILURE;
     }

    spiSlaveDeselect();

    return (_data == WL_FAILURE)? _data : WL_SUCCESS;
}


uint8_t WiFiDrv::getScanNetworks()
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(SCAN_NETWORKS, PARAM_NUMS_0);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t ssidListNum = 0;
    waitResponse(SCAN_NETWORKS, &ssidListNum, (uint8_t**)_networkSsid, WL_NETWORKS_LIST_MAXNUM);

    spiSlaveDeselect();

    return ssidListNum;
}

char* WiFiDrv::getSSIDNetoworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;

	return _networkSsid[networkItem];
}

uint8_t WiFiDrv::getEncTypeNetowrks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;

	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_IDX_ENCT_CMD, PARAM_NUMS_1);

    sendParam(&networkItem, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t dataLen = 0;
    uint8_t encType = 0;
    waitResponseCmd(GET_IDX_ENCT_CMD, PARAM_NUMS_1, (uint8_t*)&encType, &dataLen);

    spiSlaveDeselect();

    return encType;
}

int32_t WiFiDrv::getRSSINetoworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;
	int32_t	networkRssi = 0;

	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(GET_IDX_RSSI_CMD, PARAM_NUMS_1);

    sendParam(&networkItem, 1, LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t dataLen = 0;
    waitResponseCmd(GET_IDX_RSSI_CMD, PARAM_NUMS_1, (uint8_t*)&networkRssi, &dataLen);

    spiSlaveDeselect();

	return networkRssi;
}

uint8_t WiFiDrv::reqHostByName(const char* aHostname)
{
	WAIT_FOR_SLAVE_SELECT();

    // Send Command
    sendCmd(REQ_HOST_BY_NAME_CMD, PARAM_NUMS_1);
    sendParam((uint8_t*)aHostname, strlen(aHostname), LAST_PARAM);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _data = 0;
    uint8_t _dataLen = 0;
    uint8_t result = waitResponseCmd(REQ_HOST_BY_NAME_CMD, PARAM_NUMS_1, &_data, &_dataLen);

    spiSlaveDeselect();

    return result;
}

int WiFiDrv::getHostByName(IPAddress& aResult)
{
	uint8_t  _ipAddr[WL_IPV4_LENGTH];
	IPAddress dummy(0xFF,0xFF,0xFF,0xFF);
	int result = 0;

	WAIT_FOR_SLAVE_SELECT();
    // Send Command
    sendCmd(GET_HOST_BY_NAME_CMD, PARAM_NUMS_0);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _dataLen = 0;
    if (!waitResponseCmd(GET_HOST_BY_NAME_CMD, PARAM_NUMS_1, _ipAddr, &_dataLen))
    {
    }else{
    	aResult = _ipAddr;
    	result = (aResult != dummy);
    }
    spiSlaveDeselect();
    return result;
}

int WiFiDrv::getHostByName(const char* aHostname, IPAddress& aResult)
{
	uint8_t retry = 10;
	if (reqHostByName(aHostname))
	{
		while(!getHostByName(aResult) && --retry > 0)
		{
			delay(1000);
		}
	}else{
		return 0;
	}
	return (retry>0);
}

char*  WiFiDrv::getFwVersion()
{
	WAIT_FOR_SLAVE_SELECT();
    // Send Command
    sendCmd(GET_FW_VERSION_CMD, PARAM_NUMS_0);

    //Wait the reply elaboration
    waitForSlaveReady();

    // Wait for reply
    uint8_t _dataLen = 0;
    spiSlaveDeselect();
    return fwVersion;
}
