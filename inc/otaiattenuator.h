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
 * @file    otaiattenuator.h
 * @brief   This module defines the attenuator for the OTAI
 */

#if !defined (__OTAIATTENUATOR_H_)
#define __OTAIATTENUATOR_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIATTENUATOR OTAI - Attenuator specific API definitions
 *
 * @{
 */

/** @brief Attenuation mode */
typedef enum _otai_attenuator_attenuation_mode_t
{
    OTAI_ATTENUATOR_ATTENUATION_MODE_CONSTANT_POWER,
    OTAI_ATTENUATOR_ATTENUATION_MODE_CONSTANT_ATTENUATION,
} otai_attenuator_attenuation_mode_t;

/**
 * @brief Attenuator attribute IDs
 */
typedef enum _otai_attenuator_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_ATTENUATOR_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_ATTENUATOR_ATTR_ID = OTAI_ATTENUATOR_ATTR_START,

    /**
     * @brief Attenuation mode
     *
     * @type otai_attenuator_attenuation_mode_t
     * @flags CREATE_AND_SET
     * @default OTAI_ATTENUATOR_ATTENUATION_MODE_CONSTANT_ATTENUATION
     */
    OTAI_ATTENUATOR_ATTR_ATTENUATION_MODE,

    /**
     * @brief Target output power
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -60.0
     */
    OTAI_ATTENUATOR_ATTR_TARGET_OUTPUT_POWER,

    /**
     * @brief Attenuation
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     * @default -60.0
     */
    OTAI_ATTENUATOR_ATTR_ATTENUATION,

    /**
     * @brief Enabled
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default true
     */
    OTAI_ATTENUATOR_ATTR_ENABLED,

    /**
     * @brief Ingress port
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_ATTENUATOR_ATTR_INGRESS_PORT,

    /**
     * @brief Egress port
     *
     * @type char
     * @flags READ_ONLY
     */
    OTAI_ATTENUATOR_ATTR_EGRESS_PORT,

    /**
     * @brief Fix attenuation
     *
     * @type otai_double_t
     * @flags READ_ONLY
     */
    OTAI_ATTENUATOR_ATTR_FIX_ATTENUATION,

    /**
     * @brief End of attributes
     */
    OTAI_ATTENUATOR_ATTR_END,

    /** Custom range base value */
    OTAI_ATTENUATOR_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_ATTENUATOR_ATTR_CUSTOM_RANGE_END

} otai_attenuator_attr_t;

/**
 * @brief ATTENUATOR stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_attenuator_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_ATTENUATOR_STAT_START,

    /**
     * @brief Actual attenuation
     *
     * @type otai_double_t
     * @unit dB
     * @precision precision2
     * @iscounter false
     */
    OTAI_ATTENUATOR_STAT_ACTUAL_ATTENUATION = OTAI_ATTENUATOR_STAT_START,

    /**
     * @brief Output power total
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_ATTENUATOR_STAT_OUTPUT_POWER_TOTAL,

    /**
     * @brief Optical return loss
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_ATTENUATOR_STAT_OPTICAL_RETURN_LOSS,

    /**
     * @brief End of statistics
     */
    OTAI_ATTENUATOR_STAT_END,

} otai_attenuator_stat_t;

/**
 * @brief Create ATTENUATOR.
 *
 * Allocates and initializes a attenuator.
 *
 * @param[out] attenuator_id ATTENUATOR id
 * @param[in] linecard_id Linecard id on which the attenuator exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_attenuator_fn)(
        _Out_ otai_object_id_t *attenuator_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove attenuator
 *
 * @param[in] attenuator_id ATTENUATOR id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_attenuator_fn)(
        _In_ otai_object_id_t attenuator_id);

/**
 * @brief Set attenuator attribute
 *
 * @param[in] attenuator_id ATTENUATOR id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_attenuator_attribute_fn)(
        _In_ otai_object_id_t attenuator_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get attenuator attribute
 *
 * @param[in] attenuator_id ATTENUATOR id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_attenuator_attribute_fn)(
        _In_ otai_object_id_t attenuator_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get attenuator statistics.
 *
 * @param[in] attenuator_id ATTENUATOR id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_attenuator_stats_fn)(
        _In_ otai_object_id_t attenuator_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get attenuator statistics extended.
 *
 * @param[in] attenuator_id ATTENUATOR id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_attenuator_stats_ext_fn)(
        _In_ otai_object_id_t attenuator_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear attenuator statistics counters.
 *
 * @param[in] attenuator_id ATTENUATOR id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_attenuator_stats_fn)(
        _In_ otai_object_id_t attenuator_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_attenuator_api_t
{
    otai_create_attenuator_fn                create_attenuator;
    otai_remove_attenuator_fn                remove_attenuator;
    otai_set_attenuator_attribute_fn         set_attenuator_attribute;
    otai_get_attenuator_attribute_fn         get_attenuator_attribute;
    otai_get_attenuator_stats_fn             get_attenuator_stats;
    otai_get_attenuator_stats_ext_fn         get_attenuator_stats_ext;
    otai_clear_attenuator_stats_fn           clear_attenuator_stats;
} otai_attenuator_api_t;

/**
 * @}
 */
#endif /** __OTAIATTENUATOR_H_ */
