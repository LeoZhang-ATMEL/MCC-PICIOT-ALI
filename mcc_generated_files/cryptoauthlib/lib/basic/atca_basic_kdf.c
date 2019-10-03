/**
 * \file
 * \brief CryptoAuthLib Basic API methods for KDF command.
 *
 * The KDF command implements one of a number of Key Derivation Functions (KDF).
 * Generally this function combines a source key with an input string and
 * creates a result key/digest/array. Three algorithms are currently supported:
 * PRF, HKDF and AES.
 *
 * \note List of devices that support this command - ATECC608A. Refer to device
 *       datasheet for full details.
 *
 * \copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \page License
 * 
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE
 * OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
 * FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL
 * LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED
 * THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR
 * THIS SOFTWARE.
 */
#include <string.h>
#include "atca_basic.h"
#include "../atca_execution.h"

/** \brief Executes the KDF command, which derives a new key in PRF, AES, or
 *          HKDF modes.
 *
 * Generally this function combines a source key with an input string and
 * creates a result key/digest/array.
 *
 * \param[in]  mode       Mode determines KDF algorithm (PRF,AES,HKDF), source
 *                        key location, and target key locations.
 * \param[in]  key_id     Source and target key slots if locations are in the
 *                        EEPROM. Source key slot is the LSB and target key
 *                        slot is the MSB.
 * \param[in]  details    Further information about the computation, depending
 *                        on the algorithm (4 bytes).
 * \param[in]  message    Input value from system (up to 128 bytes). Actual size
 *                        of message is 16 bytes for AES algorithm or is encoded
 *                        in the MSB of the details parameter for other
 *                        algorithms.
 * \param[out] out_data   Output of the KDF function is returned here. If the
 *                        result remains in the device, this can be NULL.
 * \param[out] out_nonce  If the output is encrypted, a 32 byte random nonce
 *                        generated by the device is returned here. If output
 *                        encryption is not used, this can be NULL.
 *
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS atcab_kdf(uint8_t mode, uint16_t key_id, const uint32_t details, const uint8_t* message, uint8_t* out_data, uint8_t* out_nonce)
{
    ATCAPacket packet;
    ATCACommand ca_cmd = _gDevice->mCommands;
    ATCA_STATUS status = ATCA_GEN_FAIL;
    uint16_t out_data_size = 0;

    do
    {
        if (message == NULL)
        {
            return ATCA_BAD_PARAM;
        }

        // Build the KDF command
        packet.param1 = mode;
        packet.param2 = key_id;

        // Add details parameter
        packet.data[0] = details;
        packet.data[1] = details >> 8;
        packet.data[2] = details >> 16;
        packet.data[3] = details >> 24;

        // Add input message
        if ((mode & KDF_MODE_ALG_MASK) == KDF_MODE_ALG_AES)
        {
            // AES algorithm has a fixed message size
            memcpy(&packet.data[KDF_DETAILS_SIZE], message, AES_DATA_SIZE);
        }
        else
        {
            // All other algorithms encode message size in the last byte of details
            memcpy(&packet.data[KDF_DETAILS_SIZE], message, packet.data[3]);
        }

        // Build command
        if ((status = atKDF(ca_cmd, &packet)) != ATCA_SUCCESS)
        {
            break;
        }

        // Run command
        if ((status = atca_execute_command(&packet, _gDevice)) != ATCA_SUCCESS)
        {
            break;
        }

        if (((mode & KDF_MODE_ALG_MASK) == KDF_MODE_ALG_PRF) && (details & KDF_DETAILS_PRF_TARGET_LEN_64))
        {
            out_data_size = 64;
        }
        else
        {
            out_data_size = 32;
        }

        // Return OutData if possible
        if (out_data != NULL && packet.data[ATCA_COUNT_IDX] >= (ATCA_PACKET_OVERHEAD + out_data_size))
        {
            memcpy(out_data, &packet.data[ATCA_RSP_DATA_IDX], out_data_size);
        }

        // return OutNonce if possible
        if (out_nonce != NULL && packet.data[ATCA_COUNT_IDX] >= (ATCA_PACKET_OVERHEAD + out_data_size + 32))
        {
            memcpy(out_nonce, &packet.data[ATCA_RSP_DATA_IDX + out_data_size], 32);
        }
    }
    while (false);

    return status;
}
