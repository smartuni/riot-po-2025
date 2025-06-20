/*
 * Copyright (C) 2025 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    
 * @ingroup     sys
 * @brief       BLE send and receive functionality for the sense mate / gate mate project
 *
 * @{
 *
 * @file
 *
 * @author
 */

#ifndef MATE_BLE_H
#define MATE_BLE_H

#include "../tables/tables.h"

#define BLE_SUCCESS (0)
#define BLE_ERROR (-1)
#define BLE_ERROR_INTERNAL_MESSAGE_BUFFER_FULL (-2)

typedef struct {
   // rssi etc..
   int8_t rssi;
} ble_metadata_t;

/**
 * @brief           Blocking function to receive data over BLE
 * @param[in]       type    Type of data to receive
 * @param[inout]    cbor_packet  Pointer to output cbor packet
 * @param[inout]    metadata  Pointer to output metadata
 * @return          0 on success, negative error code on failure
 */
int ble_receive(CBOR_MESSAGE_TYPE type, cbor_buffer* cbor_packet, ble_metadata_t* metadata);

/**
 * @brief       Function to send data over BLE
 * @param[in]   data    Pointer to the data to send
 * @return      0 on success, negative error code on failure
 */
int ble_send(cbor_buffer* cbor_packet);

/**
 * @brief       Function to initialize the BLE module
 * @return      0 on success, negative error code on failure
 */
int ble_init(void);

#endif /* MATE_BLE_H */
/** @} */
