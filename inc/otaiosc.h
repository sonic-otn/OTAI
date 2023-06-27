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
 * @file    otaiosc.h
 * @brief   This module defines the OSC for the OTAI
 */

#if !defined (__OTAIOSC_H_)
#define __OTAIOSC_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIOSC OTAI - OSC specific API definitions
 *
 * @{
 */

/**
 * @brief OSC attribute IDs
 */
typedef enum _otai_osc_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_OSC_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_OSC_ATTR_ID = OTAI_OSC_ATTR_START,

    /**
     * @brief Removable
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_REMOVABLE,

    /**
     * @brief Serial no
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_SERIAL_NO,

    /**
     * @brief Equipment failure
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_EQUIPMENT_FAILURE,

    /**
     * @brief Equipment mismatch
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_EQUIPMENT_MISMATCH,

    /**
     * @brief Location
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_LOCATION,

    /**
     * @brief Hardware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_HARDWARE_VERSION,

    /**
     * @brief Admin state
     *
     * @type otai_admin_state_t
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_ADMIN_STATE,

    /**
     * @brief Operational status
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_OPER_STATUS,

    /**
     * @brief Part no
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_PART_NO,

    /**
     * @brief Mfg name
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_MFG_NAME,

    /**
     * @brief Manufacture date
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_MFG_DATE,

    /**
     * @brief Empty
     *
     * @type bool
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_EMPTY,

    /**
     * @brief Software version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_SOFTWARE_VERSION,

    /**
     * @brief Firmware version
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_FIRMWARE_VERSION,

    /**
     * @brief Output frequency
     *
     * @type otai_uint64_t
     * @flags READ_ONLY
     */
    OTAI_OSC_ATTR_OUTPUT_FREQUENCY,

    /**
     * @brief Enabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    OTAI_OSC_ATTR_ENABLED,

    /**
     * @brief Rx low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_OSC_ATTR_RX_LOW_THRESHOLD,

    /**
     * @brief Rx high threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_OSC_ATTR_RX_HIGH_THRESHOLD,

    /**
     * @brief Tx low threshold
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_OSC_ATTR_TX_LOW_THRESHOLD,

    /**
     * @brief End of attributes
     */
    OTAI_OSC_ATTR_END,

    /** Custom range base value */
    OTAI_OSC_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_OSC_ATTR_CUSTOM_RANGE_END

} otai_osc_attr_t;

/**
 * @brief OSC stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_osc_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_OSC_STAT_START,

    /**
     * @brief Input power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_INPUT_POWER = OTAI_OSC_STAT_START,

    /**
     * @brief Temperature
     *
     * @type otai_double_t
     * @precision precision1
     * @iscounter false
     */
    OTAI_OSC_STAT_TEMPERATURE,

    /**
     * @brief Output power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_OUTPUT_POWER,

    /**
     * @brief Input offset primary-line rx
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_INPUT_OFFSET_LINEP_RX,

    /**
     * @brief Output offset primary-line tx
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_OUTPUT_OFFSET_LINEP_TX,

    /**
     * @brief Input offset secondary-line rx
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_INPUT_OFFSET_LINES_RX,

    /**
     * @brief Output offset secondary-line tx
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_OUTPUT_OFFSET_LINES_TX,

    /**
     * @brief Laser bias current
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_LASER_BIAS_CURRENT,

    /**
     * @brief Panel input power primary-line rx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_PANEL_INPUT_POWER_LINEP_RX,

    /**
     * @brief Panel output power primary-line tx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_PANEL_OUTPUT_POWER_LINEP_TX,

    /**
     * @brief Panel input power secondary-line rx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_PANEL_INPUT_POWER_LINES_RX,

    /**
     * @brief Panel output power secondary-line tx
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OSC_STAT_PANEL_OUTPUT_POWER_LINES_TX,

    /**
     * @brief End of statistics
     */
    OTAI_OSC_STAT_END,

} otai_osc_stat_t;

/**
 * @brief Create OSC.
 *
 * Allocates and initializes a OSC.
 *
 * @param[out] osc_id OSC id
 * @param[in] linecard_id Linecard id on which the OSC exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_osc_fn)(
        _Out_ otai_object_id_t *osc_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove OSC
 *
 * @param[in] osc_id OSC id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_osc_fn)(
        _In_ otai_object_id_t osc_id);

/**
 * @brief Set OSC attribute
 *
 * @param[in] osc_id OSC id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_osc_attribute_fn)(
        _In_ otai_object_id_t osc_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get OSC attribute
 *
 * @param[in] osc_id OSC id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_osc_attribute_fn)(
        _In_ otai_object_id_t osc_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get OSC statistics.
 *
 * @param[in] osc_id OSC id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_osc_stats_fn)(
        _In_ otai_object_id_t osc_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get OSC statistics extended.
 *
 * @param[in] osc_id OSC id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_osc_stats_ext_fn)(
        _In_ otai_object_id_t osc_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear OSC statistics counters.
 *
 * @param[in] osc_id OSC id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_osc_stats_fn)(
        _In_ otai_object_id_t osc_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_osc_api_t
{
    otai_create_osc_fn                create_osc;
    otai_remove_osc_fn                remove_osc;
    otai_set_osc_attribute_fn         set_osc_attribute;
    otai_get_osc_attribute_fn         get_osc_attribute;
    otai_get_osc_stats_fn             get_osc_stats;
    otai_get_osc_stats_ext_fn         get_osc_stats_ext;
    otai_clear_osc_stats_fn           clear_osc_stats;
} otai_osc_api_t;

/**
 * @}
 */
#endif /** __OTAIOSC_H_ */
