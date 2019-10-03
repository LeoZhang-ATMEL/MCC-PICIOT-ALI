/*
    \file   mqtt_packetParameters.c

    \brief  MQTT Packet Parameters source file.

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

// ToDo This file needs to be renamed as app_mqttClient.c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../mqtt/mqtt_core/mqtt_core.h"
#include "mqtt_packetPopulate.h"
#include "../../config/IoT_Sensor_Node_config.h"
#include "../../debug_print.h"

#define MQTT_CID_LENGTH                                                                                                \
	64 + 60                                                                                                            \
	    + 1 // clientId(64 bytes)  +  |securemode=3,signmethod=hmacsha256,timestamp=1539939251799|(60 bytes)  +
	        // '\0'(1byte)
#define MQTT_TOPIC_LENGTH 80 // 38

char mqttUsername[MAX_DEVICE_NAME_LENGTH + 1 + MAX_PRODUCT_KEY_LENGTH
                  + 1]; // deviceName(30bytes)  +  &(1byte)  +  productKey(11bytes)  +  '\0'(1byte)
char mqttPassword[32 * 2
                  + 1]; // HMAC-SHA256 result is 256bits. 256bits/8 = 32bytes, convert to hex string(*2)  +  '\0'(1byte)
char mqttCid[MQTT_CID_LENGTH];
char mqttPublishTopic[MQTT_TOPIC_LENGTH];
char mqttSubscribeTopic[TOPIC_SIZE];

void MQTT_CLIENT_publish(uint8_t *data, uint16_t len)
{
	 mqttPublishPacket cloudPublishPacket;
    
    // Fixed header
    cloudPublishPacket.publishHeaderFlags.duplicate = 0;
    cloudPublishPacket.publishHeaderFlags.qos = 0;
    cloudPublishPacket.publishHeaderFlags.retain = 0;
    
    // Variable header
    cloudPublishPacket.topic = (uint8_t*)mqttPublishTopic;
    
    // Payload
    cloudPublishPacket.payload = data;
    // ToDo Check whether sizeof can be used for integers and strings
    cloudPublishPacket.payloadLength = len;
    
    if(MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
    {
        debug_printError("MQTT: Connection lost PUBLISH failed");
    }
}

void MQTT_CLIENT_receive(uint8_t *data, uint8_t len)
{
    MQTT_GetReceivedData(data, len);
}

void MQTT_CLIENT_connect(void)
{
	mqttConnectPacket cloudConnectPacket;

	memset(&cloudConnectPacket, 0, sizeof(mqttConnectPacket));

	cloudConnectPacket.connectVariableHeader.connectFlagsByte.All = 0x43;
	cloudConnectPacket.connectVariableHeader.keepAliveTimer       = 120;
	cloudConnectPacket.clientID                                   = (uint8_t *)mqttCid;
	cloudConnectPacket.password                                   = (uint8_t *)mqttPassword;
	cloudConnectPacket.passwordLength                             = strlen(mqttPassword);
	cloudConnectPacket.username                                   = (uint8_t *)mqttUsername;
	cloudConnectPacket.usernameLength                             = strlen(mqttUsername);

	MQTT_CreateConnectPacket(&cloudConnectPacket);
}
