/*
    \file   credentials_storage.c

    \brief  Credential Storage source file.

    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "credentials_storage.h"
#include "../config/IoT_Sensor_Node_config.h"
#include "../cloud/cloud_service.h"
#include "../cloud/crypto_client/crypto_client.h"

#define EEPROM_SSID 0
#define EEPROM_PSW EEPROM_SSID + MAX_WIFI_CREDENTIALS_LENGTH
#define EEPROM_SEC EEPROM_PSW + MAX_WIFI_CREDENTIALS_LENGTH
#define EEPROM_DBG EEPROM_SEC + 1
#define EEPROM_PRODUCT_KEY EEPROM_DBG + 1
#define EEPROM_DEVICE_NAME EEPROM_PRODUCT_KEY + MAX_PRODUCT_KEY_LENGTH + 1

char ssid[MAX_WIFI_CREDENTIALS_LENGTH];
char pass[MAX_WIFI_CREDENTIALS_LENGTH];
char authType[2];

void CREDENTIALS_STORAGE_clearWifiCredentials(void)
{
	memset(ssid, 0, sizeof(ssid));
	memset(pass, 0, sizeof(pass));	
	memset(authType, 0 ,sizeof(authType));
}

/**
 * \brief Read product key and device name from EEPROM
 *
 * \param productKey		buffer with product key
 * \param deviceName		buffer with device name
 */
void DEVICE_INFORMATION_read(char *productKey, char *deviceName)
{
	productKey[0] = 0xFF;
	deviceName[0] = 0xFF;
#if 0 /* TODO */
	uint8_t  i    = MAX_PRODUCT_KEY_LENGTH + 1;
	uint8_t *addr = (uint8_t *)EEPROM_PRODUCT_KEY;

	while (i--) {
		*productKey++ = eeprom_read_byte(addr++);
	}

	i    = MAX_DEVICE_NAME_LENGTH + 1;
	addr = (uint8_t *)EEPROM_DEVICE_NAME;

	while (i--) {
		*deviceName++ = eeprom_read_byte(addr++);
	}
#endif
}

/**
 * \brief Store product key and device name to EEPROM, store device secret to ECC608
 *
 * \param productKey		buffer with product key
 * \param deviceName		buffer with device name
 * \param deviceSecret		buffer with device secret
 */
// void DEVICE_save(char *productKey, char *deviceName, char *deviceSecret)
//{
// uint8_t i = strlen(productKey) + 1;
// uint8_t *addr = (uint8_t *)EEPROM_PRODUCT_KEY;
//
//
// while (i--)
//{
// eeprom_write_byte(addr++, (uint8_t)*productKey++);
//}
//
// i = strlen(deviceName) + 1;
// addr = (uint8_t *)EEPROM_DEVICE_NAME;
//
//
// while (i--)
//{
// eeprom_write_byte(addr++, (uint8_t)*deviceName++);
//}
//
// if(deviceSecret != NULL)
//{
// CRYPTO_CLIENT_writeDeviceSecret(deviceSecret);
//}
//}

/**
 * \brief Store product key and device name to EEPROM
 *
 * \param productKey		buffer with product key
 * \param deviceName		buffer with device name
 */
void DEVICE_INFORMATION_save(char *productKey, char *deviceName)
{
#if 0 /* TODO */
	uint8_t  i    = strlen(productKey) + 1;
	uint8_t *addr = (uint8_t *)EEPROM_PRODUCT_KEY;

	while (i--) {
		eeprom_write_byte(addr++, (uint8_t)*productKey++);
	}

	i    = strlen(deviceName) + 1;
	addr = (uint8_t *)EEPROM_DEVICE_NAME;

	while (i--) {
		eeprom_write_byte(addr++, (uint8_t)*deviceName++);
	}
#endif
}

/**
 * \brief Store device secret to ECC608
 *
 * \param deviceSecret		buffer with device secret
 */
void DEVICE_SECRET_save(char *deviceSecret)
{
	CRYPTO_CLIENT_writeDeviceSecret(deviceSecret);
}
