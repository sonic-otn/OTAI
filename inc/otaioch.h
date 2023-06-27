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
 * @file    otaioch.h
 * @brief   This module defines the optical channel for the OTAI
 */

#if !defined (__OTAIOCH_H_)
#define __OTAIOCH_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIOCH OTAI - Optical channel specific API definitions
 *
 * @{
 */

/**
 * @brief Optical channel attribute IDs
 */
typedef enum _otai_och_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_OCH_ATTR_START,

    /**
     * @brief Client port or line port.
     *
     * @type otai_port_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_OCH_ATTR_PORT_TYPE = OTAI_OCH_ATTR_START,

    /**
     * @brief Port ID.
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_OCH_ATTR_PORT_ID,

    /**
     * @brief Type for optical spectrum frequency values
     *
     * @type otai_uint64_t
     * @flags CREATE_AND_SET
     */
    OTAI_OCH_ATTR_FREQUENCY,

    /**
     * @brief Target output optical power level of the optical channel,
     * expressed in increments of 0.01 dBm
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_OCH_ATTR_TARGET_OUTPUT_POWER,

    /**
     * @brief Vendor-specific mode identifier -- sets the operational
     * mode for the channel. The specified operational mode must
     * exist in the list of supported operational modes supplied
     * by the device
     *
     * @type char
     * @flags CREATE_AND_SET
     */
    OTAI_OCH_ATTR_OPERATIONAL_MODE,

    /**
     * @brief The operational state of the och
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_OCH_ATTR_OPER_STATUS,

    /**
     * @brief End of attributes
     */
    OTAI_OCH_ATTR_END,

    /** Custom range base value */
    OTAI_OCH_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_OCH_ATTR_CUSTOM_RANGE_END

} otai_och_attr_t;

/**
 * @brief Optical channel counter IDs in otai_get_och_stats() call
 *
 * @flags Contains flags
 */
typedef enum _otai_och_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_OCH_STAT_START,

    /**
     * @brief Group delay
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_GROUP_DELAY = OTAI_OCH_STAT_START,

    /**
     * @brief Input power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_INPUT_POWER,

    /**
     * @brief Chromatic dispersion
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_CHROMATIC_DISPERSION,

    /**
     * @brief Output power
     *
     * @type otai_double_t
     * @precision precision2
     * @unit dBm
     * @iscounter false
     */
    OTAI_OCH_STAT_OUTPUT_POWER,

    /**
     * @brief Laser bias current
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_LASER_BIAS_CURRENT,

    /**
     * @brief Second ORDER polarization mode dispersion
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_SECOND_ORDER_POLARIZATION_MODE_DISPERSION,

    /**
     * @brief Polarization mode dispersion
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_POLARIZATION_MODE_DISPERSION,

    /**
     * @brief OSNR
     *
     * @type otai_double_t
     * @unit dB
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_OSNR,

    /**
     * @brief SOP Vector S1
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_SOP_VECTOR_S1,

    /**
     * @brief SOP Vector S2
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_SOP_VECTOR_S2,

    /**
     * @brief SOP Vector S3
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_SOP_VECTOR_S3,

    /**
     * @brief SOP change rate
     *
     * @type otai_uint32_t
     * @iscounter false
     */
    OTAI_OCH_STAT_SOP_CHANGE_RATE,

    /**
     * @brief Tx laser age
     *
     * @type otai_uint32_t
     * @iscounter false
     */
    OTAI_OCH_STAT_TX_LASER_AGE,

    /**
     * @brief Polarization dependent loss
     *
     * @type otai_double_t
     * @unit dB
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_POLARIZATION_DEPENDENT_LOSS,

    /**
     * @brief Input signal power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_INPUT_SIGNAL_POWER,

    /**
     * @brief EDFA bias current
     *
     * @type otai_double_t
     * @precision precision2
     * @iscounter false
     */
    OTAI_OCH_STAT_EDFA_BIAS_CURRENT,

    /**
     * @brief Actual received frequency offset. unit: MHz
     *
     * @type otai_int32_t
     * @iscounter false
     */
    OTAI_OCH_STAT_ACTUAL_FREQUENCY_OFFSET,

    /**
     * @brief End of statistics
     */
    OTAI_OCH_STAT_END,

} otai_och_stat_t;

/**
 * @brief Create optical channel.
 *
 * Allocates and initializes a optical channel.
 *
 * @param[out] och_id Optical channel id
 * @param[in] linecard_id Linecard id on which the optical channel exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_och_fn)(
        _Out_ otai_object_id_t *och_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove optical channel
 *
 * @param[in] och_id Optical channel id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_och_fn)(
        _In_ otai_object_id_t och_id);

/**
 * @brief Set optical channel attribute
 *
 * @param[in] och_id Optical channel id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_och_attribute_fn)(
        _In_ otai_object_id_t och_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get optical channel attribute
 *
 * @param[in] och_id Optical channel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_och_attribute_fn)(
        _In_ otai_object_id_t och_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get optical channel counters. Deprecated for backward compatibility.
 *
 * @param[in] och_id Optical channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_och_stats_fn)(
        _In_ otai_object_id_t och_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get optical channel statistics counters extended.
 *
 * @param[in] och_id Optical channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_och_stats_ext_fn)(
        _In_ otai_object_id_t och_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear optical channel statistics counters.
 *
 * @param[in] och_id Optical channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_och_stats_fn)(
        _In_ otai_object_id_t och_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_och_api_t
{
    otai_create_och_fn              create_och;
    otai_remove_och_fn              remove_och;
    otai_set_och_attribute_fn       set_och_attribute;
    otai_get_och_attribute_fn       get_och_attribute;
    otai_get_och_stats_fn           get_och_stats;
    otai_get_och_stats_ext_fn       get_och_stats_ext;
    otai_clear_och_stats_fn         clear_och_stats;
} otai_och_api_t;

/**
 * @}
 */
#endif /** __OTAIOCH_H_ */
