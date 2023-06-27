/**
 * Copyright (c) 2021 Alibaba Group.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 * @file    otaiport.h
 * @brief   This module defines the port for the OTAI
 */

#if !defined (__OTAIPORT_H_)
#define __OTAIPORT_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIPORT OTAI - Port specific API definitions
 *
 * @{
 */
typedef enum _otai_port_optical_port_type_t
{
    OTAI_PORT_OPTICAL_PORT_TYPE_EGRESS,
    OTAI_PORT_OPTICAL_PORT_TYPE_ADD,
    OTAI_PORT_OPTICAL_PORT_TYPE_DROP,
    OTAI_PORT_OPTICAL_PORT_TYPE_MONITOR,
    OTAI_PORT_OPTICAL_PORT_TYPE_TERMINAL_CLIENT,
    OTAI_PORT_OPTICAL_PORT_TYPE_TERMINAL_LINE,
} otai_port_optical_port_type_t;

/**
 * @brief Port attribute IDs
 */
typedef enum _otai_port_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_PORT_ATTR_START,

    /**
     * @brief Client port or line port.
     *
     * @type otai_port_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_PORT_ATTR_PORT_TYPE = OTAI_PORT_ATTR_START,

    /**
     * @brief Port ID.
     *
     * @type otai_uint32_t
     * @flags CREATE_ONLY
     */
    OTAI_PORT_ATTR_PORT_ID,

    /**
     * @brief Operational status
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_OPER_STATUS,

    /**
     * @brief Admin state
     *
     * @type otai_admin_state_t
     * @flags CREATE_AND_SET
     */
    OTAI_PORT_ATTR_ADMIN_STATE,

    /**
     * @brief Rx cd range
     *
     * @type otai_s32_range_t
     * @flags CREATE_AND_SET
     */
    OTAI_PORT_ATTR_RX_CD_RANGE,

    /**
     * @brief Roll off
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     */
    OTAI_PORT_ATTR_ROLL_OFF,

    /**
     * @brief Led mode
     *
     * @type otai_led_mode_t
     * @flags CREATE_AND_SET
     * @isrecoverable false
     */
    OTAI_PORT_ATTR_LED_MODE,

    /**
     * @brief Led flash interval
     *
     * @type otai_uint16_t
     * @flags CREATE_AND_SET
     * @isrecoverable false
     */
    OTAI_PORT_ATTR_LED_FLASH_INTERVAL,

    /**
     * @brief Led color
     *
     * @type otai_led_color_t
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_LED_COLOR,

    /**
     * @brief Los threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_PORT_ATTR_LOS_THRESHOLD,

    /**
     * @brief Optical port type
     *
     * @type otai_port_optical_port_type_t
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_OPTICAL_PORT_TYPE,

    /**
     * @brief Empty
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_EMPTY,

    /**
     * @brief Removable
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_REMOVABLE,

    /**
     * @brief Equipment failure
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_EQUIPMENT_FAILURE,

    /**
     * @brief Equipment mismatch
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_EQUIPMENT_MISMATCH,

    /**
     * @brief Low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_PORT_ATTR_LOW_THRESHOLD,

    /**
     * @brief High threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_PORT_ATTR_HIGH_THRESHOLD,

    /**
     * @brief Input offset
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_INPUT_OFFSET,

    /**
     * @brief Output offset
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_PORT_ATTR_OUTPUT_OFFSET,

    /**
     * @brief Led name
     *
     * @type char
     * @flags CREATE_AND_SET
     * @isrecoverable false
     */
    OTAI_PORT_ATTR_LED_NAME,

    /**
     * @brief End of attributes
     */
    OTAI_PORT_ATTR_END,

    /** Custom range base value */
    OTAI_PORT_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_PORT_ATTR_CUSTOM_RANGE_END

} otai_port_attr_t;

/**
 * @brief Port stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_port_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_PORT_STAT_START,

    /**
     * @brief Input power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_PORT_STAT_INPUT_POWER = OTAI_PORT_STAT_START,

    /**
     * @brief Output power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_PORT_STAT_OUTPUT_POWER,

    /**
     * @brief OSC input power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_PORT_STAT_OSC_INPUT_POWER,

    /**
     * @brief OSC output power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_PORT_STAT_OSC_OUTPUT_POWER,

    /**
     * @brief End of statistics
     */
    OTAI_PORT_STAT_END,

} otai_port_stat_t;

/**
 * @brief Create port.
 *
 * Allocates and initializes a port.
 *
 * @param[out] port_id Port id
 * @param[in] linecard_id Linecard id on which the port exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_port_fn)(
        _Out_ otai_object_id_t *port_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove port
 *
 * @param[in] port_id Port id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_port_fn)(
        _In_ otai_object_id_t port_id);

/**
 * @brief Set port attribute
 *
 * @param[in] port_id Port id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_port_attribute_fn)(
        _In_ otai_object_id_t port_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get port attribute
 *
 * @param[in] port_id Port id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_port_attribute_fn)(
        _In_ otai_object_id_t port_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get port statistics.
 *
 * @param[in] port_id Port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_port_stats_fn)(
        _In_ otai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get port statistics extended.
 *
 * @param[in] port_id Port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_port_stats_ext_fn)(
        _In_ otai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear port statistics.
 *
 * @param[in] port_id Port id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_port_stats_fn)(
        _In_ otai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Port methods table retrieved with otai_api_query()
 */
typedef struct _otai_port_api_t
{
    otai_create_port_fn          create_port;
    otai_remove_port_fn          remove_port;
    otai_set_port_attribute_fn   set_port_attribute;
    otai_get_port_attribute_fn   get_port_attribute;
    otai_get_port_stats_fn       get_port_stats;
    otai_get_port_stats_ext_fn   get_port_stats_ext;
    otai_clear_port_stats_fn     clear_port_stats;
} otai_port_api_t;

/**
 * @}
 */
#endif /** __OTAIPORT_H_ */
