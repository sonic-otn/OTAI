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
 * @file    otaimediachannel.h
 * @brief   This module defines the media channel for the OTAI
 */

#if !defined (__OTAIMEDIACHANNEL_H_)
#define __OTAIMEDIACHANNEL_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIMEDIACHANNEL OTAI - media channel specific API definitions
 *
 * @{
 */

/**
 * @brief The attenuation control modes on a media channel.
 */
typedef enum _otai_mediachannel_attenuation_control_mode_t
{
    OTAI_MEDIACHANNEL_ATTENUATION_CONTROL_MODE_ATTENUATION_FIXED_LOSS,
    OTAI_MEDIACHANNEL_ATTENUATION_CONTROL_MODE_ATTENUATION_DYNAMIC_LOSS,
    OTAI_MEDIACHANNEL_ATTENUATION_CONTROL_MODE_ATTENUATION_DYNAMIC_LOSS_DAMPED,
    OTAI_MEDIACHANNEL_ATTENUATION_CONTROL_MODE_MAX,
} otai_mediachannel_attenuation_control_mode_t;

/**
 * @brief Media channel attribute IDs
 */
typedef enum _otai_mediachannel_attr_t
{
    /**
     * @brief Start of attributes
     */
    OTAI_MEDIACHANNEL_ATTR_START,

    /**
     * @brief ID
     *
     * @type otai_uint32_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     */
    OTAI_MEDIACHANNEL_ATTR_ID = OTAI_MEDIACHANNEL_ATTR_START,

    /**
     * @brief Lower frequency
     *
     * @type otai_uint64_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_LOWER_FREQUENCY,

    /**
     * @brief Upper frequency
     *
     * @type otai_uint64_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_UPPER_FREQUENCY,

    /**
     * @brief Admin status
     *
     * @type otai_admin_status_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_ADMIN_STATUS,

    /**
     * @brief Super channel
     *
     * @type bool
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_SUPER_CHANNEL,

    /**
     * @brief Super channel parent
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_SUPER_CHANNEL_PARENT,

    /**
     * @brief Attenuation control mode
     *
     * @type otai_mediachannel_attenuation_control_mode_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_ATTENUATION_CONTROL_MODE,

    /**
     * @brief Source port name
     *
     * @type char
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_SOURCE_PORT_NAME,

    /**
     * @brief Destination port name
     *
     * @type char
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_DEST_PORT_NAME,

    /**
     * @brief Operational state
     *
     * @type otai_oper_status_t
     * @flags READ_ONLY
     */
    OTAI_MEDIACHANNEL_ATTR_OPER_STATUS,

    /**
     * @brief Attenuation value
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_ATTENUATION_VALUE,

    /**
     * @brief Wait to restore time
     *
     * @type otai_uint32_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_WAIT_TO_RESTORE_TIME,

    /**
     * @brief Target power
     *
     * @type otai_double_t
     * @flags CREATE_AND_SET
     */
    OTAI_MEDIACHANNEL_ATTR_TARGET_POWER,

    /**
     * @brief End of attributes
     */
    OTAI_MEDIACHANNEL_ATTR_END,

    /** Custom range base value */
    OTAI_MEDIACHANNEL_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    OTAI_MEDIACHANNEL_ATTR_CUSTOM_RANGE_END

} otai_mediachannel_attr_t;

/**
 * @brief Media channel stat IDs
 *
 * @flags Contains flags
 */
typedef enum _otai_mediachannel_stat_t
{
    /**
     * @brief Start of statistics
     */
    OTAI_MEDIACHANNEL_STAT_START,

    /**
     * @brief Actual attenuation
     *
     * @type otai_double_t
     * @unit dB
     * @precision precision2
     * @iscounter false
     */
    OTAI_MEDIACHANNEL_STAT_ACTUAL_ATTENUATION = OTAI_ATTENUATOR_STAT_START,

    /**
     * @brief Input power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_MEDIACHANNEL_STAT_INPUT_POWER,

    /**
     * @brief Output power
     *
     * @type otai_double_t
     * @unit dBm
     * @precision precision2
     * @iscounter false
     */
    OTAI_MEDIACHANNEL_STAT_OUTPUT_POWER,

    /**
     * @brief End of statistics
     */
    OTAI_MEDIACHANNEL_STAT_END,

} otai_mediachannel_stat_t;

/**
 * @brief Create media channel.
 *
 * Allocates and initializes a media channel.
 *
 * @param[out] mediachannel_id Media channel id
 * @param[in] linecard_id Linecard id on which the media channel exists
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_create_mediachannel_fn)(
        _Out_ otai_object_id_t *mediachannel_id,
        _In_ otai_object_id_t linecard_id,
        _In_ uint32_t attr_count,
        _In_ const otai_attribute_t *attr_list);

/**
 * @brief Remove media channel
 *
 * @param[in] mediachannel_id Media channel id
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_remove_mediachannel_fn)(
        _In_ otai_object_id_t mediachannel_id);

/**
 * @brief Set media channel attribute
 *
 * @param[in] mediachannel_id Media channel id
 * @param[in] attr Attribute
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_set_mediachannel_attribute_fn)(
        _In_ otai_object_id_t mediachannel_id,
        _In_ const otai_attribute_t *attr);

/**
 * @brief Get media channel attribute
 *
 * @param[in] mediachannel_id Media channel id
 * @param[in] attr_count Number of attributes
 * @param[inout] attr_list Array of attributes
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_mediachannel_attribute_fn)(
        _In_ otai_object_id_t mediachannel_id,
        _In_ uint32_t attr_count,
        _Inout_ otai_attribute_t *attr_list);

/**
 * @brief Get media channel statistics.
 *
 * @param[in] mediachannel_id Media channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_mediachannel_stats_fn)(
        _In_ otai_object_id_t mediachannel_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Get media channel statistics extended.
 *
 * @param[in] mediachannel_id Media channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 * @param[in] mode Statistics mode
 * @param[out] counters Array of resulting counter values.
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_get_mediachannel_stats_ext_fn)(
        _In_ otai_object_id_t mediachannel_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids,
        _In_ otai_stats_mode_t mode,
        _Out_ otai_stat_value_t *counters);

/**
 * @brief Clear media channel statistics counters.
 *
 * @param[in] mediachannel_id Media channel id
 * @param[in] number_of_counters Number of counters in the array
 * @param[in] counter_ids Specifies the array of counter ids
 *
 * @return #OTAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef otai_status_t (*otai_clear_mediachannel_stats_fn)(
        _In_ otai_object_id_t mediachannel_id,
        _In_ uint32_t number_of_counters,
        _In_ const otai_stat_id_t *counter_ids);

/**
 * @brief Routing interface methods table retrieved with otai_api_query()
 */
typedef struct _otai_mediachannel_api_t
{
    otai_create_mediachannel_fn                create_mediachannel;
    otai_remove_mediachannel_fn                remove_mediachannel;
    otai_set_mediachannel_attribute_fn         set_mediachannel_attribute;
    otai_get_mediachannel_attribute_fn         get_mediachannel_attribute;
    otai_get_mediachannel_stats_fn             get_mediachannel_stats;
    otai_get_mediachannel_stats_ext_fn         get_mediachannel_stats_ext;
    otai_clear_mediachannel_stats_fn           clear_mediachannel_stats;
} otai_mediachannel_api_t;

/**
 * @}
 */
#endif /** __OTAIMEDIACHANNEL_H_ */
